#pragma once
#include <fstream>
#include <string>

class AppSettings
{
public:
	AppSettings(std::string fileName) : FileName(fileName){};

	virtual void SaveToFile()
	{
		OutFile = std::ofstream(FileName, std::ios::binary | std::ofstream::trunc);
		if(OutFile)
		{
			if (OutFile.is_open())
			{
				OutFile << Width;
				OutFile << "\n";
				OutFile << Height;
				OutFile << "\n";
			}
		}
	};

	int Width = 800;
	int Height = 600;
	//bool Fullscreen = false;
protected:
	virtual void ReadFromFile()
	{
		InFile = std::ifstream(FileName, std::ios::binary);
		std::string input;

		if (InFile)
		{
			InFile >> input;
			Width = std::stoi(input);

			InFile >> input;
			Height = std::stoi(input);
		}
	};

	std::string FileName;
	std::ofstream OutFile;
	std::ifstream InFile;
};

