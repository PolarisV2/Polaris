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

#ifndef SDK_UTILS_H
#define SDK_UTILS_H

#include "SDK.hpp"
#include "globals.h"

#include <inttypes.h>
#include <Windows.h>
#include <psapi.h>
#include <stdio.h>
#include <iostream>

namespace polaris
{
	namespace utilities
	{
		//!  Utilities for the SDK.
		/*!
		* Serves as easy access to many SDK utilities like finding a pattern.
		*/
		class SDKUtils
		{
		private:
			static BOOL MaskCompare(PVOID pBuffer, LPCSTR lpPattern, LPCSTR lpMask);
			static PBYTE FindPattern(PVOID pBase, DWORD dwSize, LPCSTR lpPattern, LPCSTR lpMask);

		public:
			static uintptr_t BaseAddress(); //!< BaseAddress returns the base offset of the game.
			static PBYTE FindPattern(LPCSTR lpPattern, LPCSTR lpMask); //!< FindPattern finds a pattern in the game's EXE.
			static SDK::AActor* FindActor(SDK::UClass* pClass, int iSkip = 0); //!< FindActor returns an actor by its class.
			static VOID InitConsole(); //!< InitConsole initializes a new Console window if it doesn't exist yet.
			static VOID ToggleUnrealConsole(bool bIsEnabled); // initializes the Unreal Engine 4 console, handled via boolean
			static VOID InitSdk(); //!< InitSdk loads the SDK's properties.
			static VOID InitGlobals(); //!< InitGlobals puts the SDK's properties in globals.
			static VOID InitPatches(); //!< InitPatches patches Fortnite to circumvent errors.
			static SDK::UObject* FindOrLoadObject(const std::string PathName); //!< Finds an object, if the object isn't found, it'll load it into memory.
			static SDK::FGuid GenerateGuid(); //!< Generates a guid.
			static SDK::FGuid* GenerateGuidPtr(); //!< Generates a guid*.
		};
	}
}

#endif // !SDK_UTILS_H
