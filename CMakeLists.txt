# Copyright - 2021 - Jan Christoph Uhde <Jan@UhdeJC.com>
cmake_minimum_required(VERSION 3.14)
project(ext-systemd VERSION 0.0.1 LANGUAGES CXX)

include(FindPkgConfig)
if(FindPkgConfig_FOUND)
    pkg_check_modules(real_systemd REQUIRED IMPORTED_TARGET systemd)
endif()

add_library(ext-systemd src/systemd.cpp)
target_include_directories(ext-systemd PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

if(real_systemd_FOUND)
    #this seems to be broken (at least on my system)
    #message(INFO "##### ${foo_LINK_LIBRARIES}")
    #target_link_libraries(ext-systemd PRIVATE PkgConfig::real_systemd)
    message(NOTICE "found systemd")
    target_link_libraries(ext-systemd PRIVATE systemd)
    target_compile_definitions(ext-systemd PUBLIC EXT_HAS_SYSTEMD=1)
endif()

target_compile_features(ext-systemd INTERFACE cxx_std_14)
add_library(ext::systemd ALIAS ext-systemd)

if(EXTSYSTEMD_EXAMPLES)
    add_subdirectory(examples)
endif()
