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

#include "athena_plate.h"
#include "globals.h"
#include "error_utils.h"
#include "sdk_utils.h"
#include "program.h"
#include <MinHook.h>
#include <vector>
#include <map>
#include <iostream>

PVOID(*CollectGarbageInternal)(uint32_t, bool) = nullptr;
PVOID CollectGarbageInternalHook(uint32_t KeepFlags, bool bPerformFullPurge)
{
    return NULL;
}
struct AFortAsQuickBars
{
public:
    unsigned char                                      UnknownData00[0x1A88];
    class SDK::AFortQuickBars* QuickBars;
};
struct AFortAsBuildPreview
{
public:
    unsigned char UnknownData00[0x1788];
    class SDK::ABuildingPlayerPrimitivePreview* BuildPreviewMarker;
};
struct AFortAsCurrentBuildable
{
public:
    unsigned char UnknownData00[0x1940];
    class SDK::UClass* CurrentBuildableClass;
};

namespace polaris
{
    namespace tables
    {
        namespace plates
        {
            
            void AthenaPlate::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
            {
                if (m_pPlayerPawn != nullptr)
                    m_pPlayerPawn->ProcessEventHook(pObject, pFunction, pParams);

                if (pFunction->GetName().find("ReadyToStartMatch") != std::string::npos && m_bIsInitialized == false)
                    Initialize();

                if (pFunction->GetName().find("ServerAttemptAircraftJump") != std::string::npos ||
                    (pFunction->GetName().find("AircraftExitedDropZone") != std::string::npos && static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController)->IsInAircraft()))
                {
                    if (pFunction->GetName().find("ServerAttemptAircraftJump") != std::string::npos)
                        m_pPlayerPawn->~AthenaPawn();

                    m_pPlayerPawn = new pawn::pawns::AthenaPawn;
                }

                if (pFunction->GetName().find("ServerLoadingScreenDropped") != std::string::npos)
                    SetupQuickbars();
                if (pFunction->GetName().find("ServerExecuteInventoryItem") != std::string::npos) {
                    SDK::FGuid* paramGuid = reinterpret_cast<SDK::FGuid*>(pParams);
                    for (auto it = m_mItems.begin(); it != m_mItems.end(); it++) {
                        if (AreGuidsTheSame((*it->first), (*paramGuid)) && !AreGuidsTheSame((*m_pgWallBuild), (*paramGuid)) && !AreGuidsTheSame((*m_pgFloorBuild), (*paramGuid)) && !AreGuidsTheSame((*m_pgStairBuild), (*paramGuid)) && !AreGuidsTheSame((*m_pgRoofBuild), (*paramGuid))) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(it->second, (*it->first));
                            m_bOnWall = false;
                            m_bOnFloor = false;
                            m_bOnStair = false;
                            m_bOnRoof = false;
                        }
                    }
                    for (auto it = m_mTraps.begin(); it != m_mTraps.end(); it++) {
                        if (AreGuidsTheSame((*it->first), (*paramGuid))) {
                            if (bTrapDone == false) {
                                m_pTrapC = SDK::UObject::FindObject<SDK::ABuildingActor>("Trap_Floor_Player_Jump_Free_Direction_Pad_C Athena_Terrain.Athena_Terrain.PersistentLevel.Trap_Floor_Player_Jump_Free_Direction_Pad_C_1");
                                bTrapDone = true;
                            }
                            m_pPlayerPawn->m_pPawnActor->PickUpActor(m_pTrapC, it->second);
                            m_pPlayerPawn->m_pPawnActor->CurrentWeapon->ItemEntryGuid = (*it->first);
                            m_bOnWall = false;
                            m_bOnFloor = false;
                            m_bOnStair = false;
                            m_bOnRoof = false;
                        }
                    }
                    if (AreGuidsTheSame((*paramGuid), (*m_pgWallBuild))) {
                        if (!m_bOnWall) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(m_pWallBuildDef, (*paramGuid));
                            reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = SDK::APBWA_W1_Solid_C::StaticClass();
                            reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->m_pBuildPreviewWall;
                            m_pPlayerPawn->m_pBuildPreviewWall->SetActorHiddenInGame(false);
                            m_pPlayerPawn->m_pBuildPreviewFloor->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewStair->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewRoof->SetActorHiddenInGame(true);

                            // building position fix
                            if (!m_pPlayerPawn->m_bHasAutoCycledWall)
                            {
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Stone"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Metal"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                m_pPlayerPawn->m_bHasAutoCycledWall = true;
                            }

                            m_bOnWall = true;
                            m_bOnFloor = false;
                            m_bOnStair = false;
                            m_bOnRoof = false;
                        }
                    }
                    if (AreGuidsTheSame((*paramGuid), (*m_pgFloorBuild))) {
                        if (!m_bOnFloor) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(m_pFloorBuildDef, (*paramGuid));
                            reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = SDK::APBWA_W1_Floor_C::StaticClass();
                            reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->m_pBuildPreviewFloor;
                            m_pPlayerPawn->m_pBuildPreviewWall->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewFloor->SetActorHiddenInGame(false);
                            m_pPlayerPawn->m_pBuildPreviewStair->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewRoof->SetActorHiddenInGame(true);

                            // building position fix
                            if (!m_pPlayerPawn->m_bHasAutoCycledFloor)
                            {
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Stone"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Metal"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                m_pPlayerPawn->m_bHasAutoCycledFloor = true;
                            }

                            m_bOnWall = false;
                            m_bOnFloor = true;
                            m_bOnStair = false;
                            m_bOnRoof = false;
                        }
                    }
                    if (AreGuidsTheSame((*paramGuid), (*m_pgStairBuild))) {
                        if (!m_bOnStair) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(m_pStairBuildDef, (*paramGuid));
                            reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = SDK::APBWA_W1_StairW_C::StaticClass();
                            reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->m_pBuildPreviewStair;
                            m_pPlayerPawn->m_pBuildPreviewWall->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewFloor->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewStair->SetActorHiddenInGame(false);
                            m_pPlayerPawn->m_pBuildPreviewRoof->SetActorHiddenInGame(true);

                            // building position fix
                            if (!m_pPlayerPawn->m_bHasAutoCycledStair)
                            {
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Stone"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Metal"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                m_pPlayerPawn->m_bHasAutoCycledStair = true;
                            }

                            m_bOnWall = false;
                            m_bOnFloor = false;
                            m_bOnStair = true;
                            m_bOnRoof = false;
                        }
                    }
                    if (AreGuidsTheSame((*paramGuid), (*m_pgRoofBuild))) {
                        if (!m_bOnRoof) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(m_pRoofBuildDef, (*paramGuid));
                            reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = SDK::APBWA_W1_RoofC_C::StaticClass();
                            reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->m_pBuildPreviewRoof;
                            m_pPlayerPawn->m_pBuildPreviewWall->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewFloor->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewStair->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewRoof->SetActorHiddenInGame(false);

                            // building position fix
                            if (!m_pPlayerPawn->m_bHasAutoCycledRoof)
                            {
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Stone"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Metal"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                m_pPlayerPawn->m_bHasAutoCycledRoof = true;
                            }

                            m_bOnWall = false;
                            m_bOnFloor = false;
                            m_bOnStair = false;
                            m_bOnRoof = true;
                        }
                    }
                }
            }
            void AthenaPlate::Update()
            {
                if (m_pPlayerPawn != nullptr)
                    m_pPlayerPawn->Update();

                // TEMP: Go back to Frontend.
                if (GetAsyncKeyState(VK_OEM_PLUS) & 0x8000)
                    gpProgram->m_pMainTable->PopPlate();
            }

            void AthenaPlate::OnEnabled()
            {
                utilities::SDKUtils::InitSdk();
                utilities::SDKUtils::InitGlobals();

                globals::gpPlayerController->SwitchLevel(TEXT("Athena_Terrain"));
            }

            void AthenaPlate::Initialize()
            {
                //Initialize the SDK again.
                m_bIsInitialized = true;
                utilities::SDKUtils::InitSdk();
                utilities::SDKUtils::InitGlobals();
                utilities::SDKUtils::InitPatches();
                //Load Husks Into Memory.
                utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn.HuskPawn_C");
                utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Fire.HuskPawn_Fire_C");
                utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Ice.HuskPawn_Ice_C");
                utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Lightning.HuskPawn_Lightning_C");
                utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Beehive.HuskPawn_Beehive_C");
                utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Bombshell.HuskPawn_Bombshell_C");
                utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Bombshell_Poison.HuskPawn_Bombshell_Poison_C");
                utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Dwarf.HuskPawn_Dwarf_C");
                utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Dwarf_Fire.HuskPawn_Dwarf_Fire_C");
                utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Dwarf_Ice.HuskPawn_Dwarf_Ice_C");
                utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Dwarf_Lightning.HuskPawn_Dwarf_Lightning_C");

                // Disable garbage collection.
                auto pCollectGarbageInternalAddress = utilities::SDKUtils::FindPattern("\x48\x8B\xC4\x48\x89\x58\x08\x88\x50\x10", "xxxxxxxxxx");
                if (!pCollectGarbageInternalAddress)
                    utilities::ErrorUtils::ThrowException(L"Finding pattern for CollectGarbageInternal has failed. Please relaunch Fortnite and try again!");
                
                MH_CreateHook(static_cast<LPVOID>(pCollectGarbageInternalAddress), CollectGarbageInternalHook, reinterpret_cast<LPVOID*>(&CollectGarbageInternal));
                MH_EnableHook(static_cast<LPVOID>(pCollectGarbageInternalAddress));

                // Spawn a Player Pawn and setup inventory.
                m_pPlayerPawn = new pawn::pawns::AthenaPawn;
                
                SetupInventory();

                // Tell the client that we are ready to start the match, this allows the loading screen to drop.
                static_cast<SDK::AFortPlayerController*>(globals::gpPlayerController)->ServerReadyToStartMatch();
                static_cast<SDK::AGameMode*>((*globals::gpWorld)->AuthorityGameMode)->StartMatch();
            }
            bool AthenaPlate::AreGuidsTheSame(SDK::FGuid guidA, SDK::FGuid guidB)
            {
                if (guidA.A == guidB.A && guidA.B == guidB.B && guidA.C == guidB.C && guidA.D == guidB.D) 
                    return true;
                else
                    return false;
            }
            void AthenaPlate::SetupQuickbars()
            {
                auto controller = static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController);
                auto asfortquickbars = reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController);
                polaris::globals::gpPlayerController->CheatManager->Summon(TEXT("FortQuickBars"));
                asfortquickbars->QuickBars = static_cast<SDK::AFortQuickBars*>(utilities::SDKUtils::FindActor(SDK::AFortQuickBars::StaticClass()));
                asfortquickbars->QuickBars->SetOwner(polaris::globals::gpPlayerController);
                auto pQuickBars = static_cast<SDK::AFortQuickBars*>(utilities::SDKUtils::FindActor(SDK::AFortQuickBars::StaticClass()));
                controller->Role = SDK::ENetRole::ROLE_None;
                controller->OnRep_QuickBar();
                controller->Role = SDK::ENetRole::ROLE_Authority;
                asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 0);
                asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 1);
                asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 2);
                asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 3);
                asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 4);
                asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 5);
                asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Primary, 0);
                pQuickBars->ServerAddItemInternal((*m_pgWallBuild), SDK::EFortQuickBars::Secondary, 0);
                pQuickBars->ServerAddItemInternal((*m_pgFloorBuild), SDK::EFortQuickBars::Secondary, 1);
                pQuickBars->ServerAddItemInternal((*m_pgStairBuild), SDK::EFortQuickBars::Secondary, 2);
                pQuickBars->ServerAddItemInternal((*m_pgRoofBuild), SDK::EFortQuickBars::Secondary, 3);
                pQuickBars->ServerAddItemInternal((*m_pgPickaxe), SDK::EFortQuickBars::Primary, 0);
                pQuickBars->ServerActivateSlotInternal(SDK::EFortQuickBars::Primary,0,0,true);
            }
            void AthenaPlate::SetupInventory()
            {
                // Defining Item Definitions
                m_pPickaxeDef = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>(m_pPlayerPawn->mPickaxeAsWid[static_cast<SDK::AFortPlayerPawnAthena*>(m_pPlayerPawn->m_pPawnActor)->CustomizationLoadout.Character->GetName()].c_str());
                m_pWallBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Wall.BuildingItemData_Wall");
                m_pFloorBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Floor.BuildingItemData_Floor");
                m_pStairBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Stair_W.BuildingItemData_Stair_W");
                m_pRoofBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_RoofS.BuildingItemData_RoofS");
                auto pWood = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortResourceItemDefinition WoodItemData.WoodItemData");
                auto pMetal = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortResourceItemDefinition MetalItemData.MetalItemData");
                auto pStone = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortResourceItemDefinition StoneItemData.StoneItemData");
                auto pRockets = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortAmmoItemDefinition AmmoDataRockets.AmmoDataRockets");
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
                auto pInventory1 = static_cast<SDK::AFortInventory*>(polaris::utilities::SDKUtils::FindActor(SDK::AFortInventory::StaticClass()));
                auto pInventory2 = static_cast<SDK::AFortInventory*>(polaris::utilities::SDKUtils::FindActor(SDK::AFortInventory::StaticClass(), 1));
                SDK::AFortInventory* pActualInv = nullptr;

                // joe nuts
                if (pInventory1 && pInventory1->InventoryType == SDK::EFortInventoryType::World)
                {
                    pActualInv = pInventory1;
                }

                if (pInventory2 && pInventory2->InventoryType == SDK::EFortInventoryType::World)
                {
                    pActualInv = pInventory2;
                }

                if (pActualInv)
                {
                    //gaming rgb inventory
                    auto myInventory = pActualInv;
                    SDK::FFortItemList* inv = &myInventory->Inventory;
                    SDK::TArray<class SDK::UFortWorldItem*>* pItemInsts = &inv->ItemInstances;
                    pItemInsts->Count = m_mItems.size();
                    pItemInsts->Max = m_mItems.size();
                    pItemInsts->Data = (class SDK::UFortWorldItem**)::malloc(pItemInsts->Count * sizeof(SDK::UFortWorldItem*));
                    for (auto it = m_mItems.begin(); it != m_mItems.end(); it++) {
                        auto pItemEntry = new SDK::FFortItemEntry;
                        pItemEntry->Count = 100;
                        pItemEntry->ItemDefinition = it->second;
                        pItemEntry->Durability = 1000;
                        pItemEntry->Level = 1;
                        pItemEntry->ItemGuid = (*it->first);
                        pItemEntry->bIsDirty = false;
                        pItemEntry->LoadedAmmo = 0;
                        auto pWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                        pWorldItem->OwnerInventory = myInventory;
                        pWorldItem->ItemEntry = *pItemEntry;
                        pItemInsts->operator[](iInventoryIteration) = pWorldItem;
                        iInventoryIteration++;
                    }
                }
            }
        }
    }
}