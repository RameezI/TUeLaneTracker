#Creates a library called "TUeLDT" which includes all source files in directory TUeLDT.
#Any cpp or c file place in this directory will be compiled as a part of the library by cmake.
#Place all headers in the include subdirectory inside the TUeLDT directory
cmake_minimum_required (VERSION 2.8.11)
find_package(OpenCV REQUIRED)
find_package(S32VSDK QUIET)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -O0 -std=c++11")
file(GLOB  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c )

add_library(TUeLDT STATIC ${SOURCES})

set(TUeLDT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
             ${CMAKE_CURRENT_SOURCE_DIR}/../include
             ${CMAKE_CURRENT_SOURCE_DIR}/../3rdPartyLibs
             ${OpenCV_INCLUDE_DIRS})

message("Includes before append: ${TUeLDT_INCLUDE_DIRS}")

IF(S32VSDK_FOUND)
    list(APPEND TUeLDT_INCLUDE_DIRS ${S32VSDK_INCLUDE_DIRS})
    list(APPEND TUeLDT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/tue_lane_tracker/LaneTrackerApp/A53_inc)
    list(APPEND TUeLDT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/tue_lane_tracker/LaneTrackerApp/APU_gen/src)
    list(APPEND TUeLDT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/tue_lane_tracker/LaneTrackerApp/A53_gen/inc)
    target_compile_definitions(TUeLDT PUBLIC DISPLAY_GRAPHICS_DCU S32V2XX OSDIR=linux)
ENDIF(S32VSDK_FOUND)

target_include_directories (TUeLDT PUBLIC ${TUeLDT_INCLUDE_DIRS})


#Message("Current Souce Dir	:  " ${CMAKE_CURRENT_SOURCE_DIR})
#install(TARGETS TUeLDT DESTINATION install/lib)
