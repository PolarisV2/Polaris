/*
* Copyright (c) 2021, Polaris All rights reserved.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "sdk_utils.h"
#include "error_utils.h"

static SDK::UObject* (*StaticLoadObject)(SDK::UClass* ObjectClass, SDK::UObject* InOuter, const TCHAR* InName, const TCHAR* Filename, uint32_t LoadFlags, SDK::UPackageMap* Sandbox, bool bAllowObjectReconciliation);
template<class T>
static T* LoadObject(SDK::UObject* Outer, const TCHAR* Name, const TCHAR* Filename = nullptr, uint32_t LoadFlags = 0, SDK::UPackageMap* Sandbox = nullptr)
{
	return (T*)StaticLoadObject(T::StaticClass(), Outer, Name, Filename, LoadFlags, Sandbox, true);
}
namespace polaris
{
	namespace utilities
	{
		BOOL SDKUtils::MaskCompare(PVOID pBuffer, LPCSTR lpPattern, LPCSTR lpMask)
		{
			for (auto value = static_cast<PBYTE>(pBuffer); *lpMask; ++lpPattern, ++lpMask, ++value)
			{
				if (*lpMask == 'x' && *reinterpret_cast<LPCBYTE>(lpPattern) != *value)
					return false;
			}

			return true;
		}
		PBYTE SDKUtils::FindPattern(PVOID pBase, DWORD dwSize, LPCSTR lpPattern, LPCSTR lpMask)
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

		SDK::UObject* SDKUtils::FindOrLoadObject(const std::string PathName)
		{
			SDK::UClass* Class = SDK::UClass::StaticClass();
			Class->CreateDefaultObject();

			auto ObjectPtr = LoadObject<SDK::UObject>(NULL, std::wstring(PathName.begin(), PathName.end()).c_str());
			if (ObjectPtr)
			{
				SDK::UObject::GObjects
					->ObjObjects.GetItemByIndex(ObjectPtr->InternalIndex)->Flags |= int32_t(SDK::FUObjectItem::ObjectFlags::RootSet);
			}

			return ObjectPtr;
		}
		uintptr_t SDKUtils::BaseAddress()
		{
			return reinterpret_cast<uintptr_t>(GetModuleHandle(0));
		}
		PBYTE SDKUtils::FindPattern(LPCSTR lpPattern, LPCSTR lpMask)
		{
			MODULEINFO info = { 0 };

			GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));

			PBYTE result = SDKUtils::FindPattern(info.lpBaseOfDll, info.SizeOfImage, lpPattern, lpMask);
			if (!result)
				ErrorUtils::ThrowException(L"Pattern was not found.");

			return result;
		}
		SDK::AActor* SDKUtils::FindActor(SDK::UClass* pClass, int iSkip)
		{
			for (int i = 0, j = 0; i < globals::gpActors->Num(); i++)
			{
				SDK::AActor* pActor = globals::gpActors->operator[](i);

				if (pActor != nullptr)
				{
					if (pActor->IsA(pClass))
					{
						if (j >= iSkip)
							return pActor;
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
		SDK::FGuid* SDKUtils::GenerateGuidPtr()
		{
			SDK::FGuid* Guid = new SDK::FGuid();
			Guid->A = rand() % 1000;
			Guid->B = rand() % 1000;
			Guid->C = rand() % 1000;
			Guid->D = rand() % 1000;
			return Guid;
		}
		SDK::FGuid SDKUtils::GenerateGuid()
		{
			SDK::FGuid Guid;
			Guid.A = rand() % 1000;
			Guid.B = rand() % 1000;
			Guid.C = rand() % 1000;
			Guid.D = rand() % 1000;
			return Guid;
		}
		VOID SDKUtils::InitConsole()
		{
			AllocConsole();

			FILE* pFile;
			freopen_s(&pFile, "CONOUT$", "w", stdout);
		}
		VOID SDKUtils::ToggleUnrealConsole(bool bIsEnabled)
		{
			if (bIsEnabled)
			{
				// create the Unreal 4 console
				auto pConsole = SDK::UConsole::StaticClass()->CreateDefaultObject<SDK::UConsole>();
				pConsole->Outer = globals::gpLocalPlayer->ViewportClient;

				globals::gpLocalPlayer->ViewportClient->ViewportConsole = pConsole;
			}
			else
			{
				// redirect the console to a nullptr if bIsEnabled already exists
				globals::gpLocalPlayer->ViewportClient->ViewportConsole = nullptr;
			}
		}
		VOID SDKUtils::InitSdk()
		{
			auto pUWorldAddress = SDKUtils::FindPattern("\x48\x8B\x1D\x00\x00\x00\x00\x00\x00\x00\x10\x4C\x8D\x4D\x00\x4C", "xxx???????xxxx?x");
			auto pUWorldOffset = *reinterpret_cast<uint32_t*>(pUWorldAddress + 3);
			globals::gpWorld = reinterpret_cast<SDK::UWorld**>(pUWorldAddress + 7 + pUWorldOffset);

			auto pGObjectAddress = SDKUtils::FindPattern("\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xD6", "xxx????x????x????x????xxx");
			auto pGObjectOffset = *reinterpret_cast<uint32_t*>(pGObjectAddress + 3);
			SDK::UObject::GObjects = reinterpret_cast<SDK::FUObjectArray*>(pGObjectAddress + 7 + pGObjectOffset);

			auto pGNameAddress = SDKUtils::FindPattern("\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x75\x50\xB9\x00\x00\x00\x00\x48\x89\x5C\x24", "xxx????xxxxxx????xxxx");
			auto pGNameOffset = *reinterpret_cast<uint32_t*>(pGNameAddress + 3);

			SDK::FName::GNames = *reinterpret_cast<SDK::TNameEntryArray**>(pGNameAddress + 7 + pGNameOffset);

			auto pStaticConstructObject_InternalOffset = FindPattern("\xE8\x00\x00\x00\x00\x89\x78\x38", "x????xxx");
			if (!pStaticConstructObject_InternalOffset)
				ErrorUtils::ThrowException(L"Finding pattern for StaticConstructObject_Internal has failed. Please relaunch Fortnite and try again!");
			auto pStaticConstructObject_InternalAddress = pStaticConstructObject_InternalOffset + 5 + *reinterpret_cast<int32_t*>(pStaticConstructObject_InternalOffset + 1);

			globals::StaticConstructObject_Internal = reinterpret_cast<decltype(globals::StaticConstructObject_Internal)>(pStaticConstructObject_InternalAddress);
			StaticLoadObject = reinterpret_cast<decltype(StaticLoadObject)>(BaseAddress() + 0x142E560);
		}
		VOID SDKUtils::InitGlobals()
		{
			globals::gpLevel = (*globals::gpWorld)->PersistentLevel;
			globals::gpGameInstance = (*globals::gpWorld)->OwningGameInstance;
			globals::gpLocalPlayers = globals::gpGameInstance->LocalPlayers;
			globals::gpLocalPlayer = globals::gpLocalPlayers[0];
			globals::gpActors = &globals::gpLevel->Actors;
			globals::gpPlayerController = globals::gpLocalPlayer->PlayerController;
		}
		VOID SDKUtils::InitPatches()
		{
			// Item ownership check patching - allows weapons and other GameplayAbilites to properly function.
			auto pAbilityPatchAddress = SDKUtils::FindPattern
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
	}
}