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

#ifndef GLOBALS_H
#define GLOBALS_H

#include "SDK.hpp"

namespace polaris
{
	namespace globals
	{
		extern SDK::UWorld** gpWorld;
		extern SDK::ULevel* gpLevel;
		extern SDK::TArray<SDK::AActor*>* gpActors;

		extern SDK::UGameInstance* gpGameInstance;

		extern SDK::TArray<SDK::ULocalPlayer*> gpLocalPlayers;
		extern SDK::ULocalPlayer* gpLocalPlayer;

		extern SDK::APlayerController* gpPlayerController;
		extern SDK::UObject* (*StaticConstructObject_Internal)(SDK::UClass* InClass, SDK::UObject* InOuter, SDK::FName InName, int64_t InFlags, SDK::FUObjectItem::ObjectFlags InternalSetFlags, SDK::UObject* InTemplate, bool bCopyTransientsFromClassDefaults, void* InInstanceGraph, bool bAssumeTemplateIsArchetype);
	}
};

#endif // !GLOBALS_H