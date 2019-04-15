cmake_minimum_required(VERSION 3.8)
project(z2)

add_executable(z2 z2.cpp)

set_property(TARGET z2 PROPERTY CXX_STANDARD 11)


include_directories(${OpenCV_DIRS})

target_link_libraries(z2
        ${OpenCV_LIBS}
        )
