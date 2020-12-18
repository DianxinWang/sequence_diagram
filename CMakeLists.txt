cmake_minimum_required(VERSION 3.17)
project(sequence_diagram_drawer)

set(CMAKE_CXX_STANDARD 20)
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.75.0 COMPONENTS unit_test_framework)
if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()
include_directories(.)
add_executable(sequence_diagram_drawer main.cpp)
target_link_libraries(sequence_diagram_drawer ${Boost_LIBRARIES})


# Tests
add_executable(test_syntax test/test_syntax.cpp keyword_factory.h)
target_link_libraries(test_syntax ${Boost_LIBRARIES})
