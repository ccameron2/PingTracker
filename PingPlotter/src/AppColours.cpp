#include "AppColours.h"


AppColours::AppColours()
{
    SetColours();
}

void AppColours::SetColours()
{
    auto& colors = ImGui::GetStyle().Colors;
    //colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    colors[ImGuiCol_Header] = mCustomColour;
    colors[ImGuiCol_HeaderHovered] = mCustomColourFull;
    colors[ImGuiCol_HeaderActive] = mCustomColourFull;

    colors[ImGuiCol_Button] = mCustomColour;
    colors[ImGuiCol_ButtonHovered] = mCustomColourFull;
    colors[ImGuiCol_ButtonActive] = mCustomColourFull;

    colors[ImGuiCol_FrameBg] = mCustomBackgroundColour;
    colors[ImGuiCol_FrameBgHovered] = mCustomBackgroundColour;
    colors[ImGuiCol_FrameBgActive] = mCustomBackgroundColour;

    colors[ImGuiCol_Tab] = mCustomColour;
    colors[ImGuiCol_TabHovered] = mCustomColourFull;
    colors[ImGuiCol_TabActive] = mCustomColourFull;
    colors[ImGuiCol_TabUnfocused] = mCustomColourDimmer;
    colors[ImGuiCol_TabUnfocusedActive] = mCustomColourDim;

    colors[ImGuiCol_TitleBg] = mCustomColour;
    colors[ImGuiCol_TitleBgActive] = mCustomColourFull;
    colors[ImGuiCol_TitleBgCollapsed] = mCustomColourFull;

    colors[ImGuiCol_DockingPreview] = mCustomColour;
    colors[ImGuiCol_DockingEmptyBg] = mCustomColour;

    colors[ImGuiCol_CheckMark] = mCustomColour;
    colors[ImGuiCol_SliderGrab] = mCustomColour;
    colors[ImGuiCol_SliderGrabActive] = mCustomColourFull;

    colors[ImGuiCol_ResizeGrip] = mCustomColour;
    colors[ImGuiCol_ResizeGripActive] = mCustomColourFull;
    colors[ImGuiCol_ResizeGripHovered] = mCustomColourFull;

    colors[ImGuiCol_SeparatorActive] = mCustomColourFull;
    colors[ImGuiCol_SeparatorHovered] = mCustomColour;

    colors[ImGuiCol_TextLink] = mCustomColour;
    colors[ImGuiCol_TextSelectedBg] = mCustomColourFull;

    colors[ImGuiCol_PlotLines] = mCustomColour;
    colors[ImGuiCol_PlotLines] = mCustomColourFull;

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 4.0f;

}
