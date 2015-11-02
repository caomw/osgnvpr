#include <osgNVPR/StateAttributes>
#include <osgNVPR/Extensions>

namespace osgNVPR {

StencilFunc::StencilFunc(Function function, GLint ref, GLuint mask):
_func (function),
_ref  (ref),
_mask (mask) {
}

StencilFunc::StencilFunc(const StencilFunc& attr, const osg::CopyOp& copyOp):
osg::StateAttribute(attr, copyOp),
_func (attr._func),
_ref  (attr._ref),
_mask (attr._mask) {
}

int StencilFunc::compare(const osg::StateAttribute& attr) const {
	COMPARE_StateAttribute_Types(StencilFunc, attr)
	COMPARE_StateAttribute_Parameter(_func)
	COMPARE_StateAttribute_Parameter(_ref)
	COMPARE_StateAttribute_Parameter(_mask)

	return 0;
}

void StencilFunc::apply(osg::State& state) const {
	osgNVPR::Extensions* ext = osgNVPR::getNVPRExtensions(&state);

	if(ext) ext->glPathStencilFuncNV(_func, _ref, _mask);
}

StencilDepthOffset::StencilDepthOffset(GLfloat factor, GLint units):
_factor (factor),
_units  (units) {
}

StencilDepthOffset::StencilDepthOffset(const StencilDepthOffset& attr, const osg::CopyOp& copyOp):
osg::StateAttribute(attr, copyOp),
_factor (attr._factor),
_units  (attr._units) {
}

int StencilDepthOffset::compare(const osg::StateAttribute& attr) const {
	COMPARE_StateAttribute_Types(StencilDepthOffset, attr)
	COMPARE_StateAttribute_Parameter(_factor)
	COMPARE_StateAttribute_Parameter(_units)

	return 0;
}

void StencilDepthOffset::apply(osg::State& state) const {
	osgNVPR::Extensions* ext = osgNVPR::getNVPRExtensions(&state);

	if(ext) ext->glPathStencilDepthOffsetNV(_factor, _units);
}

CoverDepthFunc::CoverDepthFunc(Function function):
_zfunc(function) {
}

CoverDepthFunc::CoverDepthFunc(const CoverDepthFunc& attr, const osg::CopyOp& copyOp):
osg::StateAttribute(attr, copyOp),
_zfunc (attr._zfunc) {
}

int CoverDepthFunc::compare(const osg::StateAttribute& attr) const {
	COMPARE_StateAttribute_Types(CoverDepthFunc, attr)
	COMPARE_StateAttribute_Parameter(_zfunc)

	return 0;
}

void CoverDepthFunc::apply(osg::State& state) const {
	osgNVPR::Extensions* ext = osgNVPR::getNVPRExtensions(&state);

	if(ext) ext->glPathCoverDepthFuncNV(_zfunc);
}

}

