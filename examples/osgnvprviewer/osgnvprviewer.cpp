#include "osgnvprviewer-dragon.h"

#include <osg/ArgumentParser>
#include <osg/PositionAttitudeTransform>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgNVPR/PathCommands>
#include <osgNVPR/PathString>
#include <osgNVPR/StateAttributes>
#include <osgNVPR/Util>

int main(int argc, char** argv) {
	osg::ArgumentParser args(&argc, argv);

	// These are the settings that will determine our level of rasterization quality.
	osg::DisplaySettings::instance()->setMinimumNumStencilBits(4);
	osg::DisplaySettings::instance()->setNumMultiSamples(4);

	osg::Geode* geode = osgNVPR::createStencilGeode();

	// Create our first Path objecst using raw drawing commands; like drawing with Cairo.
	for(unsigned int i = 0; i < 5; i++) {
		float s = static_cast<float>(i) / 5.0f;

		osgNVPR::PathCommands* path   = new osgNVPR::PathCommands();
		osgNVPR::Paint*        fill   = new osgNVPR::SolidPaint(osg::Vec4(s, 1.0f, 1.0f, 1.0f));
		osgNVPR::Paint*        stroke = new osgNVPR::SolidPaint(osg::Vec4(1.0f, s, 1.0f, 1.0f));

		float cx = (i * 100.0f) + 50.0f;

		path->append(osgNVPR::MOVE_TO, cx, 0.0f);
		path->append(osgNVPR::LINE_TO, cx - 50.0f, 50.0f);
		path->append(osgNVPR::LINE_TO, cx, 100.0f);
		path->append(osgNVPR::LINE_TO, cx + 50.0f, 50.0f);
		path->close();
		path->setFillPaint(fill);
		path->setStrokePaint(stroke);
		path->setPaintMethod(osgNVPR::Drawable::FILL_THEN_STROKE);
		path->getParameters().strokeWidth.set(20.0f);

		geode->addDrawable(path);
	}

	// Create our second Path object using the SVG string data from the NVidia Path
	// Rendering SDK. An interesting exercise to the reader could be to optimize this
	// code by putting all of the same SVG paths with the same paint into a single
	// path object. This wouldn't preserve the order of drawing, however, so whether or
	// not this works for YOUR SVG images is variable. :)
	for(unsigned int i = 0; i < dragon_paths; i++) {
		osgNVPR::PathString* path  = new osgNVPR::PathString(osgNVPR::PathString::FORMAT_SVG);
		osgNVPR::Paint*      paint = new osgNVPR::SolidPaint(osg::Vec4(
			welsh_dragon_colors[i][0],
			welsh_dragon_colors[i][1],
			welsh_dragon_colors[i][2],
			0.2f
		));

		path->append(welsh_dragon[i]);
		path->setFillPaint(paint);

		geode->addDrawable(path);
	}

	osgViewer::Viewer viewer;

	unsigned int width  = 640;
	unsigned int height = 480;

	viewer.setUpViewInWindow(50, 50, width, height);
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.addEventHandler(new osgGA::StateSetManipulator(
		viewer.getCamera()->getOrCreateStateSet()
	));

	bool perspective = false;

	while(args.read("--perspective")) perspective = true;

	// Orthographic HUD view.
	if(!perspective) {
		osg::Camera* camera = viewer.getCamera();

		camera->setProjectionMatrix(osg::Matrix::ortho2D(0.0f, height, 0.0f, width));
		camera->setViewMatrix(osg::Matrix::identity());
		camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		camera->setClearStencil(0);
		
		viewer.setCameraManipulator(new osgNVPR::OrthographicCameraManipulator());
		
		geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		
		viewer.setSceneData(geode);
	}

	// TODO: FIX THIS!
	// Standard perspective view; however, we cannot calculate a BoundingBox properly
	// without a contextID, and I can't get a contextID without access to a State or
	// RenderInfo instance. This means the I don't actually ACCURATELY calculate the
	// BoundingBox until the Path has been compiled, which happens AFTER the CameraManipulator
	// caclulates its home position. :(
	else {
		// Our dragon is a bit too big for our view, and SVG's have an upper-left origin;
		// lets fix that now.
		osg::PositionAttitudeTransform* pat = new osg::PositionAttitudeTransform();

		pat->setScale(osg::Vec3(0.5f, 0.5f, 1.0f));
		pat->addChild(geode);
		pat->setAttitude(osg::Quat(-90.0f, osg::Vec3(1.0f, 0.0f, 0.0f)));

		viewer.setSceneData(pat);

		// TODO: This MUTILATES the framerate...?
		// geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
		
		osg::Camera* camera = viewer.getCamera();

		camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera->setClearStencil(0);

		geode->getOrCreateStateSet()->setAttribute(new osgNVPR::StencilDepthOffset(-1.0f));
		geode->getOrCreateStateSet()->setAttribute(new osgNVPR::CoverDepthFunc());
	}

	return viewer.run();
}

