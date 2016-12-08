# Usage:
#  Create build folder, like RC-build next to RobotControl and WifiScan folder
#  CD into build folder and execute 'cmake -DCMAKE_BUILD_TYPE=Debug ../RobotControl'
#  make

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# select build type
SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" )

PROJECT(KLib_Tests)

IF(NOT CMAKE_BUILD_TYPE)
	MESSAGE(STATUS "No build type selected. Default to Debug")
	SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()


# all supported options:
option(WITH_ZLIB "Build with zlib compression support" OFF)
MESSAGE(STATUS "zlib compression support (WITH_ZLIB):			${WITH_ZLIB}")
IF(WITH_ZLIB)
	add_definitions(-DWITH_ZLIB)
	SET(EXTRA_LIBS ${EXTRA_LIBS} z)
ENDIF()

OPTION(WITH_SSL "Build with SSL encryption support" OFF)
MESSAGE(STATUS "SSL encryption support (WITH_SSL):				${WITH_SSL}")
IF(WITH_SSL)
	add_definitions(-DWITH_SSL)
	SET(EXTRA_LIBS ${EXTRA_LIBS} ssl crypto)
ENDIF()

OPTION(WITH_7Z "Build with 7z compression support" OFF)
MESSAGE(STATUS "7z compression support (WITH_7z):				${WITH_7Z}")
IF(WITH_7Z)
	add_definitions(-DWITH_7Z)
	add_definitions(-D_7ZIP_ST)
	SET(EXTRA_SOURCES ${EXTRA_SOURCES} ./inc/7z/*.c)
ENDIF()

OPTION(WITH_PNG "Build with PNG image support" OFF)
IF(WITH_PNG)
	MESSAGE(STATUS "PNG image support: ENABLED")
	add_definitions(-DWITH_PNG)
	SET(EXTRA_LIBS ${EXTRA_LIBS} png)
ELSE()
	MESSAGE(STATUS "PNG image support: DISABLED")
ENDIF()


OPTION(WITH_JPEG "Build with JPEG image support" OFF)
IF(WITH_JPEG)
	MESSAGE(STATUS "JPEG image support: ENABLED")
	add_definitions(-DWITH_JPEG)
	SET(EXTRA_LIBS ${EXTRA_LIBS} jpeg)
ELSE()
	MESSAGE(STATUS "JPEG image support: DISABLED")
ENDIF()

OPTION(WITH_OPENCL "Build with OpenCL support" ON)
IF(WITH_OPENCL)
	MESSAGE(STATUS "OpenCL support: ENABLED")
	add_definitions(-DWITH_OPENCL)
	SET(EXTRA_LIBS ${EXTRA_LIBS} OpenCL)
ELSE()
	MESSAGE(STATUS "OpenCL support: DISABLED")
ENDIF()

# for libeigen 3.3.x
INCLUDE_DIRECTORIES(
	/apps/lib/
)

FILE(GLOB HEADERS
	./*.h
	./*/*.h
	./*/*/*.h
	./*/*/*/*.h
	./*/*/*/*/*.h
)

FILE(GLOB SOURCES
	./*.cpp
	./*/*.cpp
	./*/*/*.cpp
	./*/*/*/*.cpp
	./*/*/*/*/*.cpp
	${EXTRA_SOURCES}
)

ADD_DEFINITIONS(-DWITH_TESTS)
ADD_DEFINITIONS(-DWITH_ASSERTIONS)
ADD_DEFINITIONS(-DWITH_VERBOSE)

# system specific compiler flags
IF(MSVC)
	ADD_DEFINITIONS(-W3)
ELSE()
	ADD_DEFINITIONS(
		-std=c++11
		-Wall
		-Wextra
		-Wconversion
		-Werror=return-type
		-fstack-protector-strong
		-g
		-O0
	)
ENDIF()



# build a binary file
ADD_EXECUTABLE(
	${PROJECT_NAME}
	${HEADERS}
	${SOURCES}
)

# needed external libraries
TARGET_LINK_LIBRARIES(
	${PROJECT_NAME}
	gtest
	pthread
	${EXTRA_LIBS}
)


find_package(OpenMP)
if (OPENMP_FOUND)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
