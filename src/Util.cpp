#include <osg/Geode>
#include <osg/Stencil>
#include <osgNVPR/Util>
#include <osgNVPR/Extensions>

namespace osgNVPR {

OrthographicCameraManipulator::OrthographicCameraManipulator():
_translateScale (1.0f, 1.0f, 0.0f), 
_scale          (1.0f, 1.0f, 1.0f),
_zoomScale      (0.025f, 0.025f, 0.0f) {
}

bool OrthographicCameraManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
	osg::Vec3 pos(ea.getX(), ea.getY(), 0.0f);

	osgGA::GUIEventAdapter::EventType event = ea.getEventType();

	// If it's just MOVE, we still need to update lastPos.
	if(event == osgGA::GUIEventAdapter::MOVE) _lastPos = pos;

	// During any DRAG we translate.
	else if(event == osgGA::GUIEventAdapter::DRAG) {
		osg::Vec3 diff = pos - _lastPos;

		// _translate += osg::Vec3(-diff.x() * 14.0f, -diff.y() * 14.0f, 0.0f);
		// _translate += osg::Vec3(-diff.x(), -diff.y(), 0.0f);
		_translate -= osg::componentMultiply(diff, _translateScale);

		_matrix  = osg::Matrix::translate(_translate) * osg::Matrix::scale(_scale);
		_lastPos = pos;
	}
	
	// Mouse SCROLLing up/down we scale.
	else if(event == osgGA::GUIEventAdapter::SCROLL) {
		osgGA::GUIEventAdapter::ScrollingMotion scroll = ea.getScrollingMotion();
		osg::Vec3::value_type                   s      = 0.025f;

		if(scroll == osgGA::GUIEventAdapter::SCROLL_DOWN) _scale += osg::Vec3(s, s, 1.0f);
			
		else _scale += osg::Vec3(-s, -s, 1.0f);
		
		_matrix = osg::Matrix::translate(_translate) * osg::Matrix::scale(_scale);
	}

	else return false;

	return true;
}

// TODO: Make these parameters.
osg::Geode* createStencilGeode() {
	osg::Geode*   geode   = new osg::Geode();
	osg::Stencil* stencil = new osg::Stencil();

	stencil->setWriteMask(~0);
	stencil->setFunction(osg::Stencil::NOTEQUAL, 0, 0xFF);
	stencil->setOperation(osg::Stencil::KEEP, osg::Stencil::KEEP, osg::Stencil::ZERO);

	osg::StateSet* state = geode->getOrCreateStateSet();
	
	state->setAttributeAndModes(stencil, osg::StateAttribute::ON);

	return geode;
}

}

