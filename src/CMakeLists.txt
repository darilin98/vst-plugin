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

FetchContent_Declare(
    fftw
    URL http://www.fftw.org/fftw-3.3.10.tar.gz
)
FetchContent_MakeAvailable(fftw)

set(ENABLE_STATIC ON CACHE BOOL "Build static FFTW")
set(ENABLE_SHARED OFF CACHE BOOL "Disable shared FFTW")

set(ENABLE_FLOAT ON CACHE BOOL "Enable single precision (float) FFTW" FORCE)
set(ENABLE_DOUBLE OFF CACHE BOOL "Disable double precision FFTW" FORCE)
set(ENABLE_LONG_DOUBLE OFF CACHE BOOL "Disable long double FFTW" FORCE)
set(ENABLE_SHARED OFF CACHE BOOL "Disable shared FFTW" FORCE)
set(ENABLE_STATIC ON CACHE BOOL "Enable static FFTW" FORCE)

# Ensure FFTW builds
FetchContent_GetProperties(fftw)
if(NOT fftw_POPULATED)
    FetchContent_Populate(fftw)
    add_subdirectory(${fftw_SOURCE_DIR} ${fftw_BINARY_DIR})
endif()

# Set the VST3 SDK base directory
set(VST3_SDK_PATH ${vst3sdk_SOURCE_DIR})

include(${VST3_SDK_PATH}/cmake/modules/SMTG_VstGuiSupport.cmake)

set(SOURCES
        controller.cpp
        controller.hpp
        pluginfactory.cpp
        processor.cpp
        processor.hpp
        fftprocessor.hpp
        fftprocessor.cpp
        equalizer.hpp
        equalizer.cpp
        utils.hpp
        eqcurveview.hpp
        eqcurveview.cpp
        eqconstants.hpp
        paramlistener.cpp

        ${VST3_SDK_PATH}/public.sdk/source/main/macmain.cpp
        ${VST3_SDK_PATH}/public.sdk/source/main/moduleinit.cpp
        eqshape.hpp
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
        ${VST3_SDK_PATH}/vstgui4
        ${fftw_SOURCE_DIR}/api
        ${fftw_SOURCE_DIR}
        ${fftw_BINARY_DIR}
)

target_link_libraries(vst-plugin PRIVATE sdk "-framework CoreFoundation"
                                         fftw3f
                                         vstgui_support
)

target_compile_definitions(vst-plugin PUBLIC $<$<CONFIG:Debug>:VSTGUI_LIVE_EDITING=0>)


# Define the output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Specify the plugin format, version, etc.
set_target_properties(vst-plugin PROPERTIES
        BUNDLE TRUE
        BUNDLE_EXTENSION "vst3"
        MACOSX_BUNDLE TRUE
        OUTPUT_NAME "vst-plugin"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist"
)

set(UIDESC_SOURCE "${CMAKE_SOURCE_DIR}/viewGUI.uidesc")
set(UIDESC_DESTINATION "${CMAKE_BINARY_DIR}/vst-plugin.vst3/Contents/Resources/viewGUI.uidesc")

add_custom_command(
        OUTPUT ${UIDESC_DESTINATION}
        COMMAND ${CMAKE_COMMAND} -E copy ${UIDESC_SOURCE} ${UIDESC_DESTINATION}
        DEPENDS ${UIDESC_SOURCE}
        COMMENT "Copying .uidesc into plugin bundle Resources"
)

add_custom_target(copyUIDESC ALL DEPENDS ${UIDESC_DESTINATION})

add_dependencies(vst-plugin copyUIDESC)

# Create symlink
install(CODE "
  execute_process(COMMAND ln -sf ${CMAKE_BINARY_DIR}/vst-plugin.vst3 \$ENV{HOME}/Library/Audio/Plug-Ins/VST3/vst-plugin.vst3)
  message(STATUS \"Created symlink: \$ENV{HOME}/Library/Audio/Plug-Ins/VST3/vst-plugin.vst3 -> ${CMAKE_BINARY_DIR}/vst-plugin.vst3\")
")
