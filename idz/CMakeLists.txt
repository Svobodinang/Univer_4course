cmake_minimum_required(VERSION 3.8)
project(idz)

add_executable(idz main.cpp)

set_property(TARGET idz PROPERTY CXX_STANDARD 11)


include_directories(${OpenCV_DIRS})

target_link_libraries(idz
        ${OpenCV_LIBS}
        )
