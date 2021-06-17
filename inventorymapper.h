#ifndef INVENTORYMAPPER_H
#define INVENTORYMAPPER_H

#include "common.h"
#include "window.h"
#include "athena.h"

#include <imgui.h>
#include <list>

namespace polaris
{
	class InventoryMapper : public Window
	{
	private:
		int m_iPickingNewWIDFor = 0;
		bool m_bPickingNewWID = false;
		bool m_bHasLoadedItemList = false;

		std::string GetRarityString(SDK::EFortRarity rarity);
	public:
		std::string m_aInventoryItems[6];
		std::string m_aInventoryItemNames[6];
		std::list<SDK::UFortWeaponRangedItemDefinition*> m_lItemsInMemory;

		InventoryMapper();
		void Draw() override;
	};
}

extern polaris::InventoryMapper* gpInventoryMapper;

#endif // INVENTORYMAPPER_H