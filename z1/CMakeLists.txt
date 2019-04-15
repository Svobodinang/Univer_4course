cmake_minimum_required(VERSION 3.8)
project(z1) 

add_executable(z1 z1.cpp)

include_directories(${OpenCV_DIRS})

target_link_libraries(z1
        ${OpenCV_LIBS}
        )
