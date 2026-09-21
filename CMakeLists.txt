cmake_minimum_required(VERSION 3.20)

project(order-pp
    VERSION 1.0.0
    DESCRIPTION "The Order Metalanguage for C Preprocessor Metaprogramming"
    HOMEPAGE_URL "https://github.com/bivex/order-pp-x"
    LANGUAGES C CXX
)

# Header-only interface library
add_library(order INTERFACE)
add_library(order::order ALIAS order)

target_include_directories(order INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Dependency: Chaos Preprocessor
find_package(chaos 1.0 CONFIG QUIET)
if(NOT TARGET chaos::chaos)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../chaos-pp-x")
        target_include_directories(order INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../chaos-pp-x>
        )
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../chaos-pp")
        target_include_directories(order INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../chaos-pp>
        )
    else()
        message(FATAL_ERROR "Chaos Preprocessor library not found! Please place chaos-pp-x next to order-pp-x or install chaos.")
    endif()
else()
    target_link_libraries(order INTERFACE chaos::chaos)
endif()

include(CMakeDependentOption)
option(ORDER_ENABLE_TESTING "Build Order Metalanguage test suite" ${PROJECT_IS_TOP_LEVEL})
option(ORDER_ENABLE_EXAMPLES "Build Order Metalanguage example programs" ${PROJECT_IS_TOP_LEVEL})
option(ORDER_INSTALL "Generate install targets for Order Metalanguage" ${PROJECT_IS_TOP_LEVEL})

if(ORDER_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

if(ORDER_ENABLE_EXAMPLES)
    add_subdirectory(example)
endif()

if(ORDER_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(TARGETS order
        EXPORT orderTargets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY inc/order
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
            PATTERN "*.h"
    )

    install(EXPORT orderTargets
        FILE orderTargets.cmake
        NAMESPACE order::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/order
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/orderConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/orderConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/orderConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/order
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/orderConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/orderConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/order
    )
endif()
