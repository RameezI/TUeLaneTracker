# This compiles and links Executable called "TUeLaneTracker"
cmake_minimum_required (VERSION 2.8.11)

SET(CMAKE_MODULE_PATH 	 ${CMAKE_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH} )

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost 1.62.0 REQUIRED COMPONENTS program_options )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -O0 -std=c++11")

#add_definitions (-D_GLIBCXX_USE_CXX11_ABI=0)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c )
add_executable (TUeLaneTracker ${SOURCES})

# Link the executable to the TUeLDT library. Since the TUeLDT library has
# public include directories we will use those includes when buiding TUeLaneTracker

target_include_directories (TUeLaneTracker PUBLIC	${Boost_INCLUDE_DIRS})

target_link_libraries 	   (TUeLaneTracker PUBLIC 	TUeLDT 	
							${Boost_LIBRARIES} 
							${OpenCV_LIBS} 
							${CMAKE_THREAD_LIBS_INIT})

install(TARGETS  TUeLaneTracker DESTINATION install)
