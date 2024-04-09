//
// Created by Asphox on 04/04/2024.
//

#include "WinAbout.h"
#include "../globals.h"

using namespace IHM;

void WinAbout::update()
{
    bool opened = true;
    ImGui::custom_GlobalBeginPopupModal("About eMinitel",[&opened]()
    {
        std::string version = "eMinitel ";
        version += std::to_string(MAJOR_VERSION);
        version += ".";
        version += std::to_string(MINOR_VERSION);
        version += ".";
        version += BUILD_VERSION;
        version += " (";
        version += BUILD_INFO;
        version += ")";
        version += " ";
        version += BUILD_DATE;
        ImGui::Text(version.c_str());
        ImGui::Separator();
        ImGui::Text("Author: Dylan \"Asphox\" Soton");
        ImGui::Text("");
        ImGui::PushItemWidth(270);
        ImGui::InputText(": link", const_cast<char*>(GITHUB_LINK),sizeof(GITHUB_LINK),ImGuiInputTextFlags_ReadOnly|ImGuiInputTextFlags_AutoSelectAll);
        ImGui::PopItemWidth();
    },&opened, ImGuiWindowFlags_AlwaysAutoResize);
}