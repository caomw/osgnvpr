#include <osgNVPR/Font>

#include <algorithm>

namespace osgNVPR {

// I hate these magic numbers, but. Well. Yeah.
const unsigned int NUM_FONT_METRICS  = 13;
const unsigned int NUM_GLYPH_METRICS = 9;

Font::FontMetrics::FontMetrics(const GLfloat* data):
xMinBounds         (0.0f),
yMinBounds         (0.0f),
xMaxBounds         (0.0f),
yMaxBounds         (0.0f),
unitsPerEm         (0.0f),
ascender           (0.0f),
descender          (0.0f),
height             (0.0f),
maxAdvanceWidth    (0.0f),
maxAdvanceHeight   (0.0f),
underlinePosition  (0.0f),
underlineThickness (0.0f),
hasKerning         (false) {
	if(!data) return;

	xMinBounds         = data[0];
	yMinBounds         = data[1];
	xMaxBounds         = data[2];
	yMaxBounds         = data[3];
	unitsPerEm         = data[4];
	ascender           = data[5];
	descender          = data[6];
	height             = data[7];
	maxAdvanceWidth    = data[8];
	maxAdvanceHeight   = data[9];
	underlinePosition  = data[10];
	underlineThickness = data[11];

	// TODO: THIS APPEARS TO BE BUGGED IN THE DRIVER...
	hasKerning = !data[12] ? false : true;
}

Font::GlyphMetrics::GlyphData::GlyphData():
width                    (0.0f),
height                   (0.0f),
horizontalBearingX       (0.0f),
horizontalBearingY       (0.0f),
horizontalBearingAdvance (0.0f),
verticalBearingX         (0.0f),
verticalBearingY         (0.0f),
verticalBearingAdvance   (0.0f),
hasKerning               (false) {
}

void Font::GlyphMetrics::GlyphData::setData(const GLfloat* data) {
	if(!data) return;

	width                    = data[0];
	height                   = data[1];
	horizontalBearingX       = data[2];
	horizontalBearingY       = data[3];
	horizontalBearingAdvance = data[4];
	verticalBearingX         = data[5];
	verticalBearingY         = data[6];
	verticalBearingAdvance   = data[7];

	// TODO: THIS APPEARS TO BE BUGGED IN THE DRIVER...
	hasKerning = !data[8] ? false : true;
};

Font::GlyphMetrics::GlyphMetrics(GLsizei numGlyphs, const GLfloat* data) {
	_data.resize(numGlyphs);
	_coords.resize(numGlyphs);

	if(!data) return;

	for(GLint i = 0; i < numGlyphs; i++) _data[i].setData(&data[NUM_GLYPH_METRICS * i]);
}

const Font::GlyphMetrics::GlyphData* Font::GlyphMetrics::getGlyphData(Character character) const {
	return &_data[character];
}

const Font::GlyphMetrics::CoordArray* Font::GlyphMetrics::getGlyphCoords(Character character) const {
	return _coords[character];
}

Font::Font(
	const std::string& font,
	Style              style,
	GLfloat            scale,
	MissingGlyph       mg
):
_glyphBase (0),
_numGlyphs (128),
_compiled  (false) {
	setFont(font);
	setStyle(style);
	setScale(scale);
	setHandleMissingGlyph(mg);
}

Font::~Font() {
	// releaseGLObjects();
}

void Font::compileGLObjects(osg::RenderInfo& renderInfo) {
	if(_compiled) return;

	osgNVPR::Extensions* ext = getNVPRExtensions(renderInfo);

	if(!ext) return;

	releaseGLObjects(renderInfo.getState());

	_glyphBase = ext->glGenPathsNV(_numGlyphs);

	// TODO: Settle this...
	if(_pathTemplate.valid()) {
		_pathTemplate->compileGLObjects(renderInfo);
	}

	ext->glPathGlyphRangeNV(
		_glyphBase,
		_target,
		_name.c_str(),
		_style,
		0,
		_numGlyphs,
		_missingGlyph,
		_pathTemplate.valid() ? _pathTemplate->getName() : 0,
		_scale
	);

	_setFontMetrics(ext);
	_setGlyphMetrics(ext);

	_compiled = true;
}

void Font::releaseGLObjects(osg::State* state) {
	if(!_compiled) return;

	if(!state) return;

	osgNVPR::Extensions* ext = getNVPRExtensions(state);

	if(!ext) return;

	ext->glDeletePathsNV(_glyphBase, _numGlyphs);

	_fontMetrics  = 0;
	_glyphMetrics = 0;
	_compiled     = false;
}

GLfloat* Font::getTranslations(
	osgNVPR::Extensions* ext,
	const String&        str,
	unsigned int         offset,
	unsigned int         length
) const {
	if(!_compiled) return 0;

	GLfloat* translations = new GLfloat[length];

	translations[0] = 0.0f;

	ext->glGetPathSpacingNV(
		GL_ACCUM_ADJACENT_PAIRS_NV,
		length,
		GL_UNSIGNED_INT,
		&str[offset],
		_glyphBase,
		1.0f, // advanceScale
		1.0f, // kerningScale
		GL_TRANSLATE_X_NV,
		translations + 1
	);

	return translations;
}

void Font::setFont(const std::string& name) {
	_name = name;

	// These are the builtin fonts, case-sensitive.
	if(
		name == "Serif" ||
		name == "Sans" ||
		name == "Mono" ||
		name == "Missing"
	) {
		_target = GL_STANDARD_FONT_NAME_NV;

		return;
	}

	// Otherwise, convert to lowercase and check for .ttf.
	std::string font(name);

	std::transform(font.begin(), font.end(), font.begin(), ::tolower);

	if(font.substr(font.size() - 4) == ".ttf") _target = GL_FILE_NAME_NV;

	else _target = GL_SYSTEM_FONT_NAME_NV;
}

void Font::_setFontMetrics(osgNVPR::Extensions* ext) {
	GLbitfield mask =
		GL_FONT_X_MIN_BOUNDS_NV |
		GL_FONT_Y_MIN_BOUNDS_NV |
		GL_FONT_X_MAX_BOUNDS_NV |
		GL_FONT_Y_MAX_BOUNDS_NV |
		GL_FONT_UNITS_PER_EM_NV |
		GL_FONT_ASCENDER_NV |
		GL_FONT_DESCENDER_NV |
		GL_FONT_HEIGHT_NV |
		GL_FONT_MAX_ADVANCE_WIDTH_NV |
		GL_FONT_MAX_ADVANCE_HEIGHT_NV |
		GL_FONT_UNDERLINE_POSITION_NV |
		GL_FONT_UNDERLINE_THICKNESS_NV |
		GL_FONT_HAS_KERNING_NV
	;

	GLfloat* data = new GLfloat[NUM_FONT_METRICS];

	// Why in the world do we do + ' ' here? Because we need a valid glyph,
	// and many of the first glyphs are usually all null.
	ext->glGetPathMetricRangeNV(mask, _glyphBase + ' ', 1, 0, data);

	_fontMetrics = new FontMetrics(data);

	delete[] data;
}

void Font::_setGlyphMetrics(osgNVPR::Extensions* ext) {
	GLbitfield mask =
		GL_GLYPH_WIDTH_BIT_NV |
		GL_GLYPH_HEIGHT_BIT_NV |
		GL_GLYPH_HORIZONTAL_BEARING_X_BIT_NV |
		GL_GLYPH_HORIZONTAL_BEARING_Y_BIT_NV |
		GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV |
		GL_GLYPH_VERTICAL_BEARING_X_BIT_NV |
		GL_GLYPH_VERTICAL_BEARING_Y_BIT_NV |
		GL_GLYPH_VERTICAL_BEARING_ADVANCE_BIT_NV |
		GL_GLYPH_HAS_KERNING_NV
	;

	GLfloat* data = new GLfloat[NUM_GLYPH_METRICS * _numGlyphs];

	ext->glGetPathMetricRangeNV(mask, _glyphBase, _numGlyphs, 0, data);

	_glyphMetrics = new GlyphMetrics(_numGlyphs, data);

	for(int i = 0; i < _numGlyphs; i++) {
		if(!ext->glIsPathNV(_glyphBase + i)) continue;

		GLint numCoords = 0;

		// Get the number of coordinates from the glyph in question.
		ext->glGetPathParameterivNV(_glyphBase + i, GL_PATH_COORD_COUNT_NV, &numCoords);

		if(numCoords <= 0) continue;

		// Preallocate our CoordArray (osg::Vec2Array) so that we can efficiently
		// set the coordinates laters.
		_glyphMetrics->_coords[i] = new GlyphMetrics::CoordArray(numCoords);

		// TODO: I'm not sure if a CoordArray will guarantee that the sequence of
		// Vec2 objects will be sequential; investigate this and possibly optimize
		// this temporary variable away.
		GLfloat* coords = new GLfloat[numCoords * 2];

		ext->glGetPathCoordsNV(_glyphBase + i, coords);

		// Set the _coords array from our temporary variable.
		for(int j = 0; j < numCoords; j++) {
			(*_glyphMetrics->_coords[i])[j].set(coords[j * 2], coords[(j * 2) + 1]);
		}

		delete[] coords;
	}

	delete[] data;
}

}

