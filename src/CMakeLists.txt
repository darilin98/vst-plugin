cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

project(vst-plugin
        VERSION 1.0.0
        DESCRIPTION "Polynomial Equalizer"
)

#-------------------------------------------------------------------------------
# Fetch Steinberg VST3 SDK
#-------------------------------------------------------------------------------

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:char8_t->")
set(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES OFF CACHE BOOL "" FORCE)

# Fetch the VST3 SDK
FetchContent_Declare(
        vst3sdk
        GIT_REPOSITORY https://github.com/steinbergmedia/vst3sdk.git
        GIT_TAG master
)
FetchContent_MakeAvailable(vst3sdk)

set(VST3_SDK_PATH ${vst3sdk_SOURCE_DIR})
smtg_enable_vst3_sdk()

#-------------------------------------------------------------------------------
# Fetch FFTW
#-------------------------------------------------------------------------------

if(APPLE)

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "FFTW: build shared libraries" FORCE)
    set(ENABLE_FLOAT ON CACHE BOOL "FFTW: build single-precision (float) library" FORCE)
    set(ENABLE_LONG_DOUBLE OFF CACHE BOOL "FFTW: disable long double" FORCE)
    set(ENABLE_QUAD_PRECISION OFF CACHE BOOL "FFTW: disable quad precision" FORCE)
    set(BUILD_TESTS OFF CACHE BOOL "FFTW: don't build tests" FORCE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "Force PIC for static libraries" FORCE)
    set(DISABLE_FORTRAN ON CACHE BOOL "FFTW: disable Fortran wrappers" FORCE)
    set(ENABLE_THREADS OFF CACHE BOOL "FFTW: disable pthreads" FORCE)
    set(ENABLE_OPENMP OFF CACHE BOOL "FFTW: disable OpenMP" FORCE)

    FetchContent_Declare(
            fftw
            URL http://www.fftw.org/fftw-3.3.10.tar.gz
    )
    FetchContent_MakeAvailable(fftw)

    set(FFTW_INCLUDE_DIRS "${fftw_SOURCE_DIR}/api" "${fftw_SOURCE_DIR}" "${fftw_BINARY_DIR}")

elseif(WIN32)

    FetchContent_Declare(
            fftw
            URL https://fftw.org/pub/fftw/fftw-3.3.5-dll64.zip
    )
    FetchContent_GetProperties(fftw)

    if(NOT fftw_POPULATED)
        FetchContent_Populate(fftw)
    endif()

    set(FFTW_WIN_ROOT ${fftw_SOURCE_DIR})

    set(FFTW_DEF ${FFTW_WIN_ROOT}/libfftw3f-3.def)
    set(FFTW_LIB ${FFTW_WIN_ROOT}/libfftw3f-3.lib)
    set(FFTW_DLL ${FFTW_WIN_ROOT}/libfftw3f-3.dll)

    add_custom_command(
            OUTPUT ${FFTW_LIB}
            COMMAND lib /def:${FFTW_DEF} /out:${FFTW_LIB} /machine:x64
            DEPENDS ${FFTW_DEF}
            COMMENT "Generating MSVC import library for FFTW3f"
    )

    add_custom_target(fftw3f_importlib ALL DEPENDS ${FFTW_LIB})

    add_library(fftw3f SHARED IMPORTED)
    add_dependencies(fftw3f fftw3f_importlib)

    set_target_properties(fftw3f PROPERTIES
            IMPORTED_LOCATION "${FFTW_DLL}"
            IMPORTED_IMPLIB   "${FFTW_LIB}"
            INTERFACE_INCLUDE_DIRECTORIES "${FFTW_WIN_ROOT}/"
    )

    set(FFTW_INCLUDE_DIRS "")

endif()

#-------------------------------------------------------------------------------
# Declare sources and ext. library directories
#-------------------------------------------------------------------------------

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
        eqshape.hpp
        presetmenu.hpp
        presetmenu.cpp
)

smtg_add_vst3plugin(vst-plugin ${SOURCES})

target_include_directories(vst-plugin PRIVATE
        ${VST3_SDK_PATH}
        ${VST3_SDK_PATH}/pluginterfaces
        ${VST3_SDK_PATH}/public.sdk/source/vst/
        ${VST3_SDK_PATH}/vstgui4
        ${FFTW_INCLUDE_DIRS}
)


target_link_libraries(vst-plugin PRIVATE sdk
                                         fftw3f
                                         vstgui_support
)

#-------------------------------------------------------------------------------
# [WINDOWS] Copy dynamic fftw next to plugin binary
#-------------------------------------------------------------------------------

if(WIN32)
    add_dependencies(vst-plugin fftw3f_importlib)

    add_custom_command(TARGET vst-plugin PRE_LINK
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:vst-plugin>"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FFTW_DLL} "$<TARGET_FILE_DIR:vst-plugin>"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${FFTW_DLL}" "${CMAKE_BINARY_DIR}/bin"
    )
endif()

#-------------------------------------------------------------------------------
# Configure plugin output format
#-------------------------------------------------------------------------------

smtg_target_configure_version_file(vst-plugin)

# Specify the plugin format, version, etc.
if (APPLE)
    smtg_target_set_bundle(vst-plugin
            BUNDLE_IDENTIFIER "github.com/darilin98/vst-plugin"
            COMPANY_NAME "Darek Rudiš"
    )
elseif (WIN32)
    target_sources(vst-plugin
            PRIVATE
            win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT vst-plugin)

        smtg_target_set_debug_executable(vst-plugin
                "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
                "--pluginfolder \"$(OutDir)/\""
        )
    endif()

endif()

#-------------------------------------------------------------------------------
# Auto-copy view files
#-------------------------------------------------------------------------------

smtg_target_add_plugin_resources(vst-plugin
        RESOURCES
        "Off_LED.png"
        "on-off_LED.png"
        "viewGUI.uidesc"
)
