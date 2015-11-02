PROJECT(osgnvprviewer)

LINK_LIBRARIES(osgNVPR
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
	debug     osgGA${CMAKE_DEBUG_POSTFIX}
	optimized osgGA
)

ADD_EXECUTABLE(osgnvprviewer osgnvprviewer.cpp)

INSTALL(TARGETS osgnvprviewer DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgnvprviewer PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

