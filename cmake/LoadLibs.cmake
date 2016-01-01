
###############################################################################
#  Detect and load 3rd-party libraries
###############################################################################

include(FindLiblept)
if(LEPTONICA_FOUND AND EXISTS ${LEPTONICA_INCLUDE_DIRS}/allheaders.h)
	set(LEPTONICA ${LEPTONICA_LIBRARIES})
	include_directories(${LEPTONICA_INCLUDE_DIRS})
	message(
		STATUS "Find the 'liblept' package in ${LEPTONICA_INCLUDE_DIRS}")
else()
	set(LEPTONICA liblept)
	message(STATUS "Can't find 'liblept', we'll build it")
	add_subdirectory("${CMAKE_SOURCE_DIR}/liblept")
	include_directories(
		"${${LEPTONICA}_SOURCE_DIR}" "${${LEPTONICA}_BINARY_DIR}")
endif()

find_package(PkgConfig)
find_package(OpenCL QUIET)
find_package(ICU COMPONENTS uc i18n)

