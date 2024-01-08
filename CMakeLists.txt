cmake_minimum_required(VERSION 3.26)
project(MinitelViewer)

set(CMAKE_CXX_STANDARD 20)

add_executable(MinitelViewer main.cpp
        Screen/Screen.cpp
        Screen/Screen.h
        Screen/ScreenCell.cpp
        Screen/ScreenCell.h
        Screen/Font.cpp
        Screen/Font.h
        Screen/MosaicGlyph.cpp
        Screen/MosaicGlyph.h
        minitel_core/Minitel.cpp
        minitel_core/Minitel.h
        minitel_core_interfaces/Constants.h
        minitel_core/Glyph.h
        minitel_core_interfaces/ScreenInterface.h
        minitel_core/Functionalities.cpp
        minitel_core_interfaces/ComInterface.h
        Com/TestCom.h
        Com/TCPCom.h
        minitel_core_interfaces/mtlc.h)
target_link_libraries(MinitelViewer sfml-graphics sfml-window sfml-network sfml-system)
