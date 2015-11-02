#include <osgNVPR/Text>

namespace osgNVPR {

Text::Text(Font* font):
_font(font) {
	setUseDisplayList(true);
	setUseVertexBufferObjects(false);
}

Text::Text(const String& text, Font* font):
_font(font) {
	setUseDisplayList(true);
	setUseVertexBufferObjects(false);
	setText(text);

	// Text changes the defaults from Drawable a bit (which are designed
	// to work naturally with the Path objects instead).
	_fillStencilMode   = PATH_STENCIL;
	_fillCoverMode     = BOUNDING_BOXES;
	_strokeStencilMode = PATH_STENCIL;
	_strokeCoverMode   = BOUNDING_BOXES;
}


// TODO: This will always set the baseline to be the lowest Y, although that will
//       need to instead depend on the boundingBoxMode!
#define __expand(bt, bb, sb) \
	ext->glGetPathParameterfvNV(_font->getGlyphBase() + _text[i], bt, bounds); \
	xy = osg::Vec3((*_translations)[i], 0.0f); \
	bb.expandBy(osg::Vec3(bounds[0], bounds[1], 0.0f) + xy); \
	bb.expandBy(osg::Vec3(bounds[2], bounds[3], 0.0f) + xy); \
	if(sb && xy.y() < _baseline) _baseline = xy.y();

void Text::_computeBoundingBoxes(osgNVPR::Extensions* ext) const {
	for(String::vector_type::value_type i = 0; i < _text.size(); i++) {
		// TODO: Determine which Text glyphs are invalid in some way so we don't
		// try to get the bounding box of '\n' for example.
		const Font::GlyphMetrics::GlyphData* data = _font->getGlyphMetrics()->getGlyphData(_text[i]);

		if(!data || (data->width <= 0.0f && data->height <= 0.0f)) continue;

		GLfloat   bounds[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		osg::Vec3 xy;

		__expand(GL_PATH_OBJECT_BOUNDING_BOX_NV, _objectBoundingBox, _boundingBoxMode == OBJECT)
		__expand(GL_PATH_FILL_BOUNDING_BOX_NV, _fillBoundingBox, _boundingBoxMode == FILL)
		__expand(GL_PATH_STROKE_BOUNDING_BOX_NV, _strokeBoundingBox, _boundingBoxMode == STROKE)
	}

	const_cast<Text*>(this)->dirtyBound();
}

void Text::compileGLObjects(osg::RenderInfo& renderInfo) const {
	if(!_font.valid() || !_layout.valid()) {
		OSG_WARN
			<< "Cannot compile an osgNVPR::Text object without having "
			<< "valid Layout and Font objects!" << std::endl
		;

		return;
	}

	// TODO: Check to determine WHEN this should be called.
	_font->compileGLObjects(renderInfo);

	osgNVPR::Extensions* ext = getNVPRExtensions(renderInfo);

	if(!ext) return;

	_translations = _layout->layout(ext, _font, _text);

	_computeBoundingBoxes(ext);

	// Call the base compile to setup display lists and whatnot.
	osgNVPR::Drawable::compileGLObjects(renderInfo);
}

void Text::releaseGLObjects(osg::State* state) const {
	osgNVPR::Drawable::releaseGLObjects(state);

	// _font->releaseGLObjects(state);
}

void Text::fill(osgNVPR::Extensions* ext, osg::State& state) const {
	_fillPaint->prePaint(ext, state);

	ext->glStencilFillPathInstancedNV(
		_text.size(),
		GL_UNSIGNED_INT,
		&_text[0],
		_font->getGlyphBase(),
		_getStencilMode(_fillStencilMode),
		_fillMask,
		GL_TRANSLATE_2D_NV,
		static_cast<const GLfloat*>(_translations->getDataPointer())
	);

	_fillPaint->paint(ext, state);

	ext->glCoverFillPathInstancedNV(
		_text.size(),
		GL_UNSIGNED_INT,
		&_text[0],
		_font->getGlyphBase(),
		_getFillCoverMode(),
		GL_TRANSLATE_2D_NV,
		static_cast<const GLfloat*>(_translations->getDataPointer())
	);

	_fillPaint->postPaint(ext, state);
}

void Text::stroke(osgNVPR::Extensions* ext, osg::State& state) const {
	_strokePaint->prePaint(ext, state);
	
	ext->glStencilStrokePathInstancedNV(
		_text.size(),
		GL_UNSIGNED_INT,
		&_text[0],
		_font->getGlyphBase(),
		_getStencilMode(_strokeStencilMode),
		_strokeMask, 
		GL_TRANSLATE_2D_NV,
		static_cast<const GLfloat*>(_translations->getDataPointer())
	);

	_strokePaint->paint(ext, state);

	ext->glCoverStrokePathInstancedNV(
		_text.size(),
		GL_UNSIGNED_INT,
		&_text[0],
		_font->getGlyphBase(),
		_getStrokeCoverMode(),
		GL_TRANSLATE_2D_NV,
		static_cast<const GLfloat*>(_translations->getDataPointer())
	);

	_strokePaint->postPaint(ext, state);
}

void Text::setText(const String& text) {
	_text = text;

	_dirty();
}

void Text::setFont(Font* font) {
	_font = font;

	_dirty();
}

void Text::setLayout(Layout* layout) {
	_layout = layout;

	_dirty();
}

void Text::_dirty() {
	// We need to have a valid Font and Layout to even start.
	if(!_font.valid() || !_layout.valid()) return;

	// Clear out translations.
	_translations = 0;

	dirtyDisplayList();
}

}

