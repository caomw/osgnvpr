PROJECT(osgnvprshader)

LINK_LIBRARIES(osgNVPR
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
	debug     osgGA${CMAKE_DEBUG_POSTFIX}
	optimized osgGA
	debug     osgDB${CMAKE_DEBUG_POSTFIX}
	optimized osgDB
)

ADD_EXECUTABLE(osgnvprshader osgnvprshader.cpp)

INSTALL(TARGETS osgnvprshader DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgnvprshader PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

