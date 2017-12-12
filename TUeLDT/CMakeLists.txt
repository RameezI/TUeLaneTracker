#Creates a library called "TUeLDT" which includes all source files in directory TUeLDT.
#Any cpp or c file place in this directory will be compiled as a part of the library by cmake.
#Place all headers in the include subdirectory inside the TUeLDT directory
cmake_minimum_required (VERSION 2.8.11)
find_package(OpenCV REQUIRED)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -O1 -std=c++11")
file(GLOB  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c )

add_library(TUeLDT STATIC ${SOURCES})
target_include_directories (TUeLDT PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
					   ${CMAKE_CURRENT_SOURCE_DIR}/../include
					   ${CMAKE_CURRENT_SOURCE_DIR}/../3rdPartyLibs
					   ${CMAKE_CURRENT_SOURCE_DIR}/../3rdPartyLibs/eigen3)

#Message("Current Souce Dir	:  " ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS TUeLDT DESTINATION lib)
