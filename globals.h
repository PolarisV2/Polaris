#ifndef GLOBALS_H
#define GLOBALS_H

#include "SDK.hpp"

namespace polaris
{
	class Globals
	{
	public:
		static inline SDK::UWorld** gpWorld;
		static inline SDK::ULevel* gpLevel;
		static inline SDK::TArray<SDK::AActor*>* gpActors;

		static inline SDK::UGameInstance* gpGameInstance;

		static inline SDK::TArray<SDK::ULocalPlayer*> gpLocalPlayers;
		static inline SDK::ULocalPlayer* gpLocalPlayer;

		static inline SDK::APlayerController* gpPlayerController;
	};
};

#endif // GLOBALS_H
