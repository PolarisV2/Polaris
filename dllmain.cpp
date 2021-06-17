#include <windows.h>
#include <stdio.h>
#include <cstdio>

#include "polarisflags.h"
#include "renderer.h"
#include "console.h"
#include "athena.h"
#include "util.h"

DWORD WINAPI Main(LPVOID lpParam)
{
	// If MinHook failed to properly initialize, prompt the user to restart the game.
	if (MH_Initialize() != MH_OK)
	{
		polaris::Util::ThrowFatalError(L"Failed to initialize MinHook.");
	}

	// Initialize modules.
	new polaris::Console;
	new polaris::Renderer;
	new polaris::Athena;

	return FALSE;
}

// The entry point of the DLL. Called when the DLL is injected.
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, Main, hModule, 0, 0);
		break;
	}

	return TRUE;
}
