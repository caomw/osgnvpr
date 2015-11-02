#include <osg/ArgumentParser>
#include <osg/PositionAttitudeTransform>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgNVPR/Text>
#include <osgNVPR/PathCommands>
#include <osgNVPR/StateAttributes>
#include <osgNVPR/Util>
#include <osgNVPR/io_utils>

const std::string& jabberwocky(
"`Twas brillig, and the slithy toves\n"
"  Did gyre and gimble in the wabe:\n"
"All mimsy were the borogoves,\n"
"  And the mome raths outgrabe.\n"
"\n"
"\"Beware the Jabberwock, my son!\n"
"  The jaws that bite, the claws that catch!\n"
"Beware the Jubjub bird, and shun\n"
"  The frumious Bandersnatch!\"\n"
"He took his vorpal sword in hand:\n"
"  Long time the manxome foe he sought --\n"
"So rested he by the Tumtum tree,\n"
"  And stood awhile in thought.\n"
"And, as in uffish thought he stood,\n"
"  The Jabberwock, with eyes of flame,\n"
"Came whiffling through the tulgey wood,\n"
"  And burbled as it came!\n"
"One, two! One, two! And through and through\n"
"  The vorpal blade went snicker-snack!\n"
"He left it dead, and with its head\n"
"  He went galumphing back.\n"
"\"And, has thou slain the Jabberwock?\n"
"  Come to my arms, my beamish boy!\n"
"O frabjous day! Callooh! Callay!'\n"
"  He chortled in his joy.\n"
"\n"
"`Twas brillig, and the slithy toves\n"
"  Did gyre and gimble in the wabe;\n"
"All mimsy were the borogoves,\n"
"  And the mome raths outgrabe.\n"
);

// const std::string jabberwocky("HELLO WORLD");

int main(int argc, char** argv) {
	osg::ArgumentParser args(&argc, argv);

	osg::DisplaySettings::instance()->setMinimumNumStencilBits(4);
	osg::DisplaySettings::instance()->setNumMultiSamples(4);

	// TODO: Talk long and thoroughly about Text. :)
	osg::Geode*    geode     = osgNVPR::createStencilGeode();
	osgNVPR::Font* font      = new osgNVPR::Font("Source Code Pro");
	// osgNVPR::Path* fontStyle = new osgNVPR::PathCommands();
	osgNVPR::Text* text      = new osgNVPR::Text();

	// fontStyle->getParameters().strokeWidth.set(font->getScale() * 0.075f);

	// font->setPathTemplate(fontStyle);
	// font->setStyle(osgNVPR::Font::BOLD);

	text->setText(osgNVPR::String(jabberwocky));
	text->setFont(font);
	text->setLayout(new osgNVPR::NewlineLayout());
	text->setFillPaint(new osgNVPR::SolidPaint(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	// text->setStrokePaint(new osgNVPR::SolidPaint(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	// text->setPaintMethod(osgNVPR::Drawable::STROKE_THEN_FILL);
	text->setPaintMethod(osgNVPR::Drawable::FILL_ONLY);

	geode->addDrawable(text);

	osgViewer::Viewer viewer;

	unsigned int width  = 500;
	unsigned int height = 500;

	viewer.setUpViewInWindow(50, 50, width, height);
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.addEventHandler(new osgGA::StateSetManipulator(
		viewer.getCamera()->getOrCreateStateSet()
	));

	// osg::PositionAttitudeTransform* pat = new osg::PositionAttitudeTransform();
	// pat->setScale(osg::Vec3(0.1f, 0.1f, 1.0f));
	// pat->addChild(geode);

	viewer.setSceneData(geode);
	viewer.realize();
	viewer.frame();

	bool perspective = false;

	while(args.read("--perspective")) perspective = true;

	if(!perspective) {
		float                        w      = static_cast<float>(width);
		float                        h      = static_cast<float>(height);
		const osg::BoundingBox&      bb     = text->getBound();
		osg::Camera*                 camera = viewer.getCamera();
		osg::BoundingBox::value_type tw     = bb.xMax() - bb.xMin();

		// OSG_WARN << bb << std::endl;

		camera->setProjectionMatrix(osg::Matrix::ortho2D(0.0f, tw, 0.0f, tw / (w / h)));
		camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		camera->setViewMatrix(osg::Matrix::identity());
		camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		camera->setClearStencil(0);
		
		osgNVPR::OrthographicCameraManipulator* ocm = new osgNVPR::OrthographicCameraManipulator();

		ocm->setTranslateScale(osg::Vec3(14.0f, 14.0f, 1.0f));

		viewer.setCameraManipulator(ocm);

		geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	}

	else {
		geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
		
		osg::Camera* camera = viewer.getCamera();

		camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera->setClearStencil(0);

		geode->getOrCreateStateSet()->setAttribute(new osgNVPR::StencilDepthOffset());
		geode->getOrCreateStateSet()->setAttribute(new osgNVPR::CoverDepthFunc());
	}

	return viewer.run();
}

