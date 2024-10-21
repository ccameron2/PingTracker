#pragma once
#include <string>

#include "imgui.h"

enum class UIColour
{
	Orange,
	Purple,
	Red,
	Green,
	Blue,
	Yellow,
	Grey,
	Pink
};

class AppColours
{
public:
	AppColours(UIColour inColour);


	ImVec4 GetColour() const { return mCustomColour; }

	void SetStyle(UIColour colour);
	std::string RenderColourPicker();

	static UIColour GetColourFromString(std::string inputString);

private:
	void SetColours();

	UIColour mCurrentColour;
	const char* mColourNames[8] = { "Orange", "Purple", "Red", "Green", "Blue", "Yellow", "Grey", "Pink" };
	const char* mCurrentColourName = "Default";

	std::string mAppDataPath = "";

	ImVec4 mCustomColour = { 0.95f, 0.6f, 0.2f, 0.8f };
	ImVec4 mCustomBackgroundColour = { 0.19f, 0.19f, 0.18f, 0.9f };
	ImVec4 mCustomColourFull = { mCustomColour.x, mCustomColour.y, mCustomColour.z, 1.0f };
	ImVec4 mCustomColourDim = { mCustomColour.x, mCustomColour.y, mCustomColour.z, 0.6f };
	ImVec4 mCustomColourDimmer = { mCustomColour.x, mCustomColour.y, mCustomColour.z, 0.4f };
};

