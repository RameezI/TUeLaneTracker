# MakeLists files in this project can refer to the 
# root source directory of the project as ${TUeLaneTracker_SOURCE_DIR} and
# to the root binary directory of the project as ${TUeLaneTracker_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (TUeLaneTracker C CXX)

SET(CMAKE_CXX_FLAGS 	"${CMAKE_CXX_FLAGS} -g -Wall -O0 -std=c++11")

# Recurse into the "TUeLDT" and "LaneTrackerApp" subdirectories.
set(CMAKE_INSTALL_PREFIX ${TUeLaneTracker_SOURCE_DIR})

add_subdirectory (TUeLDT ${TUeLaneTracker_SOURCE_DIR}/TUeLDT/build)
add_subdirectory (LaneTrackerApp ${TUeLaneTracker_SOURCE_DIR}/LaneTrackerApp/build)

install(DIRECTORY ${TUeLaneTracker_SOURCE_DIR}/Templates DESTINATION install/ConfigFiles) 

