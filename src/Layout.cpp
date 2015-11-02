#include <osgNVPR/Layout>

namespace osgNVPR {

NewlineLayout::NewlineLayout():
_newline('\n') {
}

osg::Vec2Array* NewlineLayout::layout(
	osgNVPR::Extensions* ext,
	const Font*          font,
	const String&        string
) const {
	Lines lines;

	computeLines(string, _newline, lines);

	osg::Vec2Array* translations = new osg::Vec2Array();

	osg::Vec2Array::value_type::value_type y = 0.0f;

	// Setup our translations, one per line.
	for(Lines::const_iterator line = lines.begin(); line != lines.end(); line++) {
		GLfloat* t = font->getTranslations(ext, string, line->first, line->second);

		for(unsigned int i = 0; i < line->second; i++) translations->push_back(osg::Vec2(t[i], y));

		y -= font->getScale();
	}

	return translations;
}

void NewlineLayout::computeLines(const String& string, Character newline, Lines& lines) {
	unsigned int offset = 0;
	unsigned int length = 0;

	for(String::const_iterator c = string.begin(); c != string.end(); c++) {
		const Character ch = *c;

		length++;

		if(ch == newline) {
			lines.push_back(Line(offset, length));

			offset += length;
			length  = 0;
		}
	}

	unsigned int size = static_cast<unsigned int>(string.size());

	if(offset < size) lines.push_back(Line(offset, size - offset));
}

}

