#include <osg/ArgumentParser>
#include <osg/PositionAttitudeTransform>
#include <osgGA/StateSetManipulator>
#include <osgDB/FileUtils>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgNVPR/PathCommands>
#include <osgNVPR/PathString>
#include <osgNVPR/StateAttributes>
#include <osgNVPR/Util>

osg::Program* createProgram(const std::string& fp) {
	osgDB::FilePathList& paths = osgDB::getDataFilePathList();
	
	paths.push_back("../examples/osgnvprshader");

	std::string path = osgDB::findDataFile(fp);

	if(!path.size()) return 0;

	osg::Program* program = new osg::Program();
	osg::Shader*  frag    = osg::Shader::readShaderFile(osg::Shader::FRAGMENT, path);

	program->addShader(frag);

	return program;
}

int main(int argc, char** argv) {
	osg::ArgumentParser args(&argc, argv);

	unsigned int width  = 640;
	unsigned int height = 480;

	osg::DisplaySettings::instance()->setMinimumNumStencilBits(4);
	osg::DisplaySettings::instance()->setNumMultiSamples(4);

	osg::Geode*            geode  = osgNVPR::createStencilGeode();
	osgNVPR::PathCommands* path   = new osgNVPR::PathCommands();
	osgNVPR::Paint*        fill   = new osgNVPR::ShaderPaint(createProgram("osgnvprshader-fill.glsl"));
	osgNVPR::Paint*        stroke = new osgNVPR::ShaderPaint(createProgram("osgnvprshader-stroke.glsl"));

	GLfloat hs = width / 4.0f;
	GLfloat vs = height / 4.0f;

	path->append(osgNVPR::MOVE_TO, hs, vs);
	path->append(osgNVPR::LINE_TO, hs * 3.0f, vs);
	path->append(osgNVPR::LINE_TO, hs * 3.0f, vs * 3.0f);
	path->append(osgNVPR::LINE_TO, hs, vs * 3.0f);
	path->close();
	path->setFillPaint(fill);
	path->setStrokePaint(stroke);
	path->setPaintMethod(osgNVPR::Drawable::FILL_THEN_STROKE);
	path->getParameters().strokeWidth.set(20.0f);

	geode->addDrawable(path);

	osgViewer::Viewer viewer;

	viewer.setUpViewInWindow(50, 50, width, height);
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.addEventHandler(new osgGA::StateSetManipulator(
		viewer.getCamera()->getOrCreateStateSet()
	));

	bool perspective = false;

	while(args.read("--perspective")) perspective = true;

	if(!perspective) {
		osg::Camera* camera = viewer.getCamera();

		camera->setProjectionMatrix(osg::Matrix::ortho2D(0.0f, width, 0.0f, height));
		camera->setViewMatrix(osg::Matrix::identity());
		camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		camera->setClearStencil(0);
		
		viewer.setCameraManipulator(new osgNVPR::OrthographicCameraManipulator());
		viewer.setSceneData(geode);

		geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	}

	else {
		osg::PositionAttitudeTransform* pat = new osg::PositionAttitudeTransform();

		pat->addChild(geode);
		pat->setAttitude(osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3(1.0f, 0.0f, 0.0f)));

		osg::Camera* camera = viewer.getCamera();

		camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera->setClearStencil(0);

		geode->getOrCreateStateSet()->setAttribute(new osgNVPR::StencilDepthOffset());
		geode->getOrCreateStateSet()->setAttribute(new osgNVPR::CoverDepthFunc());

		viewer.setSceneData(pat);
	}

	return viewer.run();
}

