cmake_minimum_required(VERSION 3.25)
project(eMinitel)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR C:/SFML/SFML-2.6.1/lib/cmake/SFML)
    set(SFML_INCLUDE_DIR C:/SFML/SFML-2.6.1/include)
    find_package(SFML COMPONENTS system window graphics network main REQUIRED)
    include_directories(${SFML_INCLUDE_DIR})
endif(WIN32)

# No-console in Windows
if(WIN32)
    set(EXECUTABLE_OPTION WIN32)
endif(WIN32)
if(UNIX)
    set(EXECUTABLE_OPTION "" IHM/WinAbout.cpp IHM/WinAbout.h globals.h)
endif(UNIX)

file(GLOB IMGUI_SRC
        "IHM/imgui/*.hpp"
        "IHM/imgui/*.cpp"
)

add_executable(eMinitel ${EXECUTABLE_OPTION}
        main.cpp
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
        minitel_core/VideotexCodes.h
        minitel_core/Glyph.h
        minitel_core/Functionalities.cpp
        Com/TCPCom.h
        minitel_core/Interfaces.h
        Keyboard/Keyboard.cpp
        Keyboard/Keyboard.h
        eMinitel.cpp
        eMinitel.h
        IHM/MainMenuBar.h
        IHM/MainMenuBar.cpp
        IHM/IHM.h
        IHM/WinConnectionTCP.cpp
        IHM/WinConnectionTCP.h
        IHM/IHM.cpp
        ${IMGUI_SRC}

        IHM/WinAbout.cpp globals.h)
if(WIN32)
    target_link_libraries(eMinitel sfml-graphics sfml-window sfml-network sfml-system sfml-main -static-libgcc -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)
endif(WIN32)
if(UNIX)
    target_link_libraries(eMinitel sfml-graphics sfml-window sfml-network sfml-system)
endif(UNIX)
