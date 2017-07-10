# This compiles and links Executable called "TUeLaneTracker"
find_package(OpenCV REQUIRED) 
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c )
add_executable (TUeLaneTracker ${SOURCES})

# Link the executable to the TUeLDT library. Since the TUeLDT library has
# public include directories we will use those includes when buiding TUeLaneTracker
target_link_libraries (TUeLaneTracker LINK_PUBLIC TUeLDT ${OpenCV_LIBS})
install(TARGETS TUeLaneTracker DESTINATION bin)
