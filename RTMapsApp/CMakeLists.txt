# This compiles and links Executable called "RTMapsApp"
cmake_minimum_required (VERSION 2.8.11)

SET(CMAKE_MODULE_PATH 	 ${CMAKE_SOURCE_DIR}/cmake/Modules {CMAKE_MODULE_PATH} )

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost 1.62.0 REQUIRED COMPONENTS program_options )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -O0 -std=c++11")

# Package Name
set(PCK RTMapsPack)
project("${PCK}.u")
set(RTMAPS_SDKDIR "/opt/rtmaps")

if (NOT DEFINED RTMAPS_SDKDIR)
    if (DEFINED ENV{RTMAPS_SDKDIR})
        set(RTMAPS_SDKDIR "$ENV{RTMAPS_SDKDIR}" CACHE PATH "")  # Use the default RTMaps installation
    else()
        message(FATAL_ERROR "RTMAPS_SDKDIR not defined. Either pass -D\"RTMAPS_SDKDIR=...\" to CMake or define an RTMAPS_SDKDIR environment variable")
    endif()
endif()

# IMPORTANT: Include the RTMaps CMake configuration file
include("${RTMAPS_SDKDIR}/templates.u/rtmaps.cmake")

add_rtmaps_package(${PCK} PCKINFO "${PCK}.pckinfo"
    # add sources, headers and/or directories
    # NB: if you add and/or remove files to a directory, you must re-generate the CMake project

    "local_interfaces"                    # NB: if you add and/or remove files to a directory, you must re-generate the CMake project
    "src"                                 # NB: if you add and/or remove files to a directory, you must re-generate the CMake project
	"../TUeLDT"
	"../TUeLDT/include"
	"../3rdPartyLibs/Eigen"
	"../3rdPartyLibs/gtest"
	"../3rdPartyLibs/gmock"
    "../include"
    # other examples
    # "src_generated/some_file.hpp"
    # "src_generated/another_file.cpp"
    # "../external_directory"             # NB: if you add and/or remove files to a directory, you must re-generate the CMake project
)
#add_definitions (-D_GLIBCXX_USE_CXX11_ABI=0)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c )
#add_executable (RTMapsPack ${SOURCES})

# Link the executable to the TUeLDT library. Since the TUeLDT library has
# public include directories we will use those includes when buiding TUeLaneTracker

target_include_directories (${PCK} PUBLIC	${Boost_INCLUDE_DIRS})

target_link_libraries 	   (${PCK} TUeLDT 	
							${Boost_LIBRARIES} 
							${OpenCV_LIBS} 
							${CMAKE_THREAD_LIBS_INIT})

#install(TARGETS  RTMapsApp DESTINATION install-RTMaps)


# HIGHLY RECOMMENDED: Append useful compiler flags
if (MSVC)
    # MSVC https://msdn.microsoft.com/en-us/library/thxezb7y.aspx
    target_compile_options(${PCK} PRIVATE /W4 /WX)
else()
    # Gcc   https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
    # Clang http://clang.llvm.org/docs/UsersManual.html
    target_compile_options(${PCK} PRIVATE -fdiagnostics-show-option -Wall -Wextra -pedantic -Werror)
endif()


install(TARGETS  ${PCK} DESTINATION install)