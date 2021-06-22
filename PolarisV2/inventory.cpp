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

#include "inventory.h"
#include "globals.h"
#include "error_utils.h"
#include "inventory_offset_fixes.h"
#include "sdk_utils.h"
#include "program.h"
#include <MinHook.h>
#include <vector>
#include <map>
#include <iostream>

namespace polaris
{
    namespace inventory
    {
        void Inventory::SetupInventory()
        {
            // Defining Item Definitions
            m_pEditToolDef = SDK::UObject::FindObject<SDK::UFortEditToolItemDefinition>("FortEditToolItemDefinition EditTool.EditTool");
            m_pWallBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Wall.BuildingItemData_Wall");
            m_pFloorBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Floor.BuildingItemData_Floor");
            m_pStairBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Stair_W.BuildingItemData_Stair_W");
            m_pRoofBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_RoofS.BuildingItemData_RoofS");
            auto pWood = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortResourceItemDefinition WoodItemData.WoodItemData");
            auto pMetal = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortResourceItemDefinition MetalItemData.MetalItemData");
            auto pStone = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortResourceItemDefinition StoneItemData.StoneItemData");
            auto pRockets = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortAmmoItemDefinition AmmoDataRockets.AmmoDataRockets");
            m_pgEditToolDef = utilities::SDKUtils::GenerateGuidPtr();
            m_pgPickaxe = utilities::SDKUtils::GenerateGuidPtr();
            m_pgWallBuild = utilities::SDKUtils::GenerateGuidPtr();
            m_pgFloorBuild = utilities::SDKUtils::GenerateGuidPtr();
            m_pgStairBuild = utilities::SDKUtils::GenerateGuidPtr();
            m_pgRoofBuild = utilities::SDKUtils::GenerateGuidPtr();
            m_mItems.insert_or_assign(utilities::SDKUtils::GenerateGuidPtr(), pWood);
            m_mItems.insert_or_assign(utilities::SDKUtils::GenerateGuidPtr(), pMetal);
            m_mItems.insert_or_assign(utilities::SDKUtils::GenerateGuidPtr(), pStone);
            m_mItems.insert_or_assign(utilities::SDKUtils::GenerateGuidPtr(), pRockets);
            m_mItems.insert_or_assign(m_pgPickaxe, m_pPickaxeDef);
            m_mItems.insert_or_assign(m_pgWallBuild, m_pWallBuildDef);
            m_mItems.insert_or_assign(m_pgFloorBuild, m_pFloorBuildDef);
            m_mItems.insert_or_assign(m_pgStairBuild, m_pStairBuildDef);
            m_mItems.insert_or_assign(m_pgRoofBuild, m_pRoofBuildDef);
            m_mItems.insert_or_assign(m_pgEditToolDef, m_pEditToolDef);
            for (int i = 0; i < SDK::UObject::GetGlobalObjects().Num(); ++i)
            {
                auto pObject = SDK::UObject::GetGlobalObjects().GetByIndex(i);
                if (pObject != nullptr && pObject->GetFullName().find("FortniteGame") == std::string::npos)
                {
                    if (pObject->GetFullName().rfind("FortAmmoItemDefinition", 0) == 0)
                    {
                        if (pObject->GetFullName().find("Athena") != std::string::npos)
                            m_mItems[utilities::SDKUtils::GenerateGuidPtr()] = reinterpret_cast<SDK::UFortWeaponItemDefinition*>(pObject);
                    }
                    if (pObject->GetFullName().rfind("FortWeaponRangedItemDefinition", 0) == 0)
                    {
                        if (pObject->GetFullName().find("Athena") != std::string::npos || pObject->GetFullName().find("Test") != std::string::npos)
                            m_mItems[utilities::SDKUtils::GenerateGuidPtr()] = reinterpret_cast<SDK::UFortWeaponItemDefinition*>(pObject);
                    }
                    if (pObject->GetFullName().rfind("FortTrapItemDefinition", 0) == 0)
                    {
                        if (pObject->GetFullName().find("Athena") != std::string::npos) {
                            auto guid = utilities::SDKUtils::GenerateGuidPtr();
                            m_mTraps[guid] = reinterpret_cast<SDK::UFortTrapItemDefinition*>(pObject);
                            m_mItems[guid] = reinterpret_cast<SDK::UFortTrapItemDefinition*>(pObject);
                        }
                    }
                }
            }
            auto controller = static_cast<SDK::AFortPlayerController*>(globals::gpPlayerController);
            auto winventory = reinterpret_cast<polaris::models::offsetfixes::WorldInventory*>(controller)->WorldInventory;
            if (winventory)
            {
                //gaming rgb inventory
                SDK::FFortItemList* inv = &winventory->Inventory;
                pItemInsts = &inv->ItemInstances;
                pItemInsts->Count = m_mItems.size();
                pItemInsts->Max = m_mItems.size();
                pItemInsts->Data = (class SDK::UFortWorldItem**)::malloc(pItemInsts->Count * sizeof(SDK::UFortWorldItem*));
                for (auto it = m_mItems.begin(); it != m_mItems.end(); it++) {
                    auto pItemEntry = new SDK::FFortItemEntry;
                    if (it->second->IsA(SDK::UFortWeaponItemDefinition::StaticClass()))
                        pItemEntry->Count = 1;
                    else
                        pItemEntry->Count = 100;
                    pItemEntry->ItemDefinition = it->second;
                    pItemEntry->Durability = it->second->GetMaxDurability(it->second->MaxLevel);
                    pItemEntry->Level = it->second->MaxLevel;
                    pItemEntry->ItemGuid = (*it->first);
                    pItemEntry->bIsDirty = false;
                    pItemEntry->bIsReplicatedCopy = true;
                    pItemEntry->LoadedAmmo = 0;
                    pItemEntry->ParentInventory.ObjectIndex = winventory->InternalIndex;
                    pItemEntry->ParentInventory.ObjectSerialNumber = SDK::UObject::GObjects->ObjObjects.GetItemByIndex(winventory->InternalIndex)->SerialNumber;
                    
                    //reinterpret_cast<polaris::models::offsetfixes::ParentInventory*>(pItemEntry)->ParentInventory = (*winventory);
                    auto pWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), winventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                    pWorldItem->bTemporaryItemOwningController = true;
                    pWorldItem->SetOwningControllerForTemporaryItem(controller);
                    reinterpret_cast<polaris::models::offsetfixes::OwnerInventory*>(pWorldItem)->OwnerInventory = winventory;
                    pWorldItem->ItemEntry = *pItemEntry;
                    pItemInsts->operator[](iInventoryIteration) = pWorldItem;
                    auto statval = new SDK::FFortItemEntryStateValue;
                    statval->IntValue = 1;
                    statval->NameValue = SDK::FName("Item");
                    statval->StateType = SDK::EFortItemEntryState::NewItemCount;
                    controller->ServerSetInventoryStateValue((*it->first), (*statval));
                    iInventoryIteration++;
                }
            }
        }
    }
}
