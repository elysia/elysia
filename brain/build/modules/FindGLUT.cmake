# Try to find PROTOCOLBUFFERS include dirs and libraries
# Usage of this module is as follows:
# SET(PROTOCOLBUFFERS_ROOT /path/to/root)
# FIND_PACKAGE(ProtocolBuffers)
#
# Variables defined by this module:
#
#   PROTOCOLBUFFERS_FOUND              Set to true to indicate the library was found
#   PROTOCOLBUFFERS_INCLUDE_DIRS       The directory containing the protocol buffers header files
#   PROTOCOLBUFFERS_LIBRARIES          The libraries needed to use protocol buffers (without the full path)
#   PROTOCOLBUFFERS_COMPILER           The protocol buffers compiler
#   PROTOCOLBUFFERS_MONO_COMPILER      The protocol buffers mono compiler
#
#   PROTOCOLBUFFERS_SUPPORTS_CSHARP    True if this compiler supports C#
#
# Copyright (C) Ewen Cheslack-Postava, 2009
#

# Look for the protocol buffers headers, first in the additional location and then in default system locations
FIND_PATH(GLUT_INCLUDE_DIRS NAMES ${GLUT_ROOT}/include/GL/glut.h ${GLUT_ROOT}/include/glut.h DOC "Location of glut header files" NO_DEFAULT_PATH)
IF(NOT GLUT_INCLUDE_DIRS)
    FIND_PATH(GLUT_INCLUDE_DIRS NAMES GL/glut.h glut.h DOC "Location of glut header files")
ENDIF()

SET(GLUT_FOUND FALSE)
IF(GLUT_INCLUDE_DIRS)
    # toplevel directory
    SET(GLUT_ROOT_DIRS ${GLUT_INCLUDE_DIRS})
    IF("${GLUT_ROOT_DIRS}" MATCHES "/include$")
        # Destroy trailing "/include" in the path.
        GET_FILENAME_COMPONENT(GLUT_ROOT_DIRS ${GLUT_ROOT_DIRS} PATH)
    ENDIF()


    # library path
    SET(GLUT_LIBRARY_DIRS ${GLUT_ROOT_DIRS})
    IF(EXISTS "${GLUT_LIBRARY_DIRS}/lib")
        SET(GLUT_LIBRARY_DIRS ${GLUT_LIBRARY_DIRS}/lib)
    ENDIF(EXISTS "${GLUT_LIBRARY_DIRS}/lib")

    IF(WIN32) # must distinguish debug and release builds
        SET(GLUT_LIBRARIES)
        FIND_LIBRARY(GLUT_LIBRARIES NAMES glut32 libglut32 glut libglut 
                                 PATH_SUFFIXES "" PATHS ${GLUT_LIBRARY_DIRS} NO_DEFAULT_PATH)
    ELSE()
        FIND_LIBRARY(GLUT_LIBRARIES NAMES glut PATHS ${GLUT_LIBRARY_DIRS})
    ENDIF()

    IF(GLUT_LIBRARIES AND GLUT_INCLUDE_DIRS)
        SET(GLUT_FOUND TRUE)
    ENDIF()
ENDIF()

IF(GLUT_FOUND)
    IF(NOT GLUT_FIND_QUIETLY)
        MESSAGE(STATUS "GLUT: includes at ${GLUT_INCLUDE_DIRS}, libraries at ${GLUT_LIBRARIES}")
    ENDIF()
ELSE()
    IF(GLUT_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "GLUT not found")
    ENDIF()
ENDIF()
