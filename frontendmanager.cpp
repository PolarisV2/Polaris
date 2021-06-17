/*
 * [irma] This class is unfinished, but I plan to have a system here that loads the main Polaris behaviour when the player readies up
*/

#include "frontendmanager.h"
#include "console.h"
#include "util.h"

#include "SDK.hpp"

namespace polaris 
{
    PVOID(*ProcessEventB)(SDK::UObject*, SDK::UFunction*, PVOID) = nullptr;
    PVOID ProcessEventHookB(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
    {
        if (pObject && pFunction)
        {
            if (pFunction->GetName().find("ServerReadyUp") != std::string::npos)
            {
                Console::Log("Player readied up");
            }
        }

        return ProcessEventB(pObject, pFunction, pParams);
    }

	FrontendManager::FrontendManager() 
	{
		Console::Log("Starting FrontendManager");

        auto pProcessEventAddress = Util::FindPattern("\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x6C\x24\x00\x48\x89\x9D\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC5\x48\x89\x85\x00\x00\x00\x00\x48\x63\x41\x0C", "xxxxxxxxxxxxxxx????xxxx?xxx????xxx????xxxxxx????xxxx");
        if (!pProcessEventAddress)
        {
            MessageBox(NULL, static_cast<LPCWSTR>(L"Finding pattern for ProcessEvent has failed, please re-open Fortnite and try again!"), static_cast<LPCWSTR>(L"Error"), MB_ICONERROR);
            ExitProcess(EXIT_FAILURE);
        }

        MH_CreateHook(static_cast<LPVOID>(pProcessEventAddress), ProcessEventHookB, reinterpret_cast<LPVOID*>(&ProcessEventB));
        MH_EnableHook(static_cast<LPVOID>(pProcessEventAddress));
	}

	FrontendManager::~FrontendManager()
	{

	}
}
