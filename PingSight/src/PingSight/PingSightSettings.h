#pragma once
#include <fstream>
#include "App/AppSettings.h"

class PingSightSettings : public AppSettings
{
public:
	PingSightSettings(std::string fileName) : AppSettings(fileName)
	{
		ReadFromFile();
	}

	void ReadFromFile() override
	{
		AppSettings::ReadFromFile();

		if (InFile)
		{
			if (InFile.is_open())
			{
				std::string input;

				InFile >> input;
				ShowAllData = std::stoi(input);

				InFile >> input;
				ShowControls = std::stoi(input);

				InFile >> input;
				Interval = std::stoi(input);

				InFile >> input;
				DataViewRange = std::stoi(input);
			}
			InFile.close();
		}
	}

	void SaveToFile() override
	{
		AppSettings::SaveToFile();

		if (OutFile)
		{
			if (OutFile.is_open())
			{
				OutFile << ShowAllData;
				OutFile << "\n";
				OutFile << ShowControls;
				OutFile << "\n";
				OutFile << Interval;
				OutFile << "\n";
				OutFile << DataViewRange;
				OutFile << "\n";
				OutFile.close();
			}
		}
	}

	bool ShowAllData = false;
	bool ShowControls = false;
	int Interval = 5;
	int DataViewRange = 1500;
};
