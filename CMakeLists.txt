cmake_minimum_required(VERSION 3.26)
project(MinitelViewer)

set(CMAKE_CXX_STANDARD 17)

add_executable(MinitelViewer main.cpp
        Screen/Screen.cpp
        Screen/Screen.h
        Screen/ScreenCell.cpp
        Screen/ScreenCell.h
        Screen/Font.cpp
        Screen/Font.h
        Screen/MosaicGlyph.cpp
        Screen/MosaicGlyph.h)
target_link_libraries(MinitelViewer sfml-graphics sfml-window sfml-system)
