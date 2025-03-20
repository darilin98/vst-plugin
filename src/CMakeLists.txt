cmake_minimum_required(VERSION 3.29)
project(vst-plugin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch the VST3 SDK
FetchContent_Declare(
        vst3sdk
        GIT_REPOSITORY https://github.com/steinbergmedia/vst3sdk.git
        GIT_TAG master
)
FetchContent_MakeAvailable(vst3sdk)

# Set the VST3 SDK base directory
set(VST3_SDK_PATH ${vst3sdk_SOURCE_DIR})

include_directories(vst-plugin PRIVATE
        ${VST3_SDK_PATH}
        ${VST3_SDK_PATH}/pluginterfaces
        ${VST3_SDK_PATH}/public.sdk/source/vst/
)

set(SOURCES
    main.cpp
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DNDEBUG)
else()
    add_definitions(-DDEVELOPMENT)  # Default to DEVELOPMENT if not Debug/Release
endif()

# Define target
add_library(vst-plugin MODULE ${SOURCES})

# Define the output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Specify the plugin format, version, etc.
set_target_properties(vst-plugin PROPERTIES
        LIBRARY_OUTPUT_NAME "vst-plugin"
        SUFFIX ".vst3"
        BUILD_WITH_INSTALL_RPATH ON
)