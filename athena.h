#ifndef ATHENA_H
#define ATHENA_H

#include "SDK.hpp"
#include "player.h"
#include "inventorymapper.h"

namespace polaris
{
	class Athena
	{
	public:
		// Variables for the ProcessEventHook
		bool m_bIsInFrontend = true;
		bool m_bIsWaitingForLoadingScreen = false;
		bool m_bHasPreloadedWeapons = false;
		bool m_bGameOver = false;
		bool m_bIsInBuildingMode = false;
		bool m_bIsQuickswitchKeyBeingHeldDown = false;

		// Variables for Athena_Terrain
		Player* m_pPlayer;
		SDK::UFortWeaponItemDefinition* m_pHarvestingToolDefinition;
		SDK::UFortWeaponItemDefinition* m_pSlot2Definition;
		SDK::UFortWeaponItemDefinition* m_pSlot3Definition;
		SDK::UFortWeaponItemDefinition* m_pSlot4Definition;
		SDK::UFortWeaponItemDefinition* m_pSlot5Definition;
		SDK::UFortWeaponItemDefinition* m_pSlot6Definition;
		SDK::UFortWeaponItemDefinition* m_pSlotLastUsedDefinition;
		SDK::UFortWeaponItemDefinition* m_pEditToolDef;
		SDK::UFortWeaponItemDefinition* m_pWallBuildDef;
		SDK::UFortWeaponItemDefinition* m_pFloorBuildDef;
		SDK::UFortWeaponItemDefinition* m_pStairBuildDef;
		SDK::UFortWeaponItemDefinition* m_pRoofBuildDef;
		SDK::UFortWeaponItemDefinition* m_pLastUsedBuildingDef;
		int m_iLastUsedItem = 0;
		int m_iLastUsedBuildingItem = 6;
		int m_iQuickbarLastUsedBuilding = 0;
		int m_iSlotLastUsedRevision = 0;
		int m_iSlot2Revision = 0;
		int m_iSlot3Revision = 0;
		int m_iSlot4Revision = 0;
		int m_iSlot5Revision = 0;
		int m_iSlot6Revision = 0;
		Athena();
	};
}

static polaris::Athena* gpAthena;

#endif // ATHENA_H
