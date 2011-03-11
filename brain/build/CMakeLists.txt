#CMakeLists.txt
#Copyright (c) 2009, Ewen Cheslack-Postava
#All rights reserved.
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#
#    * Redistributions of source code must retain the above copyright notice,
#      this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright notice,
#      this list of conditions and the following disclaimer in the documentation
#      and/or other materials provided with the distribution.
#    * Neither the name of brain nor the names of its contributors
#      may be used to endorse or promote products derived from this software
#      without specific prior written permission.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
#ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
#ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#cmake options

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
SET(CMAKE_VERBOSE_MAKEFILE OFF)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
IF(NOT CMAKE_BUILD_TYPE)
#the below lines work around a bug when cmake first is configured without <CONFIG>
  SET(FORCE_USE_DEBUG_LOCATION "TRUE")
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Build types include: Default Debug Release None RelWithDebInfo." FORCE
      )
ENDIF()

IF(NOT WIN32)
SET(BRAIN_DEFAULT_COMPILE_FLAGS "-Wall -Wno-unused-function -Wno-unused-variable -fvisibility=hidden")
ENDIF()
IF(WIN32)
SET(elysia_LDFLAGS "")
ELSE()
SET(elysia_LDFLAGS "-lpthread ")
ENDIF()

IF(APPLE)
SET(CMAKE_CXX_COMPILER /usr/bin/g++-4.0)
  SET(CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX10.5.sdk")
  SET(CMAKE_OSX_ARCHITECTURES "i386")
  SET(CMAKE_OSX_DEPLOYMENT_TARGET "10.5")
  SET(CMAKE_EXE_LINKER_FLAGS "-Wl,-syslibroot -Wl,/Developer/SDKs/MacOSX10.5.sdk")
  ADD_DEFINITIONS(-isysroot /Developer/SDKs/MacOSX10.5.sdk -arch i386 -mmacosx-version-min=10.5)
  SET(elysia_LDFLAGS "${elysia_LDFLAGS} -Wl,-syslibroot -Wl,/Developer/SDKs/MacOSX10.5.sdk")
  SET(CMAKE_CXX_FLAGS -isysroot /Developer/SDKs/MacOSX10.5.sdk -arch i386 -mmacosx-version-min=10.5)
ENDIF()
SET(CMAKE_CXX_FLAGS ${BRAIN_DEFAULT_COMPILE_FLAGS})

# Default build target
SET(CMAKE_CXX_FLAGS_DEFAULT "-O0 -g2 ${BRAIN_DEFAULT_COMPILE_FLAGS}" CACHE STRING
    "Built cxxflags with default settings."
    FORCE )
SET( CMAKE_C_FLAGS_DEFAULT "-g2 ${BRAIN_DEFAULT_COMPILE_FLAGS}" CACHE STRING
    "Built cflags with default settings."
    FORCE )
SET( CMAKE_EXE_LINKER_FLAGS_DEFAULT
    "" CACHE STRING
    "Linking binaries with default settings."
    FORCE )
SET( CMAKE_SHARED_LINKER_FLAGS_DEFAULT
    "" CACHE STRING
    "Building shared libraries with default settings."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_DEFAULT
    CMAKE_C_FLAGS_DEFAULT
    CMAKE_EXE_LINKER_FLAGS_DEFAULT
    CMAKE_SHARED_LINKER_FLAGS_DEFAULT )

# Profile build target
SET(CMAKE_CXX_FLAGS_PROFILE "-g2 -O3 -pg ${BRAIN_DEFAULT_COMPILE_FLAGS}" CACHE STRING
    "Built cxxflags with default settings."
    FORCE )
SET( CMAKE_C_FLAGS_PROFILE "-g2 -O3 -pg ${BRAIN_DEFAULT_COMPILE_FLAGS}" CACHE STRING
    "Built cflags with default settings."
    FORCE )
SET( CMAKE_EXE_LINKER_FLAGS_PROFILE
    "-pg" CACHE STRING
    "Linking binaries with default settings."
    FORCE )
SET( CMAKE_SHARED_LINKER_FLAGS_PROFILE
    "-pg" CACHE STRING
    "Building shared libraries with default settings."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_PROFILE
    CMAKE_C_FLAGS_PROFILE
    CMAKE_EXE_LINKER_FLAGS_PROFILE
    CMAKE_SHARED_LINKER_FLAGS_PROFILE )

# Coverage build target
SET(CMAKE_CXX_FLAGS_COVERAGE "-g2 -fprofile-arcs -ftest-coverage ${BRAIN_DEFAULT_COMPILE_FLAGS}" CACHE STRING
    "Built cxxflags with default settings."
    FORCE )
SET( CMAKE_C_FLAGS_COVERAGE "-g2 -fprofile-arcs -ftest-coverage ${BRAIN_DEFAULT_COMPILE_FLAGS}" CACHE STRING
    "Built cflags with default settings."
    FORCE )
SET( CMAKE_EXE_LINKER_FLAGS_COVERAGE
    "-fprofile-arcs -ftest-coverage" CACHE STRING
    "Linking binaries with default settings."
    FORCE )
SET( CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    "-fprofile-arcs -ftest-coverage" CACHE STRING
    "Building shared libraries with default settings."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE )

#project global flags

PROJECT(brain C CXX)

IF(NOT TOP_LEVEL)
  SET(TOP_LEVEL ${CMAKE_CURRENT_SOURCE_DIR}/..)
ENDIF()
GET_FILENAME_COMPONENT(TOP_LEVEL ${TOP_LEVEL} ABSOLUTE)

IF(NOT DEPENDENCIES_ROOT)
  SET(DEPENDENCIES_ROOT ${TOP_LEVEL}/../externals/win32depends/)
  IF(WIN32)
    SET(PLATFORM_DEPENDENCIES_ROOT ${TOP_LEVEL}/../externals/win32depends/)
  ELSE()
    IF(APPLE)
      SET(PLATFORM_DEPENDENCIES_ROOT ${TOP_LEVEL}/../externals/macdepends/)
    ELSE()
      SET(PLATFORM_DEPENDENCIES_ROOT ${TOP_LEVEL}/../externals/linuxdepends/)
    ENDIF()
  ENDIF()
ENDIF()
#add-ins

IF(NOT CMAKE_MODULE_PATH)
  SET(CMAKE_MODULE_PATH ${TOP_LEVEL}/build/modules)
ENDIF()
GET_FILENAME_COMPONENT(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ABSOLUTE)
INCLUDE(ListUtil)
INCLUDE(ParseArguments)
INCLUDE(AddPluginTarget)


MACRO(VERIFY_VERSION package_name major minor subminor)
  IF (${package_name}_MAJOR_VERSION LESS ${major})
    MESSAGE(FATAL_ERROR "${package_name} out of date: ${major}.${minor}.${subminor}")
  ELSEIF(${package_name}_MAJOR_VERSION EQUAL ${major})
    IF(${package_name}_MINOR_VERSION LESS ${minor})
       MESSAGE(FATAL_ERROR "${package_name} out of date: ${major}.${minor}.${subminor}")
    ELSEIF(${package_name}_MINOR_VERSION EQUAL ${major})
      IF(${package_name}_SUBMINOR_VERSION LESS ${subminor})
        MESSAGE(FATAL_ERROR "${package_name} out of date: ${major}.${minor}.${subminor}")
      ENDIF()
    ENDIF()
  ENDIF()
ENDMACRO(VERIFY_VERSION)


#libraries

# first we find/verify sirikata and installed-sirikata, which will
# provide a lot of useful tools
SET(SIRIKATA_CODE_ROOT)
IF(EXISTS ${DEPENDENCIES_ROOT}/sirikata)
  SET(SIRIKATA_CODE_ROOT ${DEPENDENCIES_ROOT}/sirikata)
ENDIF()

SET(SIRIKATA_INSTALL_ROOT)
IF(EXISTS ${DEPENDENCIES_ROOT}/installed-sirikata)
  SET(SIRIKATA_INSTALL_ROOT ${DEPENDENCIES_ROOT}/installed-sirikata)
ELSE()
IF(EXISTS ${DEPENDENCIES_ROOT}/)
  SET(SIRIKATA_INSTALL_ROOT ${DEPENDENCIES_ROOT})
ENDIF()
ENDIF()


#dependency: sirikata
FIND_PACKAGE(Sirikata)


# add sirikata's cmake modules
IF(Sirikata_CMAKE_MODULES)
  SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${Sirikata_CMAKE_MODULES})
ENDIF()

IF(EXISTS ${PLATFORM_DEPENDENCIES_ROOT}/installed-protobufs)
SET(PROTOCOLBUFFERS_ROOT ${PLATFORM_DEPENDENCIES_ROOT}/installed-protobufs)
ELSE()
SET(PROTOCOLBUFFERS_ROOT ${PLATFORM_DEPENDENCIES_ROOT})
ENDIF()
#dependency: protocolbuffers
IF(NOT PROTOCOLBUFFERS_ROOT)
  IF(Sirikata_PROTOBUFS_ROOT)
    SET(PROTOCOLBUFFERS_ROOT ${Sirikata_PROTOBUFS_ROOT})
  ENDIF()
ENDIF()
FIND_PACKAGE(ProtocolBuffers)
IF(NOT PROTOCOLBUFFERS_FOUND)
  MESSAGE(FATAL_ERROR "Cannot find Protocol Buffers")
ENDIF()

IF(WIN32)
SET(PROTOCOLBUFFERS_COMPILER ${DEPENDENCIES_ROOT}/bin/protoc.exe)
ENDIF()

SET(GLUT_ROOT  ${PLATFORM_DEPENDENCIES_ROOT})
FIND_PACKAGE(GLUT)
SET(BOOST_ROOT ${PLATFORM_DEPENDENCIES_ROOT})
IF(NOT WIN32)
  SET(BOOST_ROOT ${PLATFORM_DEPENDENCIES_ROOT}/installed-boost)
ENDIF()
#dependency: boost > 1.35
IF(NOT BOOST_ROOT)
  IF(Sirikata_BOOST_ROOT)
    SET(BOOST_ROOT ${Sirikata_BOOST_ROOT})
  ELSE()
  ENDIF()
ENDIF()
SET(Boost_FIND_REQUIRED TRUE)
IF(WIN32)
  FIND_PACKAGE(Boost COMPONENTS thread date_time)# COMPONENTS system thread date_time program_options)
ELSE()
  FIND_PACKAGE(Boost COMPONENTS thread)# COMPONENTS system)
  #SET(Boost_THREAD_LIBRAR_DIRS ${BOOST_ROOT}/lib)
  #SET(Boost_THREAD_LIBRARY boost_thread)
  #STRING(REPLACE "boost_system" "boost_thread" Boost_THREAD_LIBRARY ${Boost_SYSTEM_LIBRARY})
  #STRING(REPLACE "boost_system" "boost_regex" Boost_REGEX_LIBRARY ${Boost_SYSTEM_LIBRARY})
  #STRING(REPLACE "boost_system" "boost_date_time" Boost_DATE_TIME_LIBRARY ${Boost_SYSTEM_LIBRARY})
  #STRING(REPLACE "boost_system" "boost_program_options" Boost_PROGRAM_OPTIONS_LIBRARY ${Boost_SYSTEM_LIBRARY})
ENDIF()
VERIFY_VERSION(Boost 1 37 0)


#source file location
SET(BRAIN_SOURCE_DIR ${TOP_LEVEL})
IF(WIN32)
ADD_DEFINITIONS(/FI\"${BRAIN_SOURCE_DIR}/Platform.hpp\")
ELSE()
ADD_DEFINITIONS(-include \"${BRAIN_SOURCE_DIR}/Platform.hpp\")
ENDIF()
SET(FINAL_INCLUDE_DIRS ${BRAIN_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
        ${Sirikata_INCLUDE_DIR}
        ${PROTOCOLBUFFERS_INCLUDE_DIRS}) 
IF(GLUT_FOUND)
  SET(FINAL_INCLUDE_DIRS ${FINAL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
ENDIF()
#cxx flags


INCLUDE_DIRECTORIES(BEFORE ${FINAL_INCLUDE_DIRS})        

SET(FINAL_LINK_DIRS ${Boost_LIBRARY_DIRS})
IF(GLUT_FOUND)
  SET(FINAL_LINK_DIRS ${FINAL_LINK_DIRS}         ${GLUT_LIBRARY_DIRS})
ENDIF()
#link flags
LINK_DIRECTORIES(${FINAL_LINK_DIRS})

#source files
SET(BRAIN_CORE_SOURCES
)

ADD_CUSTOM_TARGET(genome
                  DEPENDS ${BRAIN_SOURCE_DIR}/genome.pb.cc ${BRAIN_SOURCE_DIR}/genome.pb.h)
ADD_CUSTOM_COMMAND(OUTPUT ${BRAIN_SOURCE_DIR}/genome.pb.cc ${BRAIN_SOURCE_DIR}/genome.pb.h
                   COMMAND ${PROTOCOLBUFFERS_COMPILER} ${BRAIN_SOURCE_DIR}/genome.proto -I${BRAIN_SOURCE_DIR} --cpp_out=dllexport_decl=BRAIN_CORE_EXPORT:${BRAIN_SOURCE_DIR}
                   DEPENDS ${BRAIN_SOURCE_DIR}/genome.proto)


SET(BRAIN_SOURCES
  ${BRAIN_SOURCE_DIR}/main.cpp
  ${BRAIN_SOURCE_DIR}/Brain.cpp  
  ${BRAIN_SOURCE_DIR}/Base64.cpp
  ${BRAIN_SOURCE_DIR}/genome.pb.cc
  ${BRAIN_SOURCE_DIR}/SimpleProteinEnvironment.cpp
)





#libs
#ADD_LIBRARY(cbr-core STATIC ${BRAIN_CORE_SOURCES} ${BRAIN_PROTOCOL_SOURCES})

#binaries
#ADD_EXECUTABLE(brain ${BRAIN_SOURCES})
#ADD_DEPENDENCIES(brain genome)
#TARGET_LINK_LIBRARIES(brain
#  cbr-core
#        ${PROTOCOLBUFFERS_LIBRARIES}
#        )


#Nghia's ANN test
SET(NEAREST_NEIGHBOR_SOURCES
  ${BRAIN_SOURCE_DIR}/main.cpp
  ${BRAIN_SOURCE_DIR}/test.cpp
  ${BRAIN_SOURCE_DIR}/test.hpp
)
SET(BRAIN_CORE_LIB_SOURCES 
  ${BRAIN_SOURCE_DIR}/ActivityStats.hpp
  ${BRAIN_SOURCE_DIR}/ActivityStats.cpp
  ${BRAIN_SOURCE_DIR}/Brain.cpp
  ${BRAIN_SOURCE_DIR}/BrainPlugins.cpp
  ${BRAIN_SOURCE_DIR}/BrainPlugin.cpp
  ${BRAIN_SOURCE_DIR}/SharedLibrary.cpp
  ${BRAIN_SOURCE_DIR}/Brain.hpp  
  ${BRAIN_SOURCE_DIR}/Base64.cpp
  ${BRAIN_SOURCE_DIR}/Base64.hpp
  ${BRAIN_SOURCE_DIR}/CellComponent.cpp
  ${BRAIN_SOURCE_DIR}/CellComponent.hpp  
  ${BRAIN_SOURCE_DIR}/MainThread.cpp
  ${BRAIN_SOURCE_DIR}/MainThread.hpp
  ${BRAIN_SOURCE_DIR}/Placeable.cpp
  ${BRAIN_SOURCE_DIR}/Placeable.hpp
  ${BRAIN_SOURCE_DIR}/Neuron.cpp
  ${BRAIN_SOURCE_DIR}/Neuron.hpp  
  ${BRAIN_SOURCE_DIR}/DevelopmentFactory.hpp  
  ${BRAIN_SOURCE_DIR}/DevelopmentFactory.cpp  
  ${BRAIN_SOURCE_DIR}/StandardDevelopment.hpp  
  ${BRAIN_SOURCE_DIR}/Development.hpp  
  ${BRAIN_SOURCE_DIR}/Synapse.cpp
  ${BRAIN_SOURCE_DIR}/Synapse.hpp  
  ${BRAIN_SOURCE_DIR}/Branch.cpp
  ${BRAIN_SOURCE_DIR}/Branch.hpp
  ${BRAIN_SOURCE_DIR}/ProteinEnvironment.cpp
  ${BRAIN_SOURCE_DIR}/ProteinEnvironment.hpp
  ${BRAIN_SOURCE_DIR}/ProteinDensity.hpp
  ${BRAIN_SOURCE_DIR}/ProteinDensity.cpp
  ${BRAIN_SOURCE_DIR}/SimpleProteinEnvironment.cpp
  ${BRAIN_SOURCE_DIR}/SimpleProteinEnvironment.hpp
  ${BRAIN_SOURCE_DIR}/SimpleSpatialSearch.cpp
  ${BRAIN_SOURCE_DIR}/SimpleSpatialSearch.hpp
  ${BRAIN_SOURCE_DIR}/TreeNNSpatialSearch.cpp
  ${BRAIN_SOURCE_DIR}/TreeNNSpatialSearch.hpp
  ${BRAIN_SOURCE_DIR}/AnnSpace.hpp
  ${BRAIN_SOURCE_DIR}/AnnSpace.cpp
  ${BRAIN_SOURCE_DIR}/AnnPoints.hpp
  ${BRAIN_SOURCE_DIR}/AnnPoints.cpp
  ${BRAIN_SOURCE_DIR}/genome.pb.cc)

#Andrew's brain directory
SET(BRAINSANDBOX_SOURCES
  ${BRAIN_SOURCE_DIR}/main.cpp
  ${BRAIN_SOURCE_DIR}/test.cpp
  ${BRAIN_SOURCE_DIR}/test.hpp
  ${BRAIN_SOURCE_DIR}/test_sequence.cpp
  ${BRAIN_SOURCE_DIR}/test_sequence.hpp
)
SET(BRAIN_PLUGIN_DIR ${BRAIN_SOURCE_DIR}/plugins)

SET(BRAIN_PLUGIN_VIS_DIR ${BRAIN_PLUGIN_DIR}/vis)

SET(BRAIN_PLUGIN_NAIVE_DIR ${BRAIN_PLUGIN_DIR}/naive)

SET(BRAIN_PLUGIN_VIS_SOURCES ${BRAIN_PLUGIN_VIS_DIR}/vis_plugin.cpp ${BRAIN_PLUGIN_VIS_DIR}/Visualization.cpp ${BRAIN_PLUGIN_VIS_DIR}/GraphicsSystem.cpp )
SET(BRAIN_PLUGIN_NAIVE_SOURCES 
	${BRAIN_PLUGIN_NAIVE_DIR}/naive_plugin.cpp 
	${BRAIN_PLUGIN_NAIVE_DIR}/NaiveDevelopment.hpp 
	${BRAIN_PLUGIN_NAIVE_DIR}/NaiveDevelopment.cpp 
	${BRAIN_PLUGIN_NAIVE_DIR}/RefinedDevelopment.cpp 
	${BRAIN_PLUGIN_NAIVE_DIR}/RefinedDevelopment.hpp
	${BRAIN_PLUGIN_NAIVE_DIR}/RotatingDevelopment.hpp 
	${BRAIN_PLUGIN_NAIVE_DIR}/RotatingDevelopment.cpp 
) 

SET(VIS_PLUGIN_LIBS ${GLUT_LIBRARIES} brain-core)
SET(NAIVE_PLUGIN_LIBS brain-core)
IF(WIN32)
SET(BRAIN_CORE_LIB_LIBS ${BRAIN_CORE_LIBS} debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE} debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
SET(VIS_PLUGIN_LIBS ${VIS_PLUGIN_LIBS} debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE} debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
ELSE()
SET(BRAIN_CORE_LIB_LIBS ${BRAIN_CORE_LIBS} ${Boost_THREAD_LIBRARY_RELEASE}  ${Boost_DATE_TIME_LIBRARY_RELEASE})
SET(VIS_PLUGIN_LIBS ${VIS_PLUGIN_LIBS} ${Boost_THREAD_LIBRARY_RELEASE}  ${Boost_DATE_TIME_LIBRARY_RELEASE})
ENDIF()

SET(vis_LDFLAGS ${GLUT_LDFLAGS})
IF(APPLE)
SET(vis_LDFLAGS "${vis_LDFLAGS} -framework OpenGL")
ENDIF()
ADD_PLUGIN_TARGET(vis
                    SOURCES ${BRAIN_PLUGIN_VIS_SOURCES}
                    TARGET_LDFLAGS "${elysia_LDFLAGS} ${vis_LDFLAGS}"
                    LIBRARIES ${VIS_PLUGIN_LIBS}
                    TARGET_LIBRARIES brain-core)

ADD_PLUGIN_TARGET(naive
                    SOURCES ${BRAIN_PLUGIN_NAIVE_SOURCES}
                    TARGET_LDFLAGS "${elysia_LDFLAGS}"
                    LIBRARIES ${NAIVE_PLUGIN_LIBS}
                    TARGET_LIBRARIES brain-core)


ADD_LIBRARY(brain-core SHARED ${BRAIN_CORE_LIB_SOURCES})
SET_TARGET_PROPERTIES(brain-core PROPERTIES COMPILE_DEFINITIONS BRAIN_CORE_BUILD)
SET_TARGET_PROPERTIES(brain-core PROPERTIES LINK_FLAGS "${elysia_LDFLAGS}")
IF(NOT WIN32)
FIND_LIBRARY(DYLIB_LIBRARY NAMES dl)
SET(PROTOCOLBUFFERS_LIBRARIES ${PROTOCOLBUFFERS_LIBRARIES} ${DYLIB_LIBRARY})
ENDIF()
TARGET_LINK_LIBRARIES(brain-core
        ${BRAIN_CORE_LIB_LIBS}
#  cbr-core
        ${PROTOCOLBUFFERS_LIBRARIES}
        )
#libs
#ADD_LIBRARY(cbr-core STATIC ${BRAIN_CORE_SOURCES} ${BRAIN_PROTOCOL_SOURCES})

#binaries
ADD_EXECUTABLE(nearestneighbor ${NEAREST_NEIGHBOR_SOURCES})
ADD_DEPENDENCIES(nearestneighbor genome)
ADD_DEPENDENCIES(nearestneighbor brain-core)
ADD_DEPENDENCIES(vis naive brain-core)


TARGET_LINK_LIBRARIES(nearestneighbor
#  cbr-core
        brain-core
        ${PROTOCOLBUFFERS_LIBRARIES}
        )

#binaries
ADD_EXECUTABLE(brainsandbox ${BRAINSANDBOX_SOURCES})
ADD_DEPENDENCIES(brainsandbox genome)
TARGET_LINK_LIBRARIES(brainsandbox
#  cbr-core
        brain-core
        ${PROTOCOLBUFFERS_LIBRARIES}
        )
ADD_DEPENDENCIES(brainsandbox brain-core)
SET_TARGET_PROPERTIES(nearestneighbor PROPERTIES LINK_FLAGS "${elysia_LDFLAGS}")
SET_TARGET_PROPERTIES(brainsandbox PROPERTIES LINK_FLAGS "${elysia_LDFLAGS}")

SET_TARGET_PROPERTIES(brainsandbox brain-core nearestneighbor vis naive
                      PROPERTIES
                      DEBUG_POSTFIX "_d" )
SET(CMAKE_DEBUG_POSTFIX "_d")
