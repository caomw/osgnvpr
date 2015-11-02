// -*-c++-*- osgNVPR - Copyright (C) 2011 osgNVPR Development Team
// $Id: Extensions.cpp 27 2012-10-01 15:57:26Z cubicool $

#include <osg/GLExtensions>
#include <osg/State>
#include <osgNVPR/Extensions>

namespace osgNVPR {

#define LOAD_NVPR_PROC(name) osg::setGLExtensionFuncPtr(name, (#name), (std::string(#name)).c_str())

Extensions::Extensions(unsigned int contextID):
glGenPathsNV                   (0),
glDeletePathsNV                (0),
glIsPathNV                     (0),
glPathCommandsNV               (0),
glPathCoordsNV                 (0),
glPathSubCommandsNV            (0),
glPathSubCoordsNV              (0),
glPathStringNV                 (0),
glPathGlyphsNV                 (0),
glPathGlyphRangeNV             (0),
glWeightPathsNV                (0),
glCopyPathNV                   (0),
glInterpolatePathsNV           (0),
glTransformPathNV              (0),
glPathParameterivNV            (0),
glPathParameteriNV             (0),
glPathParameterfvNV            (0),
glPathParameterfNV             (0),
glPathDashArrayNV              (0),
glPathStencilFuncNV            (0),
glPathStencilDepthOffsetNV     (0),
glStencilFillPathNV            (0),
glStencilStrokePathNV          (0),
glStencilFillPathInstancedNV   (0),
glStencilStrokePathInstancedNV (0),
glPathCoverDepthFuncNV         (0),
glPathColorGenNV               (0),
glPathTexGenNV                 (0),
glPathFogGenNV                 (0),
glCoverFillPathNV              (0),
glCoverStrokePathNV            (0),
glCoverFillPathInstancedNV     (0),
glCoverStrokePathInstancedNV   (0),
glGetPathParameterivNV         (0),
glGetPathParameterfvNV         (0),
glGetPathCommandsNV            (0),
glGetPathCoordsNV              (0),
glGetPathDashArrayNV           (0),
glGetPathMetricsNV             (0),
glGetPathMetricRangeNV         (0),
glGetPathSpacingNV             (0),
glGetPathColorGenivNV          (0),
glGetPathColorGenfvNV          (0),
glGetPathTexGenivNV            (0),
glGetPathTexGenfvNV            (0),
glIsPointInFillPathNV          (0),
glIsPointInStrokePathNV        (0),
glGetPathLengthNV              (0),
glPointAlongPathNV             (0),
_supported                     (false) {
	initialize(contextID);
}

bool Extensions::initialize(unsigned int contextID) {
	if(_supported) return true;

	if(!osg::isGLExtensionSupported(contextID, "GL_NV_path_rendering")) {
		_supported = false;

		return false;
	}

	LOAD_NVPR_PROC(glGenPathsNV);
	LOAD_NVPR_PROC(glDeletePathsNV);
	LOAD_NVPR_PROC(glIsPathNV);
	LOAD_NVPR_PROC(glPathCommandsNV);
	LOAD_NVPR_PROC(glPathCoordsNV);
	LOAD_NVPR_PROC(glPathSubCommandsNV);
	LOAD_NVPR_PROC(glPathSubCoordsNV);
	LOAD_NVPR_PROC(glPathStringNV);
	LOAD_NVPR_PROC(glPathGlyphsNV);
	LOAD_NVPR_PROC(glPathGlyphRangeNV);
	LOAD_NVPR_PROC(glWeightPathsNV);
	LOAD_NVPR_PROC(glCopyPathNV);
	LOAD_NVPR_PROC(glInterpolatePathsNV);
	LOAD_NVPR_PROC(glTransformPathNV);
	LOAD_NVPR_PROC(glPathParameterivNV);
	LOAD_NVPR_PROC(glPathParameteriNV);
	LOAD_NVPR_PROC(glPathParameterfvNV);
	LOAD_NVPR_PROC(glPathParameterfNV);
	LOAD_NVPR_PROC(glPathDashArrayNV);
	LOAD_NVPR_PROC(glPathStencilFuncNV);
	LOAD_NVPR_PROC(glPathStencilDepthOffsetNV);
	LOAD_NVPR_PROC(glStencilFillPathNV);
	LOAD_NVPR_PROC(glStencilStrokePathNV);
	LOAD_NVPR_PROC(glStencilFillPathInstancedNV);
	LOAD_NVPR_PROC(glStencilStrokePathInstancedNV);
	LOAD_NVPR_PROC(glPathCoverDepthFuncNV);
	LOAD_NVPR_PROC(glPathColorGenNV);
	LOAD_NVPR_PROC(glPathTexGenNV);
	LOAD_NVPR_PROC(glPathFogGenNV);
	LOAD_NVPR_PROC(glCoverFillPathNV);
	LOAD_NVPR_PROC(glCoverStrokePathNV);
	LOAD_NVPR_PROC(glCoverFillPathInstancedNV);
	LOAD_NVPR_PROC(glCoverStrokePathInstancedNV);
	LOAD_NVPR_PROC(glGetPathParameterivNV);
	LOAD_NVPR_PROC(glGetPathParameterfvNV);
	LOAD_NVPR_PROC(glGetPathCommandsNV);
	LOAD_NVPR_PROC(glGetPathCoordsNV);
	LOAD_NVPR_PROC(glGetPathDashArrayNV);
	LOAD_NVPR_PROC(glGetPathMetricsNV);
	LOAD_NVPR_PROC(glGetPathMetricRangeNV);
	LOAD_NVPR_PROC(glGetPathSpacingNV);
	LOAD_NVPR_PROC(glGetPathColorGenivNV);
	LOAD_NVPR_PROC(glGetPathColorGenfvNV);
	LOAD_NVPR_PROC(glGetPathTexGenivNV);
	LOAD_NVPR_PROC(glGetPathTexGenfvNV);
	LOAD_NVPR_PROC(glIsPointInFillPathNV);
	LOAD_NVPR_PROC(glIsPointInStrokePathNV);
	LOAD_NVPR_PROC(glGetPathLengthNV);
	LOAD_NVPR_PROC(glPointAlongPathNV);

	_supported = 
		glGenPathsNV != 0 &&
		glDeletePathsNV != 0 &&
		glIsPathNV != 0 &&
		glPathCommandsNV != 0 &&
		glPathCoordsNV != 0 &&
		glPathSubCommandsNV != 0 &&
		glPathSubCoordsNV != 0 &&
		glPathStringNV != 0 &&
		glPathGlyphsNV != 0 &&
		glPathGlyphRangeNV != 0 &&
		glWeightPathsNV != 0 &&
		glCopyPathNV != 0 &&
		glInterpolatePathsNV != 0 &&
		glTransformPathNV != 0 &&
		glPathParameterivNV != 0 &&
		glPathParameteriNV != 0 &&
		glPathParameterfvNV != 0 &&
		glPathParameterfNV != 0 &&
		glPathDashArrayNV != 0 &&
		glPathStencilFuncNV != 0 &&
		glPathStencilDepthOffsetNV != 0 &&
		glStencilFillPathNV != 0 &&
		glStencilStrokePathNV != 0 &&
		glStencilFillPathInstancedNV != 0 &&
		glStencilStrokePathInstancedNV != 0 &&
		glPathCoverDepthFuncNV != 0 &&
		glPathColorGenNV != 0 &&
		glPathTexGenNV != 0 &&
		glPathFogGenNV != 0 &&
		glCoverFillPathNV != 0 &&
		glCoverStrokePathNV != 0 &&
		glCoverFillPathInstancedNV != 0 &&
		glCoverStrokePathInstancedNV != 0 &&
		glGetPathParameterivNV != 0 &&
		glGetPathParameterfvNV != 0 &&
		glGetPathCommandsNV != 0 &&
		glGetPathCoordsNV != 0 &&
		glGetPathDashArrayNV != 0 &&
		glGetPathMetricsNV != 0 &&
		glGetPathMetricRangeNV != 0 &&
		glGetPathSpacingNV != 0 &&
		glGetPathColorGenivNV != 0 &&
		glGetPathColorGenfvNV != 0 &&
		glGetPathTexGenivNV != 0 &&
		glGetPathTexGenfvNV != 0 &&
		glIsPointInFillPathNV != 0 &&
		glIsPointInStrokePathNV != 0 &&
		glGetPathLengthNV != 0 &&
		glPointAlongPathNV != 0
	;

	return _supported;
}

static osg::buffered_object<osg::ref_ptr<Extensions> > s_extensions;

Extensions* Extensions::instance(unsigned contextID) {
	Extensions* ext = s_extensions[contextID].get();

	if(!ext) s_extensions[contextID] = new Extensions(contextID);

	if(!s_extensions[contextID]->initialize(contextID)) {
		OSG_WARN << "Failed to initialize osgNVPR!" << std::endl;
	}

	return s_extensions[contextID].get();
}

Extensions* getNVPRExtensions(unsigned int contextID) {
	osgNVPR::Extensions* ext = osgNVPR::Extensions::instance(contextID);

	if(!ext || !ext->isSupported()) {
		OSG_WARN << "GL_NV_path_rendering not supported." << std::endl;

		return 0;
	}

	return ext;
}

}

