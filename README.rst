########
osgNVPR
########

osgNVPR is an OSG nodekit that provides the middleware for using the
NV_path_render extension (by NVidia) to facilitate easy vector-based drawing
(efficiently) in OpenSceneGraph.

TODO
====

- Vastly improve this README; **fuck**!

- I believe there is a bug when you generate a range of paths from glyph/font
  data.  For characters who do not exist in the font, there is no path data.
  However, glIsPathNV() still returns TRUE when you test that particular path,
  so that subsequent calls to glGetPathParameterNV will cause OpenGL errors.
  This only seems to be an issue when using GL_SKIP_MISSING_GLYPH_NV.
