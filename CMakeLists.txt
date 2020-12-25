cmake_minimum_required(VERSION 3.18)
project(nmea
        VERSION 0.1.0
        DESCRIPTION "nmea parser"
        HOMEPAGE_URL https://github.com/mathisloge/nmea
        LANGUAGES CXX)


option(BUILD_TESTING "enables testing" ON)

find_package(Boost REQUIRED)

add_library(nmea INTERFACE)
add_library(nmea::nmea ALIAS nmea)
target_include_directories(nmea INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(nmea INTERFACE Boost::headers)

enable_testing()
add_subdirectory(tests)


install(DIRECTORY include/ TYPE INCLUDE)
install(TARGETS nmea
    EXPORT nmeaTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(EXPORT nmeaTargets
    FILE nmeaTargets.cmake
    NAMESPACE nmea::
    DESTINATION share/nmea/cmake
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/nmeaConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/nmeaConfig.cmake"
    INSTALL_DESTINATION "share/nmea/cmake"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/nmeaConfigVersion.cmake"
    VERSION "${nmea_VERSION_MAJOR}.${nmea_VERSION_MINOR}.${nmea_VERSION_PATCH}"
    COMPATIBILITY SameMajorVersion
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/nmeaConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/nmeaConfigVersion.cmake
        DESTINATION share/nmea/cmake
)


export(EXPORT nmeaTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}nmeaTargets.cmake"
)


set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_VERSION_MAJOR ${nmea_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${nmea_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${nmea_VERSION_PATCH})
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
    /.git
    /dist
    /.vscode
    /.*build.*
    /\\\\.DS_Store
)
include(CPack)

add_custom_target(dist
    COMMAND "${CMAKE_COMMAND}"
      --build "${CMAKE_BINARY_DIR}"
      --target package_source
    VERBATIM
    USES_TERMINAL
)