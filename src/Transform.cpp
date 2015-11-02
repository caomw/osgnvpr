#include <osgNVPR/Transform>
#include <osgNVPR/Util>
#include <osgNVPR/Text>

namespace osgNVPR {

Transform::Transform():
_alignment      (USER_ALIGNMENT),
_axisAlignment  (USER_AXIS_ALIGNMENT),
_dirtyTransform (true) {
	_geode = createStencilGeode();

	addChild(_geode.get());
}

void Transform::traverse(osg::NodeVisitor& nv) {
	// If we aren't dirty, we don't need to do any processing.
	if(!_dirtyTransform) {
		osg::MatrixTransform::traverse(nv);

		return;
	}

	for(unsigned int i = 0; i < _geode->getNumDrawables(); i++) {
		Drawable* drawable = dynamic_cast<Drawable*>(_geode->getDrawable(i));

		if(!drawable) continue;

		// We can't have any uncompiled Drawables...
		if(!drawable->isCompiled()) {
			osg::MatrixTransform::traverse(nv);

			return;
		}
	}

	osg::Matrix matrix = getMatrix();

	// Our Geode could have ANYTHING in it, but we only really care if the user has
	// requested BASELINE Alignment; only Text has a baseline, so we need to check for
	// that and fail out if necessary.
	osg::Vec3 baselineOffset;

	if(
		_alignment == LEFT_BASELINE ||
		_alignment == CENTER_BASELINE ||
		_alignment == RIGHT_BASELINE
	) {
		for(unsigned int i = 0; i < _geode->getNumDrawables(); i++) {
			Text* text = dynamic_cast<Text*>(_geode->getDrawable(i));

			if(!text) {
				OSG_WARN
					<< "Attempted to use BASELINE Alignment on a Transform that contains "
					<< "Drawables other than Text; this is not supported!"
					<< std::endl
				;

				return;
			}

			osg::Vec3::value_type baseline = text->getBaseline();

			if(baseline < baselineOffset.y()) baselineOffset.set(0.0f, baseline, 0.0f);
		}
	}

	const osg::BoundingBox& bb = _geode->getBoundingBox();

	osg::Vec3 offset;

	switch(_alignment) {
		case LEFT_TOP:
			offset.set(bb.xMin(), bb.yMax(), bb.zMin());

			break;

		case LEFT_CENTER:
			offset.set(bb.xMin(), (bb.yMax() + bb.yMin()) * 0.5f, bb.zMin());

			break;

		case LEFT_BOTTOM:
			offset.set(bb.xMin(), bb.yMin(),bb.zMin());

			break;

		case CENTER_TOP:
			offset.set((bb.xMax() + bb.xMin()) * 0.5f, bb.yMax(), bb.zMin());

			break;

		case CENTER_CENTER:
			offset.set((bb.xMax() + bb.xMin()) * 0.5f, (bb.yMax() + bb.yMin()) * 0.5f, bb.zMin());

			break;

		case CENTER_BOTTOM:
			offset.set((bb.xMax() + bb.xMin()) * 0.5f, bb.yMin(), bb.zMin());

			break;

		case RIGHT_TOP:
			offset.set(bb.xMax(), bb.yMax(), bb.zMin());

			break;

		case RIGHT_CENTER:
			offset.set(bb.xMax(), (bb.yMax() + bb.yMin()) * 0.5f, bb.zMin());

			break;

		case RIGHT_BOTTOM:
			offset.set(bb.xMax(), bb.yMin(), bb.zMin());

			break;

		case LEFT_BASELINE:
			offset = baselineOffset;

			break;

		case CENTER_BASELINE:
			offset = baselineOffset + osg::Vec3(bb.xMax() * 0.5f, 0.0f, 0.0f);

			break;

		case RIGHT_BASELINE:
			offset = baselineOffset + osg::Vec3(bb.xMax(), 0.0f, 0.0f);

			break;

		default:
			break;
	}

	matrix.preMult(osg::Matrix::translate(_position - offset));

	osg::Quat rotation;

	switch(_axisAlignment) {
		case XZ_PLANE:
			rotation = osg::Quat(osg::inDegrees(90.0f), osg::Vec3(1.0f, 0.0f, 0.0f));

			break;

		case REVERSED_XZ_PLANE:
			rotation =
				osg::Quat(osg::inDegrees(180.0f), osg::Vec3(0.0f, 1.0f, 0.0f)) *
				osg::Quat(osg::inDegrees(90.0f), osg::Vec3(1.0f, 0.0f, 0.0f))
			;

			break;

		case YZ_PLANE:
			rotation =
				osg::Quat(osg::inDegrees(90.0f), osg::Vec3(1.0f, 0.0f, 0.0f)) *
				osg::Quat(osg::inDegrees(90.0f), osg::Vec3(0.0f, 0.0f, 1.0f))
			;

			break;

		case REVERSED_YZ_PLANE:
			rotation =
				osg::Quat(osg::inDegrees(180.0f), osg::Vec3(0.0f, 1.0f, 0.0f)) *
				osg::Quat(osg::inDegrees(90.0f), osg::Vec3(1.0f, 0.0f, 0.0f)) *
				osg::Quat(osg::inDegrees(90.0f), osg::Vec3(0.0f, 0.0f, 1.0f))
			;

			break;

		case REVERSED_XY_PLANE:
			rotation = osg::Quat(osg::inDegrees(180.0f), osg::Vec3(0.0f, 1.0f, 0.0f));

			break;

		default:
			break;
	}

	matrix.postMult(osg::Matrix::rotate(rotation));

	setMatrix(matrix);

	_dirtyTransform = false;

	osg::MatrixTransform::traverse(nv);
}

void Transform::addDrawable(Drawable* drawable) {
	_geode->addDrawable(drawable);

	_dirtyTransform = true;
}

void Transform::setAlignment(Alignment alignment) {
	_alignment = alignment;
	_dirtyTransform = true;
}

void Transform::setAxisAlignment(AxisAlignment axisAlignment) {
	_axisAlignment = axisAlignment;
	_dirtyTransform = true;
}

void Transform::setPosition(const osg::Vec3& position) {
	_position = position;
	_dirtyTransform = true;
}

}

