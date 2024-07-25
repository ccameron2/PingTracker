#pragma once
#include "imgui.h"

class AppColours
{
public:
	AppColours();

	ImVec4 GetColours() const { return mCustomColour; };
private:
	void SetColours();
	ImVec4 mCustomColour = { 0.95f, 0.6f, 0.2f, 0.8f };
	ImVec4 mCustomBackgroundColour = { 0.19f, 0.19f, 0.18f, 0.9f };
	ImVec4 mCustomColourFull = { mCustomColour.x, mCustomColour.y, mCustomColour.z, 1 };
	ImVec4 mCustomColourDim = { mCustomColour.x, mCustomColour.y, mCustomColour.z, 0.6 };
	ImVec4 mCustomColourDimmer = { mCustomColour.x, mCustomColour.y, mCustomColour.z, 0.4 };
};

