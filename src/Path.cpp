// -*-c++-*- osgNVPR - Copyright (C) 2011 osgNVPR Development Team
// $Id: Path.cpp 29 2012-10-24 04:14:12Z cubicool $

#include <algorithm>
#include <osgNVPR/io_utils>
#include <osgNVPR/Path>

namespace osgNVPR {

Path::Path():
_name(0) {
	setUseDisplayList(true);
	setUseVertexBufferObjects(false);
}

// TODO: This!
Path::Path(const Path& path, const osg::CopyOp& copyOp):
Drawable(path, copyOp) {
}

void Path::releaseGLObjects(osg::State* state) const {
	osg::Drawable::releaseGLObjects(state);

	if(!state) return;

	osgNVPR::Extensions* ext = getNVPRExtensions(state);

	if(!ext) return;

	if(_name) ext->glDeletePathsNV(_name, 1);
}

void Path::fill(osgNVPR::Extensions* ext, osg::State& state) const {
	_fillPaint->prePaint(ext, state);
	
	ext->glStencilFillPathNV(_name, _getStencilMode(_fillStencilMode), _fillMask);
	
	_fillPaint->paint(ext, state);

	ext->glCoverFillPathNV(_name, _getFillCoverMode());
	
	_fillPaint->postPaint(ext, state);
}

// This routine is very similar to the one above.
void Path::stroke(osgNVPR::Extensions* ext, osg::State& state) const {
	_strokePaint->prePaint(ext, state);
	
	ext->glStencilStrokePathNV(_name, _getStencilMode(_strokeStencilMode), _strokeMask);
	
	_strokePaint->paint(ext, state);

	ext->glCoverStrokePathNV(_name, _getStrokeCoverMode());
	
	_strokePaint->postPaint(ext, state);
}

#define __computeBoundingBox(bt, bb) \
	ext->glGetPathParameterfvNV(_name, bt, bounds); \
	bb.set( \
		osg::Vec3(bounds[0], bounds[1], 0.0f), \
		osg::Vec3(bounds[2], bounds[3], 0.0f) \
	);

void Path::_computeBoundingBoxes(osgNVPR::Extensions* ext) const {
	GLfloat bounds[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	__computeBoundingBox(GL_PATH_OBJECT_BOUNDING_BOX_NV, _objectBoundingBox)
	__computeBoundingBox(GL_PATH_FILL_BOUNDING_BOX_NV, _fillBoundingBox)
	__computeBoundingBox(GL_PATH_STROKE_BOUNDING_BOX_NV, _strokeBoundingBox)

	/*
	OSG_WARN
		<< _objectBoundingBox << std::endl
		<< _fillBoundingBox << std::endl
		<< _strokeBoundingBox << std::endl
	;
	*/

	const_cast<Path*>(this)->dirtyBound();
}

#define __setParameter(p, t) \
	if(p.isSet()) ext->glPathParameter##t##NV(_name, p.param(), p.get())

void Path::_setParameters(osgNVPR::Extensions* ext) const {
	__setParameter(_parameters.strokeWidth, f);
	__setParameter(_parameters.intialEndCap, i);
	__setParameter(_parameters.terminalEndCap, i);
	__setParameter(_parameters.initialDashCap, i);
	__setParameter(_parameters.terminalDashCap, i);
	__setParameter(_parameters.joinStyle, i);
	__setParameter(_parameters.miterLimit, f);
	__setParameter(_parameters.dashOffset, f);
	__setParameter(_parameters.dashOffsetReset, i);
	__setParameter(_parameters.clientLength, f);
	__setParameter(_parameters.fillMode, i);
	__setParameter(_parameters.fillMask, i);
	__setParameter(_parameters.fillCoverMode, i);
	__setParameter(_parameters.strokeCoverMode, i);
}

}

