#pragma once
#include <fstream>
#include <string>

#include "AppColours.h"

class AppSettings
{
public:
	AppSettings(std::string fileName) : FileName(fileName), mAppColours(UIColour::Orange){};

	virtual void SaveToFile()
	{
		OutFile = std::ofstream(FileName, std::ios::binary | std::ofstream::trunc);
		if(OutFile)
		{
			if (OutFile.is_open())
			{
				OutFile << Colour;
				OutFile << "\n";
				OutFile << Width;
				OutFile << "\n";
				OutFile << Height;
				OutFile << "\n";
			}
		}
	};

	virtual void RenderColourPicker()
	{
		Colour = mAppColours.RenderColourPicker();
	}

	virtual ImVec4 GetCurrentColourRGBA() { return mAppColours.GetColour(); }

	std::string Colour = "Orange";
	int Width = 1280;
	int Height = 720;
	//bool Fullscreen = false;
protected:
	virtual void ReadFromFile()
	{
		InFile = std::ifstream(FileName, std::ios::binary);
		std::string input;

		if (InFile.is_open())
		{
			InFile >> input;

			Colour = input;
			mAppColours.SetStyle(AppColours::GetColourFromString(input));

			InFile >> input;
			Width = std::stoi(input);

			InFile >> input;
			Height = std::stoi(input);
		}
	};

	std::string FileName;
	std::ofstream OutFile;
	std::ifstream InFile;
	AppColours mAppColours;
};

