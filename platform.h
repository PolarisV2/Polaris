#ifndef PLATFORM_H
#define PLATFORM_H

#include <windows.h>
#include <string>

#include "util.h"

namespace polaris
{
    class Platform
    {
    public:
        static std::string GetFontsDir()
        {
            char WinDir[MAX_PATH];
            if (!GetWindowsDirectoryA(WinDir, sizeof(WinDir)))
            {
                Util::ThrowFatalError(L"An unknown error occured.");
            }

            return Util::GetConcatPath(WinDir, "Fonts");
        }
    };
}

#endif // PLATFORM_H