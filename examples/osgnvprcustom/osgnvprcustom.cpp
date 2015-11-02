#include <osg/ArgumentParser>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgNVPR/PathCommands>
#include <osgNVPR/Text>
#include <osgNVPR/Transform>
#include <osgNVPR/StateAttributes>
#include <osgNVPR/Util>

osgNVPR::Transform* createPathTransform() {
	osgNVPR::PathCommands* path      = new osgNVPR::PathCommands();
	osgNVPR::Paint*        stroke    = new osgNVPR::SolidPaint(osg::Vec4(0.0f, 0.0f, 0.0f, 0.3f));
	osgNVPR::Transform*    transform = new osgNVPR::Transform();

	path->append(osgNVPR::MOVE_TO, -200.0f, -200.0f);
	path->append(osgNVPR::LINE_TO, 200.0f, -200.0f);
	path->append(osgNVPR::LINE_TO, 200.0f, 200.0f);
	path->append(osgNVPR::LINE_TO, -200.0f, 200.0f);
	path->close();
	path->setStrokePaint(stroke);
	path->setPaintMethod(osgNVPR::Drawable::STROKE_ONLY);
	path->setBoundingBoxMode(osgNVPR::Drawable::STROKE);
	path->getParameters().strokeWidth.set(50.0f);

	transform->addDrawable(path);
	transform->setAlignment(osgNVPR::Transform::CENTER_BOTTOM);

	return transform;
}

osgNVPR::String string(
	"I have eaten the plums\n"
	"that were in the icebox\n"
	"and which you were probably saving\n"
	"for later\n"
	"they were so sweet!"
);

osgNVPR::Transform* createTextTransform() {
	osgNVPR::Text*      text      = new osgNVPR::Text();
	osgNVPR::Font*      font      = new osgNVPR::Font("Hand of Sean");
	osgNVPR::Layout*    layout    = new osgNVPR::NewlineLayout();
	osgNVPR::Paint*     fill      = new osgNVPR::SolidPaint(osg::Vec4(1.0f, 1.0f, 1.0f, 0.0f));
	osgNVPR::Transform* transform = new osgNVPR::Transform();

	font->setScale(30.0f);

	text->setFont(font);
	text->setLayout(layout);
	text->setText(string);
	text->setFillPaint(fill);
	text->setPaintMethod(osgNVPR::Drawable::FILL_ONLY);

	transform->addDrawable(text);
	transform->setAlignment(osgNVPR::Transform::LEFT_BASELINE);

	return transform;
}

int main(int argc, char** argv) {
	osg::ArgumentParser args(&argc, argv);

	osg::DisplaySettings::instance()->setMinimumNumStencilBits(8);
	osg::DisplaySettings::instance()->setNumMultiSamples(8);

	osgNVPR::Transform* transform = createTextTransform();

	osgViewer::Viewer viewer;

	unsigned int width  = 512;
	unsigned int height = 512;

	viewer.setUpViewInWindow(50, 50, width, height);
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.addEventHandler(new osgGA::StateSetManipulator(
		viewer.getCamera()->getOrCreateStateSet()
	));

	viewer.setSceneData(transform);
	viewer.realize();
	viewer.frame();

	bool perspective = false;

	while(args.read("--perspective")) perspective = true;

	if(!perspective) {
		osg::Camera* camera = viewer.getCamera();

		osg::BoundingBox::value_type w  = transform->getGeode()->getBoundingBox().xMax();
		osg::BoundingBox::value_type h1 = transform->getGeode()->getBoundingBox().yMin();
		osg::BoundingBox::value_type h2 = transform->getGeode()->getBoundingBox().yMax();

		camera->setProjectionMatrix(osg::Matrix::ortho2D(0.0f, w, 0.0f, w));
		camera->setViewMatrix(osg::Matrix::identity());
		camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		camera->setClearStencil(0);

		viewer.setCameraManipulator(new osgNVPR::OrthographicCameraManipulator());

		transform->getGeode()->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	}

	else {
		// TODO: This MUTILATES the framerate...?
		// geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

		osg::Camera* camera = viewer.getCamera();

		camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera->setClearStencil(0);

		transform->setAxisAlignment(osgNVPR::Transform::XZ_PLANE);
		transform->getGeode()->getOrCreateStateSet()->setAttribute(new osgNVPR::StencilDepthOffset());
		transform->getGeode()->getOrCreateStateSet()->setAttribute(new osgNVPR::CoverDepthFunc());
	}

	return viewer.run();
}

