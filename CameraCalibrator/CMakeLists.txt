cmake_minimum_required(VERSION 2.8)
project(CAMERA_CALLIBRATOR )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set (BINARY cameraCalibrator)
add_executable(${BINARY} main.cpp )
target_link_libraries( ${BINARY} ${OpenCV_LIBS} )

File(GLOB YAML_FILES "build/*.yaml")
install(FILES  ${YAML_FILES}  DESTINATION install/ConfigFiles/Camera)
