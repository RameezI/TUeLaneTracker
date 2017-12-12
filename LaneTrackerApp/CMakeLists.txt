# This compiles and links Executable called "TUeLaneTracker"
cmake_minimum_required (VERSION 2.8.11)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -O1 -std=c++11")

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c )
add_executable (TUeLaneTracker ${SOURCES})

# Link the executable to the TUeLDT library. Since the TUeLDT library has
# public include directories we will use those includes when buiding TUeLaneTracker
target_include_directories (TUeLaneTracker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries (TUeLaneTracker LINK_PUBLIC TUeLDT ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS TUeLaneTracker DESTINATION bin)
