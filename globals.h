//
// Created by Asphox on 04/04/2024.
//

#ifndef MINITELVIEWER_GLOBALS_H
#define MINITELVIEWER_GLOBALS_H

#include <cstdint>
#include <string>

constexpr std::size_t MAJOR_VERSION = 0;
constexpr std::size_t MINOR_VERSION = 2;
constexpr const char* BUILD_VERSION = "a";
constexpr const char* BUILD_DATE = __DATE__;
constexpr const char* BUILD_INFO =
#ifdef WIN32
        "Windows 64 bits"
#else
        "Windows 64 bits"
#endif
#ifdef UNIX
        "Linux 64 bits"
#endif
;
constexpr const char GITHUB_LINK [] = "https://github.com/Asphox/eMinitel\0";

#endif //MINITELVIEWER_GLOBALS_H
