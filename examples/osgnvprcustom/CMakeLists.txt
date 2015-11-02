PROJECT(osgnvprcustom)

LINK_LIBRARIES(osgNVPR
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
	debug     osgGA${CMAKE_DEBUG_POSTFIX}
	optimized osgGA
)

ADD_EXECUTABLE(osgnvprcustom osgnvprcustom.cpp)

INSTALL(TARGETS osgnvprcustom DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgnvprcustom PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

