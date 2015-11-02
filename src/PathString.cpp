// -*-c++-*- osgNVPR - Copyright (C) 2011 osgNVPR Development Team
// $Id: PathString.cpp 27 2012-10-01 15:57:26Z cubicool $

#include <osgNVPR/PathString>

namespace osgNVPR {

PathString::PathString(Format format):
_format(format) {
}

void PathString::append(const std::string& str) {
	_str += str;
}

void PathString::compileGLObjects(osg::RenderInfo& renderInfo) const {
	osgNVPR::Extensions* ext = getNVPRExtensions(renderInfo);

	if(!ext) return;

	_name = ext->glGenPathsNV(1);

	ext->glPathStringNV(_name, _format, _str.size(), _str.c_str());

	/*
	if(glGetError()) {
		// Potential errors here...
		// INVALID_ENUM
		// INVALID_OPERATION
		// INVALID_VALUE

		OSG_WARN << "Error in PathString::compileGLObjects." << std::endl;
	}
	*/

	_computeBoundingBoxes(ext);
	_setParameters(ext);

	osgNVPR::Drawable::compileGLObjects(renderInfo);
}

}

