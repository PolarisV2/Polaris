#ifndef UTIL_H
#define UTIL_H

#include <inttypes.h>
#include <Windows.h>
#include <psapi.h>
#include <stdio.h>

#include <MinHook.h>
#pragma comment(lib, "libMinHook.lib")

#include "globals.h"

namespace polaris
{
    template<class T>
    struct TArray
    {
        friend struct SDK::FString;

    public:
        inline TArray()
        {
            Data = nullptr;
            Count = Max = 0;
        };

        inline int Num() const
        {
            return Count;
        };

        inline T& operator[](int i)
        {
            return Data[i];
        };

        inline const T& operator[](int i) const
        {
            return Data[i];
        };

        inline bool IsValidIndex(int i) const
        {
            return i < Num();
        }

        inline void ResizeGrow(int OldNum)
        {
            int Retval;

            SIZE_T Grow = 4; // this is the amount for the first alloc
            if (Max || (unsigned __int64)(Count) > Grow)
            {
                // Allocate slack for the array proportional to its size.
                Grow = (unsigned __int64)(Count) + 3 * (unsigned __int64)(Count) / 8 + 16;
            }

            Retval = Grow;

            // NumElements and MaxElements are stored in 32 bit signed integers so we must be careful not to overflow here.
            if (Count > Retval)
            {
                Retval = 0x7fffffff;
            }

            Max = Retval;

            if (Count)
            {
                Data = ::realloc(Data, Count * sizeof(T));
            }
            else
            {
                ::free(Data);

                Data = nullptr;
            }
        }

        inline int AddUninitialized(int NewCount = 1)
        {
            const int OldNum = Count;
            if ((Count += NewCount) > Max)
            {
                ResizeGrow(OldNum);
            }
            return OldNum;
        }

        inline void SetNumUninitialized(int NewNum, bool bAllowShrinking = true)
        {
            if (NewNum > Num())
            {
                AddUninitialized(NewNum - Num());
            }
            else if (NewNum < Num())
            {
                // TODO
            }
        }

        inline TArray(int what)
        {
            Data = (T*)::malloc(what * sizeof(T));
            Count = what;
            Max = what;
        }

    private:
        T* Data;
        int32_t Count;
        int32_t Max;
    };

    class Util
    {
    private:
        static BOOL MaskCompare(PVOID pBuffer, LPCSTR lpPattern, LPCSTR lpMask)
        {
            for (auto value = static_cast<PBYTE>(pBuffer); *lpMask; ++lpPattern, ++lpMask, ++value)
            {
                if (*lpMask == 'x' && *reinterpret_cast<LPCBYTE>(lpPattern) != *value)
                    return false;
            }

            return true;
        }

        static PBYTE FindPattern(PVOID pBase, DWORD dwSize, LPCSTR lpPattern, LPCSTR lpMask)
        {
            dwSize -= static_cast<DWORD>(strlen(lpMask));

            for (auto i = 0UL; i < dwSize; ++i)
            {
                auto pAddress = static_cast<PBYTE>(pBase) + i;

                if (MaskCompare(pAddress, lpPattern, lpMask))
                    return pAddress;
            }

            return NULL;
        }

    public:

        static VOID EnableConsole(bool enabled)
        {
            if (enabled)
            {
                auto pConsole = SDK::UConsole::StaticClass()->CreateDefaultObject<SDK::UConsole>();

                pConsole->Outer = Globals::gpLocalPlayer->ViewportClient;

                Globals::gpLocalPlayer->ViewportClient->ViewportConsole = pConsole;
            }
            else
                Globals::gpLocalPlayer->ViewportClient->ViewportConsole = nullptr;
        }

        // Initializes the console.
        static VOID InitConsole()
        {
            AllocConsole();

            FILE* pFile;
            freopen_s(&pFile, "CONOUT$", "w", stdout);
        }

        // Initializes the SDK.
        static VOID InitSdk()
        {
            auto pUWorldAddress = Util::FindPattern("\x48\x8B\x1D\x00\x00\x00\x00\x00\x00\x00\x10\x4C\x8D\x4D\x00\x4C", "xxx???????xxxx?x");
            if (!pUWorldAddress)
            {
                Util::ThrowFatalError(L"Finding pattern for UWorld has failed.");
            }

            auto pUWorldOffset = *reinterpret_cast<uint32_t*>(pUWorldAddress + 3);

            polaris::Globals::gpWorld = reinterpret_cast<SDK::UWorld**>(pUWorldAddress + 7 + pUWorldOffset);

            auto pGObjectAddress = Util::FindPattern("\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xD6", "xxx????x????x????x????xxx");
            if (!pGObjectAddress)
            {
                Util::ThrowFatalError(L"Finding pattern for GObject has failed.");
            }

            auto pGObjectOffset = *reinterpret_cast<uint32_t*>(pGObjectAddress + 3);

            SDK::UObject::GObjects = reinterpret_cast<SDK::FUObjectArray*>(pGObjectAddress + 7 + pGObjectOffset);

            auto pGNameAddress = Util::FindPattern("\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x75\x50\xB9\x00\x00\x00\x00\x48\x89\x5C\x24", "xxx????xxxxxx????xxxx");
            if (!pGNameAddress)
            {
                Util::ThrowFatalError(L"Finding pattern for GName has failed.");
            }

            auto pGNameOffset = *reinterpret_cast<uint32_t*>(pGNameAddress + 3);

            SDK::FName::GNames = *reinterpret_cast<SDK::TNameEntryArray**>(pGNameAddress + 7 + pGNameOffset);
        }

        // Initializes Polaris.
        static VOID InitCore()
        {
            uintptr_t pBaseAddress = Util::BaseAddress();
            if (!pBaseAddress)
            {
                Util::ThrowFatalError(L"Finding BaseAddress has failed.");
            }

            if (!polaris::Globals::gpWorld)
            {
                Util::ThrowFatalError(L"The UWorld is not initialized.");
            }

            polaris::Globals::gpLevel = (*polaris::Globals::gpWorld)->PersistentLevel;

            polaris::Globals::gpGameInstance = (*polaris::Globals::gpWorld)->OwningGameInstance;

            polaris::Globals::gpLocalPlayers = polaris::Globals::gpGameInstance->LocalPlayers;
            polaris::Globals::gpLocalPlayer = polaris::Globals::gpLocalPlayers[0];

            polaris::Globals::gpActors = &polaris::Globals::gpLevel->Actors;

            polaris::Globals::gpPlayerController = polaris::Globals::gpLocalPlayer->PlayerController;
        }

        // Patches certain parts of Fortnite.
        static VOID InitPatches()
        {
            // Item ownership check patching - allows weapons and other GameplayAbilites to properly function.
            auto pAbilityPatchAddress = polaris::Util::FindPattern
            (
                "\xC0\x0F\x84\x3C\x02\x00\x00\x0F\x2F\xF7\x0F\x86\xF5\x00\x00\x00",
                "xxxxxxxxxxxxxxxx"
            );
            if (pAbilityPatchAddress)
            {
                DWORD dwProtection;
                VirtualProtect(pAbilityPatchAddress, 16, PAGE_EXECUTE_READWRITE, &dwProtection);

                reinterpret_cast<uint8_t*>(pAbilityPatchAddress)[2] = 0x85;
                reinterpret_cast<uint8_t*>(pAbilityPatchAddress)[11] = 0x8D;

                DWORD dwTemp;
                VirtualProtect(pAbilityPatchAddress, 16, dwProtection, &dwTemp);
            }
        }

        // Shows an error message and closes Fortnite.
        static VOID ThrowFatalError(LPCWSTR errorInfo)
        {
            LPCWSTR base = L"Something went wrong with Polaris, please try restarting Fortnite.\n\nAdditional info:\n";
            std::wstring combined = std::wstring(base) + errorInfo;
            LPCWSTR combinedLPC = combined.c_str();

            MessageBox(0, combinedLPC, L"Error", MB_ICONERROR);
            ExitProcess(EXIT_FAILURE);
        }

        // Get the Base Address.
        static uintptr_t BaseAddress()
        {
            return reinterpret_cast<uintptr_t>(GetModuleHandle(0));
        }

        static PBYTE FindPattern(LPCSTR lpPattern, LPCSTR lpMask)
        {
            MODULEINFO info = { 0 };

            GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));

            return Util::FindPattern(info.lpBaseOfDll, info.SizeOfImage, lpPattern, lpMask);
        }

        // Find an AActor in the current UWorld.
        template<class T>
        static T* FindActor(int iSkip = 0)
        {
            for (int i = 0, j = 0; i < polaris::Globals::gpActors->Num(); i++)
            {
                SDK::AActor* pActor = polaris::Globals::gpActors->operator[](i);

                if (pActor != nullptr)
                {
                    if (pActor->IsA(T::StaticClass()))
                    {
                        if (j >= iSkip)
                            return (T*)pActor;
                        else
                        {
                            j++;
                            continue;
                        }
                    }
                }
            }

            return nullptr;
        }

        static std::string GetConcatPath(const std::string& sFirst, const std::string& sSecond)
        {
            std::string sTemp = sFirst;

            if (sFirst[sFirst.length()] != '\\')
            {
                sTemp += '\\';

                return(sTemp + sSecond);
            }
            else
                return(sFirst + sSecond);
        }
    };
}

#endif // UTIL_H
