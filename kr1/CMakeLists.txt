cmake_minimum_required(VERSION 3.8)
project(kr1)

add_executable(kr1 kr1.cpp)

set_property(TARGET kr1 PROPERTY CXX_STANDARD 11)


include_directories(${OpenCV_DIRS})

target_link_libraries(kr1
        ${OpenCV_LIBS}
        )
