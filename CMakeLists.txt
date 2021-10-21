# Copyright - 2021 - Jan Christoph Uhde <Jan@UhdeJC.com>
cmake_minimum_required(VERSION 3.14)
project(ext-systemd VERSION 0.0.1 LANGUAGES CXX)

if (EXT_SYSTEMD_REQUIRED)
	set(systemd_required "REQUIRED")
endif()

find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(real_systemd ${systemd_required} IMPORTED_TARGET systemd)
elseif(EXT_SYSTEMD_REQUIRED)
	message(FATAL_ERROR "pkg-config not available")
endif()

add_library(ext-systemd src/systemd.cpp)
target_include_directories(ext-systemd PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

if(real_systemd_FOUND)
    # DEBUG properties
    execute_process(
        COMMAND "${CMAKE_COMMAND}" --help-property-list
        OUTPUT_VARIABLE props
    )

    string(REPLACE "\n" ";" props "${props}")

    foreach (prop ${props})
        if (prop MATCHES "<CONFIG>$")
            string(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" prop "${prop}")
            string(TOUPPER "${prop}" prop)
        endif()
        get_target_property(value PkgConfig::real_systemd "${prop}")
        if (value)
            message(DEBUG "${prop}=${value}")
        endif()
    endforeach()
    # DEBUG properties - end

    if(NOT TARGET PkgConfig::real_systemd)
        message(FATAL_ERROR "PkgConfig::real_systemd target not created")
    endif()

    message(NOTICE "systemd found")

    #this is a bug -- maybe only on my pc
    get_target_property(import_target PkgConfig::real_systemd INTERFACE_LINK_LIBRARIES>)
    if(${import_target} STREQUAL "import_target-NOTFOUND")
        #this should be the only valid case
        target_link_libraries(ext-systemd PRIVATE systemd)
    else()
        target_link_libraries(ext-systemd PRIVATE PkgConfig::real_systemd)
    endif()

    target_compile_definitions(ext-systemd PUBLIC EXT_SYSTEMD_AVAILABLE=1)
else()
    message(NOTICE "systemd NOT found")
endif()

target_compile_features(ext-systemd INTERFACE cxx_std_14)
add_library(ext::systemd ALIAS ext-systemd)

if(EXT_SYSTEMD_EXAMPLES)
    add_subdirectory(examples)
endif()

