if(NOT BUILD_TESTING) 
    return()
endif()

find_package(Catch2 REQUIRED)

include(CTest)

add_executable(nmea-test
    src/main.cpp
)
set_property(TARGET nmea-test PROPERTY CXX_STANDARD 20)
target_link_libraries(nmea-test Catch2::Catch2 nmea::nmea)

include(Catch)
catch_discover_tests(nmea-test)
