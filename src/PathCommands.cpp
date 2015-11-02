// -*-c++-*- osgNVPR - Copyright (C) 2011 osgNVPR Development Team
// $Id: PathCommands.cpp 28 2012-10-04 14:52:04Z cubicool $

#include <osgNVPR/PathCommands>

namespace osgNVPR {

PathCommands::PathCommands():
_commands (0),
_coords   (0) {
	// TODO: Give the use an opportunity to set the sizes of these values.
	_commands = new osg::UByteArray();
	_coords   = new osg::FloatArray();
}

void PathCommands::close() {
	_commands->push_back(CLOSE_PATH);
}

bool PathCommands::append(Command command, ...) {
	va_list vl;

	va_start(vl, command);

	bool ret = _append(command, vl);

	va_end(vl);

	return ret;
}

bool PathCommands::append(char alias, ...) {
	va_list vl;

	va_start(vl, alias);

	bool ret = _append(aliasToCommand(alias), vl);

	va_end(vl);

	return ret;
}

void PathCommands::compileGLObjects(osg::RenderInfo& renderInfo) const {
	osgNVPR::Extensions* ext = getNVPRExtensions(renderInfo);

	if(!ext) return;

	_name = ext->glGenPathsNV(1);

	ext->glPathCommandsNV(
		_name,
		_commands->getNumElements(),
		static_cast<const GLubyte*>(_commands->getDataPointer()),
		_coords->getNumElements(),
		_coords->getDataType(),
		_coords->getDataPointer()
	);

	/*
	if(glGetError()) {
		// Potential errors here...
		// INVALID_ENUM
		// INVALID_OPERATION
		// INVALID_VALUE

		OSG_WARN << "Error in PathCommands::compileGLObjects." << std::endl;
	}
	*/

	_setParameters(ext);
	_computeBoundingBoxes(ext);

	// Call the base compile to setup display lists and whatnot.
	osgNVPR::Drawable::compileGLObjects(renderInfo);
}

Command PathCommands::aliasToCommand(char alias) {
	switch(alias) {
		case 'M': return MOVE_TO;
		case 'm': return RELATIVE_MOVE_TO;
		case 'Z': return CLOSE_PATH;
		case 'z': return CLOSE_PATH;
		case 'L': return LINE_TO;
		case 'l': return RELATIVE_LINE_TO;
		case 'H': return HORIZONTAL_LINE_TO;
		case 'h': return RELATIVE_HORIZONTAL_LINE_TO;
		case 'V': return VERTICAL_LINE_TO;
		case 'v': return RELATIVE_VERTICAL_LINE_TO;
		case 'Q': return QUADRATIC_CURVE_TO;
		case 'q': return RELATIVE_QUADRATIC_CURVE_TO;
		case 'C': return CUBIC_CURVE_TO;
		case 'c': return RELATIVE_CUBIC_CURVE_TO;
		case 'T': return SMOOTH_QUADRATIC_CURVE_TO;
		case 't': return RELATIVE_SMOOTH_QUADRATIC_CURVE_TO;
		case 'S': return SMOOTH_CUBIC_CURVE_TO;
		case 's': return RELATIVE_SMOOTH_CUBIC_CURVE_TO;
		case 'A': return ARC_TO;
		case 'a': return RELATIVE_ARC_TO;
		default:  return CLOSE_PATH;
	}
}

unsigned int PathCommands::getCommandArgumentSize(Command command) {
	switch(command) {
		case HORIZONTAL_LINE_TO:
		case RELATIVE_HORIZONTAL_LINE_TO:
		case VERTICAL_LINE_TO:
		case RELATIVE_VERTICAL_LINE_TO:
			return 1;

		case MOVE_TO:
		case RELATIVE_MOVE_TO:
		case LINE_TO:
		case RELATIVE_LINE_TO:
		case SMOOTH_QUADRATIC_CURVE_TO:
		case RELATIVE_SMOOTH_QUADRATIC_CURVE_TO:
			return 2;

		case QUADRATIC_CURVE_TO:
		case RELATIVE_QUADRATIC_CURVE_TO:
		case SMOOTH_CUBIC_CURVE_TO:
		case RELATIVE_SMOOTH_CUBIC_CURVE_TO:
			return 4;

		case SMALL_CCW_ARC_TO:
		case RELATIVE_SMALL_CCW_ARC_TO:
		case SMALL_CW_ARC_TO:
		case RELATIVE_SMALL_CW_ARC_TO:
		case LARGE_CCW_ARC_TO:
		case RELATIVE_LARGE_CCW_ARC_TO:
		case LARGE_CW_ARC_TO:
		case RELATIVE_LARGE_CW_ARC_TO:
		case CIRCULAR_CCW_ARC_TO:
		case CIRCULAR_CW_ARC_TO:
		case CIRCULAR_TANGENT_ARC_TO:
			return 5;

		case CUBIC_CURVE_TO:
		case RELATIVE_CUBIC_CURVE_TO:
			return 6;

		case ARC_TO:
		case RELATIVE_ARC_TO:
			return 7;

		default: return 0;
	}
}

bool PathCommands::_append(Command command, va_list& vl) {
	unsigned int numArgs = getCommandArgumentSize(command);

	typedef std::list<GLfloat> Args;

	Args args;

	for(unsigned int i = 0; i < numArgs; i++) {
		Args::value_type f = static_cast<Args::value_type>(va_arg(vl, double));

		args.push_back(f);
	}

	if(args.size() != numArgs) return false;

	_coords->insert(_coords->end(), args.begin(), args.end());

	_commands->push_back(command);

	return true;
}

}

