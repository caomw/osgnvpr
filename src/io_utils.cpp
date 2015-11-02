#include <osgNVPR/io_utils>

std::ostream& operator<<(std::ostream& output, const osgNVPR::Font::FontMetrics& metrics) {
	output
		<< "xMinBounds         = " << metrics.xMinBounds << std::endl
		<< "yMinBounds         = " << metrics.yMinBounds << std::endl
		<< "xMaxBounds         = " << metrics.xMaxBounds << std::endl
		<< "yMaxBounds         = " << metrics.yMaxBounds << std::endl
		<< "unitsPerEm         = " << metrics.unitsPerEm << std::endl
		<< "ascender           = " << metrics.ascender << std::endl
		<< "descender          = " << metrics.descender << std::endl
		<< "height             = " << metrics.height << std::endl
		<< "maxAdvanceWidth    = " << metrics.maxAdvanceWidth << std::endl
		<< "maxAdvanceHeight   = " << metrics.maxAdvanceHeight << std::endl
		<< "underlinePosition  = " << metrics.underlinePosition << std::endl
		<< "underlineThickness = " << metrics.underlineThickness << std::endl
		<< "hasKerning         = " << metrics.hasKerning
	;

	return output;
}

std::ostream& operator<<(std::ostream& output, const osgNVPR::Font::GlyphMetrics::GlyphData& glyphData) {
	output
		<< "width                    = " << glyphData.width << std::endl
		<< "height                   = " << glyphData.height << std::endl
		<< "horizontalBearingX       = " << glyphData.horizontalBearingX << std::endl
		<< "horizontalBearingY       = " << glyphData.horizontalBearingY << std::endl
		<< "horizontalBearingAdvance = " << glyphData.horizontalBearingAdvance << std::endl
		<< "verticalBearingX         = " << glyphData.verticalBearingX << std::endl
		<< "verticalBearingY         = " << glyphData.verticalBearingY << std::endl
		<< "verticalBearingAdvance   = " << glyphData.verticalBearingAdvance << std::endl
		<< "hasKerning               = " << glyphData.hasKerning << std::endl
	;

	return output;
}

std::ostream& operator<<(std::ostream& output, const osg::BoundingBox& boundingBox) {
	output
		<< "xMin = " << boundingBox.xMin() << ", "
		<< "yMin = " << boundingBox.yMin() << ", "
		<< "xMax = " << boundingBox.xMax() << ", "
		<< "yMax = " << boundingBox.yMax()
	;

	return output;
}

