#include <osgGA/StateSetManipulator>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgNVPR/PathCommands>
#include <osgNVPR/StateAttributes>
#include <osgNVPR/Util>

#include "EffectCompositor"

osg::Geode* createNVPRQuad(const osg::Vec4& color, bool b = false) {
	osg::Geode*            geode = osgNVPR::createStencilGeode();
	osgNVPR::PathCommands* path  = new osgNVPR::PathCommands();
	osgNVPR::Paint*        paint = new osgNVPR::SolidPaint(color);

	float s = 20.0f;

	if(b) s = 10.0f;

	path->append(osgNVPR::MOVE_TO, -s, -s);
	path->append(osgNVPR::LINE_TO, s, -s);
	path->append(osgNVPR::LINE_TO, s, s);
	path->append(osgNVPR::LINE_TO, -s, s);
	path->close();
	path->setFillPaint(paint);
	path->setPaintMethod(osgNVPR::Drawable::FILL_ONLY);

	geode->addDrawable(path);

	return geode;
}

const unsigned int width  = 512;
const unsigned int height = 512;

const char* XML_PATH = "../examples/osgnvpreffectcompositor/osgnvpreffectcompositor.xml";

int main(int argc, char** argv) {
	osg::DisplaySettings::instance()->setMinimumNumStencilBits(4);
	osg::DisplaySettings::instance()->setNumMultiSamples(4);

	osgViewer::Viewer viewer;

	// osg::Geode*              quad       = createNVPRQuad(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f), true);
	// osg::Geode*              blur       = createNVPRQuad(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	osg::Node* quad = osgDB::readNodeFile("cow.osg");
	osg::Node* blur = osgDB::readNodeFile("cow-left.osg");
	osgFX::EffectCompositor* compositor = osgFX::readEffectFile(XML_PATH);

	compositor->addChild(blur);

	/*
	osg::Texture* texture = compositor->getTexture("scene-blur-y");

	if(!texture) return 1;

	osg::Geode*    geode    = new osg::Geode();
	osg::Geometry* geometry = osg::createTexturedQuadGeometry(
		osg::Vec3(0.0f, 0.0f, 0.0f),
		osg::Vec3(0.0f, 1.0f, 0.0f),
		osg::Vec3(0.0f, 0.0f, 1.0f)
	);

	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture);
	geode->addDrawable(geometry);
	*/

	/*
	osgFX::EffectCompositor::PassData pd;

	compositor->getPassData("scene-blur-y", pd);

	OSG_WARN << pd.pass << std::endl;
	*/

	osg::Group* g = new osg::Group();

	g->addChild(compositor);
	g->addChild(quad);

	viewer.getCamera()->setClearColor(osg::Vec4(1.0f, 0.5f, 0.1f, 1.0f));

	viewer.setSceneData(g);
	viewer.setUpViewInWindow(50, 50, width, height);
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.addEventHandler(new osgGA::StateSetManipulator(
		viewer.getCamera()->getOrCreateStateSet()
	));

	return viewer.run();
}

