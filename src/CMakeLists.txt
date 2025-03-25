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

set(SOURCES
        controller.cpp
        pluginfactory.cpp
        processor.cpp
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

target_include_directories(vst-plugin PRIVATE
        ${VST3_SDK_PATH}
        ${VST3_SDK_PATH}/pluginterfaces
        ${VST3_SDK_PATH}/public.sdk/source/vst/
)

target_link_libraries(vst-plugin PRIVATE sdk "-framework CoreFoundation")

# Define the output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Specify the plugin format, version, etc.
set_target_properties(vst-plugin PROPERTIES
        BUNDLE TRUE
        BUNDLE_EXTENSION "vst3"
        MACOSX_BUNDLE TRUE
        PREFIX ""
        # SUFFIX ".vst3"
        OUTPUT_NAME "vst-plugin"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist"
)

# Install the plugin to the standard VST3 path
install(TARGETS vst-plugin
        BUNDLE DESTINATION "$ENV{HOME}/Library/Audio/Plug-Ins/VST3"
        LIBRARY DESTINATION "$ENV{HOME}/Library/Audio/Plug-Ins/VST3"
)