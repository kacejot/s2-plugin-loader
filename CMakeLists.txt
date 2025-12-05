cmake_minimum_required(VERSION 3.24)

project(pluginloader VERSION 1.0.6)

add_library(_pluginloader_base INTERFACE)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

target_compile_features(_pluginloader_base INTERFACE cxx_std_20)
set_target_properties(_pluginloader_base PROPERTIES
    COMPILE_WARNING_AS_ERROR True
    INTERPROCEDURAL_OPTIMIZATION True
)

if(MSVC)
    target_compile_options(_pluginloader_base INTERFACE /W4)
else()
    target_compile_options(_pluginloader_base INTERFACE -Wall -Wextra -Wpedantic)
endif()
# CMake doesn't understand warnings as errors for MinGW yet
if(MINGW)
    target_compile_options(_pluginloader_base INTERFACE -Werror)
endif()

set(CONFIGURE_FILES_DIR "${CMAKE_CURRENT_BINARY_DIR}/configure")

configure_file(
    "src/version.inl.in"
    "${CONFIGURE_FILES_DIR}/version.inl"
)

set(GIT_PRE_CONFIGURE_FILE "src/git.inl.in")
set(GIT_POST_CONFIGURE_FILE "${CONFIGURE_FILES_DIR}/git.inl")
include(common_cmake/git_watcher.cmake)

# Note not recursive, only top level - we specify proxy explicitly
file(GLOB sources CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
target_sources(_pluginloader_base PUBLIC
    ${sources}
    ${GIT_POST_CONFIGURE_FILE}
    "src/proxy/proxy.h"
    "src/versioninfo.rc"
)
target_include_directories(_pluginloader_base INTERFACE "src" ${CONFIGURE_FILES_DIR})
target_precompile_headers(_pluginloader_base INTERFACE "src/pch.h")

set(PLUGINLOADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PLUGINLOADER_SOURCE_DIR ${PLUGINLOADER_SOURCE_DIR} PARENT_SCOPE)

function(pluginloader_add name role)
    # role:
    #   LOADER_ROLE     -> Proxy DLL Loader
    #   PLUGIN_ROLE     -> DLL with payload
    #   STANDALONE_ROLE -> proxy + payload - loader

    if (NOT role STREQUAL "PLUGIN_ROLE")
        set(SEARCH_PATH "${PLUGINLOADER_SOURCE_DIR}/src/proxy/${name}.*")
		file(GLOB role_sources "${SEARCH_PATH}")
    endif()

    add_library(pluginloader_${name} SHARED ${role_sources})
    target_link_libraries(pluginloader_${name} PUBLIC _pluginloader_base)
    set_target_properties(pluginloader_${name} PROPERTIES
        PREFIX ""
        OUTPUT_NAME ${name}
        SUFFIX ".dll"
    )
	
	target_compile_definitions(pluginloader_${name} PUBLIC ${role})
endfunction()

# ==================================================================================================

if (CMAKE_SOURCE_DIR STREQUAL PLUGINLOADER_SOURCE_DIR)
    pluginloader_add(no_proxy LOADER_ROLE)
    pluginloader_add(d3d11 LOADER_ROLE)
    pluginloader_add(dsound LOADER_ROLE)
    pluginloader_add(winmm LOADER_ROLE)
    pluginloader_add(xinput1_3 LOADER_ROLE)

    install(
    TARGETS
        pluginloader_no_proxy
        pluginloader_d3d11
        pluginloader_dsound
        pluginloader_winmm
        pluginloader_xinput1_3
    RUNTIME DESTINATION .
)
else()
    message(STATUS "[pluginloader] Loaded as submodule — no default targets")
endif()
