#include <osg/ArgumentParser>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgNVPR/PathCommands>
#include <osgNVPR/StateAttributes>
#include <osgNVPR/Util>

osg::Texture2D* createFBOTexture(unsigned int width, unsigned int height) {
	osg::Texture2D* texture = new osg::Texture2D();

	texture->setTextureSize(width, height);
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER);
	texture->setInternalFormat(GL_RGBA);
	texture->setBorderColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.f));

	return texture;
}

osg::Camera* createFBOCamera(osg::Texture2D* texture) {
	osg::Camera* camera = new osg::Camera();

	int width  = texture->getTextureWidth();
	int height = texture->getTextureHeight();

	// camera->setNodeMask(0x00000001);
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	camera->setRenderOrder(osg::Camera::PRE_RENDER);
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0.0f, width, 0.0f, height));
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera->setClearStencil(0);
	camera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	camera->attach(osg::Camera::PACKED_DEPTH_STENCIL_BUFFER, GL_DEPTH_STENCIL_EXT);
	camera->attach(osg::Camera::COLOR_BUFFER, texture);

	osg::Geode*            geode   = osgNVPR::createStencilGeode();
	osgNVPR::PathCommands* path    = new osgNVPR::PathCommands();
	osgNVPR::Paint*        fill    = new osgNVPR::SolidPaint(osg::Vec4(1.0f, 0.5f, 0.0f, 1.0f));
	osgNVPR::Paint*        stroke  = new osgNVPR::SolidPaint(osg::Vec4(0.5f, 0.5f, 1.0f, 1.0f));

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

	camera->addChild(geode);

	return camera;
}

const unsigned int width  = 512;
const unsigned int height = 512;

int main(int argc, char** argv) {
	osg::ArgumentParser args(&argc, argv);

	osg::DisplaySettings::instance()->setMinimumNumStencilBits(4);
	osg::DisplaySettings::instance()->setNumMultiSamples(4);

	osg::Texture2D* texture  = createFBOTexture(width, height);
	osg::Camera*    camera   = createFBOCamera(texture);
	osg::Geode*     geode    = new osg::Geode();
	osg::Group*     group    = new osg::Group();
	osg::Geometry*  geometry = osg::createTexturedQuadGeometry(
		osg::Vec3(0.0f, 0.0f, 0.0f),
		osg::Vec3(width, 0.0f, 0.0f),
		osg::Vec3(0.0f, 0.0f, height)
	);

	geode->addDrawable(geometry);

	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture);
	geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	geode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	osgViewer::Viewer viewer;

	group->addChild(camera);
	group->addChild(geode);

	viewer.setSceneData(group);
	// viewer.getCamera()->setCullMask(0x00000001);
	viewer.setUpViewInWindow(50, 50, width, height);
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.addEventHandler(new osgGA::StateSetManipulator(
		viewer.getCamera()->getOrCreateStateSet()
	));

	return viewer.run();
}

