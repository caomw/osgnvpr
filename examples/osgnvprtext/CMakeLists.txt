PROJECT(osgnvprtext)

LINK_LIBRARIES(osgNVPR
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
	debug     osgGA${CMAKE_DEBUG_POSTFIX}
	optimized osgGA
)

ADD_EXECUTABLE(osgnvprtext osgnvprtext.cpp)

INSTALL(TARGETS osgnvprtext DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgnvprtext PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

