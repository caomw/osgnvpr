#include <osg/GL2Extensions>
#include <osgNVPR/Paint>

namespace osgNVPR {

SolidPaint::SolidPaint(const osg::Vec4& color):
_color(color) {
}

void SolidPaint::paint(osgNVPR::Extensions* ext, osg::State& state) const {
	state.Color(_color.r(), _color.b(), _color.g(), _color.a());
}

ShaderPaint::ShaderPaint(osg::Program* program) {
	_stateSet = new osg::StateSet();

	setProgram(program);
}

void ShaderPaint::prePaint(osgNVPR::Extensions* ext, osg::State& state) const {
	state.pushStateSet(_stateSet.get());
	state.apply();
}

void ShaderPaint::paint(osgNVPR::Extensions* ext, osg::State& state) const {
	state.apply(_stateSet.get());
}

void ShaderPaint::postPaint(osgNVPR::Extensions* ext, osg::State& state) const {
	state.popStateSet();
	state.apply();
}

void ShaderPaint::setProgram(osg::Program* program) {
	if(_program.valid() && !program) _stateSet->setAttributeAndModes(
		_program.get(),
		osg::StateAttribute::OFF
	);

	_program = program;

	_stateSet->setAttributeAndModes(_program.get());
}

}

