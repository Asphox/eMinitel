cmake_minimum_required(VERSION 3.26)
project(MinitelViewer)

set(CMAKE_CXX_STANDARD 17)

add_executable(MinitelViewer main.cpp
        MinitelScreen.cpp
        MinitelScreen.h
        MinitelScreenCell.cpp
        MinitelScreenCell.h
        MinitelFont.cpp
        MinitelFont.h
        MinitelCharacters.h)
target_link_libraries(MinitelViewer sfml-graphics sfml-window sfml-system)
