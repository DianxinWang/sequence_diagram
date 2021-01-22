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

include_directories(./include)
set(key_component
        src/key_components/straight_line_with_marker.cpp
        src/key_components/title.cpp
        src/key_components/note.cpp
        )
set(view_component
        include/view_components/rectangle.h
        include/view_components/straight_line.h
        )
add_executable(sequence_diagram_drawer main.cpp src/composer.cpp ${key_component} ${view_component})
target_link_libraries(sequence_diagram_drawer ${Boost_LIBRARIES})


# Tests
add_executable(test_syntax test/test_syntax.cpp include/keyword_factory.h ${key_component})
target_link_libraries(test_syntax ${Boost_LIBRARIES})
