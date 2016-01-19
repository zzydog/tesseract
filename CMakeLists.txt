
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

set(CMAKE_BINARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_OUTPUT_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")

# Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake Targets")



###############################################################################
# configuration
###############################################################################

enable_testing()
enable_language(C)

include(Configure)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/api)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR})

configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/templates/TesseractConfig.cmake.in
    ${CMAKE_BINARY_DIR}/TesseractConfig.cmake @ONLY) 
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/templates/TesseractConfig-version.cmake.in
    ${CMAKE_BINARY_DIR}/TesseractConfig-version.cmake @ONLY)



###############################################################################
# project settings
###############################################################################

if( NOT TESSERACT )
    set(TESSERACT libtess)
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
if( WIN32 )
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

if( NOT TESSERACT_LIBRARY_TYPE )
    set(TESSERACT_LIBRARY_TYPE "SHARED")
else()
    set(TESSERACT_LIBRARY_TYPE "STATIC")
endif()
add_library(
    ${TESSERACT} ${TESSERACT_LIBRARY_TYPE} ${SOURCES} ${HEADERS})
if( TESSERACT_LIBRARY_TYPE STREQUAL "SHARED" )
    # Export all interfaces if compiling a shared library
    target_compile_definitions(${TESSERACT} PRIVATE TESS_EXPORTS)
    target_compile_definitions(${TESSERACT} INTERFACE TESS_IMPORTS)
endif()
target_link_libraries(${TESSERACT} ${LEPTONICA} ${Ws2_32} ${pthread})

string(SUBSTRING ${VERSION_MINOR} 0 1 VERSION_MINOR_0)
string(SUBSTRING ${VERSION_MINOR} 1 1 VERSION_MINOR_1)
set_target_properties(${TESSERACT} PROPERTIES PREFIX "")
set_target_properties(${TESSERACT} PROPERTIES VERSION 
    "${VERSION_MAJOR}.${VERSION_MINOR_0}.${VERSION_MINOR_1}")
set_target_properties(${TESSERACT} PROPERTIES SOVERSION 
    "${VERSION_MAJOR}.${VERSION_MINOR_0}.${VERSION_MINOR_1}")
set_target_properties(
    ${TESSERACT} PROPERTIES LIBRARY_OUTPUT_NAME ${TESSERACT})
if( MSVC )
    set_target_properties(
        ${TESSERACT} PROPERTIES COMPILE_PDB_NAME ${TESSERACT})
endif()

# Compiler/Linker features on the target

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
    # Enable C++11 support
    target_compile_options(${TESSERACT} PRIVATE -std=c++11)
endif()

if ( WIN32 )
    target_compile_definitions(${TESSERACT} PRIVATE __WIN32__)
endif()

if (CYGWIN)
    target_compile_definitions(${TESSERACT} PRIVATE __CYGWIN__)
endif()

if ( MSVC AND WIN32 )
    # Enable Security Features
    target_compile_options(${TESSERACT} PRIVATE /GS)
    if (NOT ( MSVC_VERSION LESS 1900 ))
        target_compile_options(${TESSERACT} PRIVATE /guard:cf)
    endif()
    if (NOT ( MSVC_VERSION LESS 1700 ))
        target_compile_options(${TESSERACT} PRIVATE /sdl /analyze)
    endif()
    # Multiple Processes Building
    if(MSVC_VERSION GREATER 1400)
        set(PROCESSOR_COUNT "$ENV{NUMBER_OF_PROCESSORS}")
        target_compile_options(${TESSERACT} PRIVATE "/MP${PROCESSOR_COUNT}")
    endif()
endif()

if ( MINGW AND WIN32 )
    # Enable Security Features
    target_compile_options(${TESSERACT} PRIVATE -Wall)
    target_compile_definitions(${TESSERACT} PRIVATE _FORTIFY_SOURCE=2)
    target_compile_options(${TESSERACT} PRIVATE -fstack-protector-strong)
    # Enable security APIs
    target_compile_definitions(${TESSERACT} PRIVATE MINGW_HAS_SECURE_API)
endif()



###############################################################################
# program settings
###############################################################################

set(APPMAIN tesseract)
set(SOURCES
    api/tesseractmain.cpp 
    vs2010/tesseract/resource.h vs2010/tesseract/tesseract.rc
)
add_executable(${APPMAIN} ${SOURCES})
target_link_libraries(${APPMAIN} ${TESSERACT} ${pthread})
set_target_properties(${APPMAIN} PROPERTIES OUTPUT_NAME ${APPMAIN})
add_test(NAME appmain-help COMMAND appmain "--help")
add_test(NAME appmain-version COMMAND appmain "--version")


add_subdirectory(training)
