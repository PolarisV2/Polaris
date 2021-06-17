#include "console.h"
#include "polarisflags.h"

#include <iostream>

HANDLE hConsole;

namespace polaris
{
	Console::Console()
	{
		Util::InitConsole();

#ifdef POLARIS_RELEASE
		std::string versionString = VERSION;
#else
		std::string versionString = "Pre-release build, do not redistribute!";
#endif

		// Draw fancy thing :D
		for (int i = 0; i < 18; i++)
		{
			char buffer[2000];
			sprintf_s(buffer, sizeof(buffer), Console::sAscii[i], versionString.c_str());

			polaris::Console::LogRaw(buffer, 11);
		}
	}

	// Log a message to console
	void Console::Log(const char* msg)
	{
		if(!hConsole)
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// Make the console text cyan to differentiate Polaris from Fortnite logs
		SetConsoleTextAttribute(hConsole, 11);

		std::cout << "LogPolaris:";

		// Make the console text white again
		SetConsoleTextAttribute(hConsole, 15);

		std::cout << "Display: " << msg << std::endl;
	}

	// Log an error to console.
	void Console::LogError(const char* msg)
	{
		if (!hConsole)
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// Make the console text cyan to differentiate Polaris from Fortnite logs
		SetConsoleTextAttribute(hConsole, 11);

		std::cout << "LogPolaris:";

		// Make the console text red, indicating an error.
		SetConsoleTextAttribute(hConsole, 12);

		std::cout << "Error: " << msg << std::endl;

		// Make the console text white again
		SetConsoleTextAttribute(hConsole, 15);
	}

	// Log a message to the console without the Polaris prefix.
	void Console::LogRaw(const char* msg, int color)
	{
		if (!hConsole)
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// Make the console text any color specified by `color`
		SetConsoleTextAttribute(hConsole, color);

		std::cout << msg << std::endl;

		// Make the console text white again
		SetConsoleTextAttribute(hConsole, 15);
	}
}
