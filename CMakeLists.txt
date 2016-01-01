
###############################################################################
# cmake settings
###############################################################################

cmake_minimum_required(VERSION 3.0.0)

# In-source builds are disabled.
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
        "CMake generation is not possible within the source directory!\n "
        "Remove CMakeCache.txt and try again from another folder, e.g.:\n "
        "rm CMakeCache.txt\n "
        "mkdir build\n "
        "cd build\n "
        "cmake ..\n "
    )
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")

# Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake Targets")



###############################################################################
# configuration
###############################################################################

enable_testing()
enable_language(C)

set(AUTOCONFIG ${CMAKE_BINARY_DIR}/config_auto.h)
set(AUTOCONFIG_SRC ${CMAKE_BINARY_DIR}/config_auto.h.in)

include(Configure)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/api)

configure_file(${AUTOCONFIG_SRC} ${AUTOCONFIG} @ONLY)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR})

configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/templates/TesseractConfig.cmake.in
    ${CMAKE_BINARY_DIR}/TesseractConfig.cmake @ONLY) 
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/templates/TesseractConfig-version.cmake.in
    ${CMAKE_BINARY_DIR}/TesseractConfig-version.cmake @ONLY)



###############################################################################
# compiler and linker
###############################################################################

if (WIN32)
	if (MSVC)
		add_definitions(-D_SCL_SECURE_NO_WARNINGS)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
	endif()
endif()

if (CYGWIN)
	add_definitions(-D__CYGWIN__)
endif()

if (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
endif()



###############################################################################
# project settings
###############################################################################

if( NOT TESSERACT )
	set(TESSERACT tesseract)
endif()
project(${TESSERACT} C CXX)
set(VERSION_MAJOR 3)
set(VERSION_MINOR 05)
set(VERSION_PLAIN "${VERSION_MAJOR}.${VERSION_MINOR}")


include(LoadLibs)
include(SourceGroups)
include(BuildFunctions)

if(WIN32)
	set(Ws2_32 Ws2_32)
endif()
if(UNIX)
	set(pthread pthread)
endif()

add_definitions(-DHAVE_CONFIG_H)
add_definitions(-DUSE_STD_NAMESPACE=1)
add_definitions(-DWINDLLNAME="${TESSERACT}.dll")
add_definitions(-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS=1)

include_directories(api)
include_directories(ccmain)
include_directories(ccstruct)
include_directories(ccutil)
include_directories(classify)
include_directories(cube)
include_directories(cutil)
include_directories(dict)
include_directories(neural_networks/runtime)
include_directories(opencl)
include_directories(textord)
include_directories(vs2010/port)
include_directories(viewer)
include_directories(wordrec)
include_directories(liblept/src)



###############################################################################
# library settings
###############################################################################

set(LIBRARY_TYPE SHARED)
if (STATIC)
	set(LIBRARY_TYPE)
endif()
file(GLOB HEADERS
	"api/*.h"
	"ccmain/*.h"
	"ccstruct/*.h"
	"ccutil/*.h"
	"classify/*.h"
	"cube/*.h"
	"cutil/*.h"
	"dict/*.h"
	"neural_networks/runtime/*.h"
	"opencl/*.h"
	"textord/*.h"
	"viewer/*.h"
	"wordrec/*.h"
)
file(GLOB SOURCES
	"ccmain/*.cpp"
	"ccstruct/*.cpp"
	"ccutil/*.cpp"
	"classify/*.cpp"
	"cube/*.cpp"
	"cutil/*.cpp"
	"dict/*.cpp"
	"neural_networks/runtime/*.cpp"
	"opencl/*.cpp"
	"textord/*.cpp"
	"viewer/*.cpp"
	"wordrec/*.cpp"
)
if (WIN32)
	file(GLOB WIN32_HEADERS "vs2010/port/*.h")
	file(GLOB WIN32_SOURCES "vs2010/port/*.cpp")
	list(APPEND ${SOURCES} ${WIN32_SOURCES})
	list(APPEND ${HEADERS} ${WIN32_HEADERS})
endif()

set(SOURCES ${SOURCES}
	api/capi.cpp
	api/baseapi.cpp
	api/renderer.cpp
	api/pdfrenderer.cpp
)

add_library(${TESSERACT} ${LIBRARY_TYPE} ${SOURCES} ${HEADERS})
if (NOT STATIC)
	target_compile_definitions(${TESSERACT} PUBLIC -DTESS_EXPORTS)
endif()
target_link_libraries(${TESSERACT} ${LEPTONICA} ${Ws2_32} ${pthread})

string(SUBSTRING ${VERSION_MINOR} 0 1 VERSION_MINOR_0)
string(SUBSTRING ${VERSION_MINOR} 1 1 VERSION_MINOR_1)
set_target_properties(${TESSERACT} PROPERTIES PREFIX "")
set_target_properties(${TESSERACT} PROPERTIES VERSION 
	${VERSION_MAJOR}.${VERSION_MINOR_0}.${VERSION_MINOR_1})
set_target_properties(${TESSERACT} PROPERTIES SOVERSION 
	${VERSION_MAJOR}.${VERSION_MINOR_0}.${VERSION_MINOR_1})
if(WIN32)
	set_target_properties(
		${TESSERACT} PROPERTIES OUTPUT_NAME ${TESSERACT})
	set_target_properties(
		${TESSERACT} PROPERTIES COMPILE_PDB_NAME ${TESSERACT})
	set_target_properties(
		${TESSERACT} PROPERTIES DEBUG_OUTPUT_NAME '${TESSERACT}d')
endif()
#export(TARGETS ${TESSERACT} FILE ${CMAKE_BINARY_DIR}/TesseractTargets.cmake)



###############################################################################
# program settings
###############################################################################

set(APPMAIN appmain)
set(SOURCES
	api/tesseractmain.cpp 
	vs2010/tesseract/resource.h vs2010/tesseract/tesseract.rc
)
add_executable(${APPMAIN} ${SOURCES})
target_link_libraries(${APPMAIN} ${TESSERACT})
set_target_properties(${APPMAIN} PROPERTIES OUTPUT_NAME ${TESSERACT})


add_subdirectory(training)
