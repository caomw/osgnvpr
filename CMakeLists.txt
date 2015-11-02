PROJECT(osgNVPR)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/build/")

# Use a debug postfix to distinguish build products. Mostly important on
# Windows, because linking a debug library into a release executable (or
# vice-versa, or just mixing C++ runtime versions) on Windows will lead
# to crashes if the libraries use the C++ runtime.
SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Add a postfix, usually d on windows")

# On GCC, we need to set these compiler flags.
IF(NOT WIN32)
	SET(CMAKE_C_FLAGS "-W -Wall -Wno-unused -fPIC")
	SET(CMAKE_CXX_FLAGS "-W -Wall -Wno-unused -fPIC")
ENDIF(NOT WIN32)

SET(OPENSCENEGRAPH_INCLUDE_DIR "" CACHE PATH "Location of the OSG include headers.")
SET(OPENSCENEGRAPH_LIB_DIR "" CACHE PATH "Location of the the OSG library objects.")

SET(SRC_FILES
	"src/io_utils.cpp"
	"src/Drawable.cpp"
	"src/Extensions.cpp"
	"src/Font.cpp"
	"src/Layout.cpp"
	"src/Paint.cpp"
	"src/Path.cpp"
	"src/PathCommands.cpp"
	"src/PathString.cpp"
	"src/StateAttributes.cpp"
	"src/Text.cpp"
	"src/Transform.cpp"
	"src/Util.cpp"
)

SET(HEADER_PATH ${osgNVPR_SOURCE_DIR}/include/osgNVPR)
SET(HEADER_FILES
	"${HEADER_PATH}/io_utils"
	"${HEADER_PATH}/Drawable"
	"${HEADER_PATH}/Export"
	"${HEADER_PATH}/Extensions"
	"${HEADER_PATH}/Font"
	"${HEADER_PATH}/Layout"
	"${HEADER_PATH}/Paint"
	"${HEADER_PATH}/Path"
	"${HEADER_PATH}/PathCommands"
	"${HEADER_PATH}/PathString"
	"${HEADER_PATH}/StateAttributes"
	"${HEADER_PATH}/Text"
	"${HEADER_PATH}/Transform"
	"${HEADER_PATH}/Util"
	"${HEADER_PATH}/Version"
)

SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})
SOURCE_GROUP("Source Files" FILES ${SRC_FILES})

INCLUDE_DIRECTORIES(
	"include"
	"ext/EffectCompositor"
	"${OPENSCENEGRAPH_INCLUDE_DIR}"
)

LINK_DIRECTORIES(${OPENSCENEGRAPH_LIB_DIR})

# Put the exectuables right in our local directory...
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# Add the EffectCompositor code from Wang Rui until it's actually added
# into OpenSceneGraph itself.
ADD_SUBDIRECTORY("ext/EffectCompositor")

ADD_LIBRARY(osgNVPR SHARED ${HEADER_FILES} ${SRC_FILES})

# Add debug postfix to OSG libraries so we link to the right ones in debug.
TARGET_LINK_LIBRARIES(osgNVPR
	debug     OpenThreads${CMAKE_DEBUG_POSTFIX}
	optimized OpenThreads
	debug     osg${CMAKE_DEBUG_POSTFIX}
	optimized osg
	debug     osgText${CMAKE_DEBUG_POSTFIX}
	optimized osgText
	debug     osgGA${CMAKE_DEBUG_POSTFIX}
	optimized osgGA
)

# The archive/runtime part is so that the lib/dll pair goes into lib and bin
# respectively on Windows.
INSTALL(TARGETS osgNVPR
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

FOREACH(INCLUDEFILE ${HEADER_FILES})
	FILE(RELATIVE_PATH REL_INCLUDEFILE ${osgNVPR_SOURCE_DIR}/include/osgNVPR ${INCLUDEFILE})
	GET_FILENAME_COMPONENT(REL_INCLUDE_PATH ${REL_INCLUDEFILE} PATH)
	INSTALL(
		FILES ${INCLUDEFILE}
		DESTINATION ${CMAKE_INSTALL_PREFIX}/include/osgNVPR/${REL_INCLUDE_PATH}
	)
ENDFOREACH(INCLUDEFILE)

ADD_SUBDIRECTORY("examples/osgnvprcustom")
ADD_SUBDIRECTORY("examples/osgnvpreffectcompositor")
ADD_SUBDIRECTORY("examples/osgnvprfbo")
ADD_SUBDIRECTORY("examples/osgnvprviewer")
ADD_SUBDIRECTORY("examples/osgnvprshader")
ADD_SUBDIRECTORY("examples/osgnvprtext")

CONFIGURE_FILE(
	"${CMAKE_CURRENT_SOURCE_DIR}/build/uninstall.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY
)

ADD_CUSTOM_TARGET(uninstall
	"${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
)

# Packaging information.
SET(CPACK_GENERATOR TGZ)
SET(CPACK_SOURCE_GENERATOR TGZ)
SET(CPACK_SOURCE_IGNORE_FILES
	"~$"
	".*.svn"
	".*build/"
	".*debug/"
	".*release/"
)

SET(OSGNVPR_VERSION "1.0.0")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "osgNVPR-${OSGNVPR_VERSION}")

INCLUDE(CPack)

