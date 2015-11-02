PROJECT(osgnvprfbo)

LINK_LIBRARIES(osgNVPR
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
	debug     osgGA${CMAKE_DEBUG_POSTFIX}
	optimized osgGA
)

ADD_EXECUTABLE(osgnvprfbo osgnvprfbo.cpp)

INSTALL(TARGETS osgnvprfbo DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgnvprfbo PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

