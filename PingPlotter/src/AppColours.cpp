#include "AppColours.h"

#include <fstream>
#include <iostream>

AppColours::AppColours(UIColour inColour)
{
    UIColour finalColour = UIColour::Orange;
    SetStyle(inColour);
}

void AppColours::SetStyle(UIColour colour)
{
    std::string colourString = "Orange";
    mCurrentColour = colour;

    switch(colour)
    {
        case UIColour::Orange:
            mCustomColour = { 0.95f, 0.6f, 0.2f, 0.8f };
            colourString = "Orange";
            break;
        case UIColour::Purple:
            mCustomColour = { 0.7f, 0.4f, 0.7f, 0.8f };
            colourString = "Purple";
            break;
        case UIColour::Red:
            mCustomColour = { 0.95f, 0.3f, 0.3f, 0.8f };
            colourString = "Red";
            break;
        case UIColour::Green:
            mCustomColour = { 0.4f, 0.8f, 0.4f, 0.8f };
            colourString = "Green";
            break;
        case UIColour::Blue:
            mCustomColour = { 0.3f, 0.5f, 0.95f, 0.8f };
            colourString = "Blue";
            break;
        case UIColour::Yellow:
            mCustomColour = { 0.95f, 0.9f, 0.4f, 0.8f };
            colourString = "Yellow";
            break;
        case UIColour::Grey:
            mCustomColour = { 0.7f, 0.7f, 0.7f, 0.8f };
            colourString = "Grey";
            break;
        case UIColour::Pink:
            mCustomColour = { 0.95f, 0.6f, 0.75f, 0.8f };
            colourString = "Pink";
            break;
    }

    mCustomColourFull = { mCustomColour.x, mCustomColour.y, mCustomColour.z, 1 };
    mCustomColourDim = { mCustomColour.x, mCustomColour.y, mCustomColour.z, 0.6 };
    mCustomColourDimmer = { mCustomColour.x, mCustomColour.y, mCustomColour.z, 0.4 };

    mCurrentColourName = mColourNames[static_cast<int>(mCurrentColour)];
    SetColours();
}

std::string AppColours::RenderColourPicker()
{
    std::string returnColour;
    mCurrentColourName = mColourNames[static_cast<int>(mCurrentColour)];

    if (ImGui::BeginCombo("Colour", mCurrentColourName))
    {
        for (int i = 0; i < IM_ARRAYSIZE(mColourNames); i++)
        {
            bool isSelected = (mCurrentColourName == mColourNames[i]);
            if (ImGui::Selectable(mColourNames[i], isSelected))
            {
                mCurrentColour = static_cast<UIColour>(i);
                SetStyle(mCurrentColour);
                returnColour = mColourNames[i];
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
        return returnColour;
    }
    return mCurrentColourName;
}

UIColour AppColours::GetColourFromString(std::string inputString)
{
    UIColour colour = UIColour::Orange;
    if (inputString == "Orange")
        colour = UIColour::Orange;

    if (inputString == "Purple")
        colour = UIColour::Purple;

    if (inputString == "Red")
        colour = UIColour::Red;

    if (inputString == "Green")
        colour = UIColour::Green;

    if (inputString == "Blue")
        colour = UIColour::Blue;

    if (inputString == "Yellow")
        colour = UIColour::Yellow;

    if (inputString == "Grey")
        colour = UIColour::Grey;

    if (inputString == "Pink")
        colour = UIColour::Pink;
    return colour;
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
