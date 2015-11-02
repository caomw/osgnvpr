PROJECT(osgnvpreffectcompositor)

LINK_LIBRARIES(osgNVPR
	EffectCompositor
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
	debug     osgGA${CMAKE_DEBUG_POSTFIX}
	optimized osgGA
	debug     osgDB${CMAKE_DEBUG_POSTFIX}
	optimized osgDB
)

ADD_EXECUTABLE(osgnvpreffectcompositor osgnvpreffectcompositor.cpp)

INSTALL(TARGETS osgnvpreffectcompositor DESTINATION ${CMAKE_INSTALL_PREFIX}/share/OpenSceneGraph/bin)
SET_TARGET_PROPERTIES(osgnvpreffectcompositor PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

