#ifndef CONSOLE_H
#define CONSOLE_H

#include "util.h"
#include <string>
#include <iostream>

namespace polaris
{
	class Console
	{
	public:
		static inline const char* sAscii[18] = {
			" ",
			"                      `:+syddmmmdhyo/.",
			"                   `/yddddddddddddddddho-",
			"                 .+hhhhhhhhhyyyhhhhhhhhhhy:`",
			"                /yyyyyyyy+:-....-:/syyyyyyys.",
			"               +yyyyyyo-`           ./syyyyys-",
			"              /ssssss:                .ossssss.       Welcome to Polaris!",
			"             .ssssss:                  `osssss+       They said to think back, but I think we went a bit too far.",
			"       ``````:ssssss.`````              /ssssss",
			"       `/////+oossssoooo+-              /yyssss       %s",
			"        `-////++oosssso/.              `syyyss+",
			"          `-////++ooo+-               .ossssss.",
			"            .:///+++:`              `:ossssss-",
			"              .://:`          ``.-:+oooooss+.",
			"               `-.            /oooooooooo+-",
			"                              /ooooooo+/.",
			"                              :/+//:-.",
			" "
		};

		Console();

		static void Log(const char* msg);
		static void LogError(const char* msg);
		static void LogRaw(const char* msg, int color = 15);
	};
}

#endif // CONSOLE_H
