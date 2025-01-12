cmake_minimum_required(VERSION 3.5)

project(kaplayer)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

add_executable(kaplayer
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(kaplayer PRIVATE Qt6::Widgets Qt6::Multimedia)

