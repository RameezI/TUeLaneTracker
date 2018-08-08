#Creates a library called "TUeLDT" which includes all source files in directory TUeLDT.
#Any cpp or c file place in this directory will be compiled as a part of the library by cmake.
#Place all headers in the include subdirectory inside the TUeLDT directory
cmake_minimum_required (VERSION 2.8.11)
find_package(OpenCV REQUIRED)
find_package(S32VSDK)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -O0 -std=c++11")
file(GLOB  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c )

add_library(TUeLDT STATIC ${SOURCES})

set(TUeLDT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
             ${CMAKE_CURRENT_SOURCE_DIR}/../include
             ${CMAKE_CURRENT_SOURCE_DIR}/../3rdPartyLibs)

message("Includes before append: ${TUeLDT_INCLUDE_DIRS}")

IF(S32VSDK_FOUND)
    list(APPEND TUeLDT_INCLUDE_DIRS ${S32VSDK_INCLUDE_DIRS})
    message("Includes after append: ${TUeLDT_INCLUDE_DIRS}")
ENDIF(S32VSDK_FOUND)

target_include_directories (TUeLDT PUBLIC ${TUeLDT_INCLUDE_DIRS})


#Message("Current Souce Dir	:  " ${CMAKE_CURRENT_SOURCE_DIR})
#install(TARGETS TUeLDT DESTINATION install/lib)
