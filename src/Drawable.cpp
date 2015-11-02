// -*-c++-*- osgNVPR - Copyright (C) 2011 osgNVPR Development Team
// $Id: Drawable.cpp 27 2012-10-01 15:57:26Z cubicool $

#include <osgNVPR/Drawable>

namespace osgNVPR {

Drawable::Drawable():
_paintMethod       (FILL_ONLY),
_fillStencilMode   (COUNT_UP),
_fillMask          (0xFF),
_fillCoverMode     (BOUNDING_BOX),
_strokeStencilMode (COUNT_UP),
_strokeMask        (0xFF),
_strokeCoverMode   (BOUNDING_BOX),
_boundingBoxMode   (OBJECT) {
}

// TODO: This!
Drawable::Drawable(const Drawable& path, const osg::CopyOp& copyOp):
osg::Drawable(path, copyOp) {
}

void Drawable::drawImplementation(osg::RenderInfo& renderInfo) const {
	osgNVPR::Extensions* ext = getNVPRExtensions(renderInfo);

	if(!ext) return;

	osg::State* state = renderInfo.getState();

	if(!state) return;

	switch(_paintMethod) {
		case FILL_ONLY:
			if(_fillPaint.valid()) fill(ext, *state);

			break;
	
		case STROKE_ONLY:
			if(_strokePaint.valid()) stroke(ext, *state);

			break;

		case FILL_THEN_STROKE:
			if(_fillPaint.valid()) fill(ext, *state);
			if(_strokePaint.valid()) stroke(ext, *state);

			break;
	
		case STROKE_THEN_FILL:
			if(_strokePaint.valid()) stroke(ext, *state);
			if(_fillPaint.valid()) fill(ext, *state);

			break;
	}
}

void Drawable::compileGLObjects(osg::RenderInfo& renderInfo) const {
	osg::Drawable::compileGLObjects(renderInfo);

	_compiled = true;
}

void Drawable::releaseGLObjects(osg::State* state) const {
	osg::Drawable::releaseGLObjects(state);

	_compiled = false;
}

osg::BoundingBox Drawable::computeBound() const {
	if(_boundingBoxMode == OBJECT) return _objectBoundingBox;

	else if(_boundingBoxMode == FILL) return _fillBoundingBox;

	else return _strokeBoundingBox;
}

GLint Drawable::_getStencilMode(StencilMode sm) const {
	if(sm == PATH_STENCIL) return GL_PATH_FILL_MODE_NV;

	else return sm;
}

GLenum Drawable::_getFillCoverMode() const {
	if(_fillCoverMode == PATH_COVER) return GL_PATH_FILL_COVER_MODE_NV;

	else return _fillCoverMode;
}

GLenum Drawable::_getStrokeCoverMode() const {
	if(_strokeCoverMode == PATH_COVER) return GL_PATH_STROKE_COVER_MODE_NV;

	else return _strokeCoverMode;
}

}

