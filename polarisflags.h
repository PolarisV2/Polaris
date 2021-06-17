#ifndef POLARISFLAGS_H
#define POLARISFLAGS_H

// Uncomment this for official releases
// Also don't forget to update the VERSION flag.
#define POLARIS_RELEASE

#include <string>

namespace polaris
{
	static const bool ENABLE_DEBUGGING_TOOLS = true;
	static const bool ENABLE_EXPERIMENTAL_DEBUGGING_TOOLS = false;
	static std::string VERSION = "BETA 3";
}

#endif // POLARISFLAGS_H