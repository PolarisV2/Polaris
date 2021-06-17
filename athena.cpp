#pragma warning(disable:26495)

#include "athena.h"

namespace polaris
{
	static SDK::UObject* (*StaticLoadObject)(SDK::UClass* ObjectClass, SDK::UObject* InOuter, const TCHAR* InName, const TCHAR* Filename, uint32_t LoadFlags, SDK::UPackageMap* Sandbox, bool bAllowObjectReconciliation);
	template<class T>
	static T* LoadObject(SDK::UObject* Outer, const TCHAR* Name, const TCHAR* Filename = nullptr, uint32_t LoadFlags = 0, SDK::UPackageMap* Sandbox = nullptr)
	{
		return (T*)StaticLoadObject(T::StaticClass(), Outer, Name, Filename, LoadFlags, Sandbox, true);
	}
	template<typename T>
	static T* FindOrLoadObject(const std::string PathName)
	{
		SDK::UClass* Class = T::StaticClass();
		Class->CreateDefaultObject();

		T* ObjectPtr = LoadObject<T>(NULL, std::wstring(PathName.begin(), PathName.end()).c_str());
		if (ObjectPtr)
		{
			SDK::UObject::GObjects
				->ObjObjects.GetItemByIndex(ObjectPtr->InternalIndex)->Flags |= int32_t(SDK::FUObjectItem::ObjectFlags::RootSet);
		}

		return ObjectPtr;
	}

	// Listens to functions called in game, use this to hook code to functions in question.
	PVOID(*ProcessEvent)(SDK::UObject*, SDK::UFunction*, PVOID) = nullptr;
	PVOID ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
	{

		if (pObject && pFunction)
		{
			// Hooks for Frontend
			if (gpAthena->m_bIsInFrontend)
			{
				// Load Athena_Terrain once the player readies up.
				if (pFunction->GetName().find("BndEvt__BP_PlayButton_K2Node_ComponentBoundEvent_1_CommonButtonClicked__DelegateSignature") != std::string::npos)
				{
					gpAthena->m_bIsWaitingForLoadingScreen = true;
					Globals::gpPlayerController->SwitchLevel(TEXT("Athena_Terrain"));
				}

				// This gets called once the loading screen is ready to drop.
				// We reinitialize the SDK, Core and update memory to fix weapon attack abilities.
				if (pFunction->GetName().find("ReadyToStartMatch") != std::string::npos && gpAthena->m_bIsWaitingForLoadingScreen)
				{
					gpAthena->m_bIsInFrontend = false;
					gpAthena->m_bIsWaitingForLoadingScreen = false;

					// Reinitialize the core, there's missing references because of Frontend unloading.
					Util::InitSdk();
					Util::InitCore();
					Util::InitPatches();

					StaticLoadObject = reinterpret_cast<decltype(StaticLoadObject)>(Util::BaseAddress() + 0x142E560);

					// Load UPackages required for Polaris to properly function
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Weapons/FORT_BuildingTools/Blueprints/DefaultBuildingTool.DefaultBuildingTool_C");
					FindOrLoadObject<SDK::UDataTable>("/Game/Items/Datatables/AthenaTraps.AthenaTraps");
					FindOrLoadObject<SDK::UCurveTable>("/Game/Items/Datatables/AthenaProjectiles.AthenaProjectiles");
					FindOrLoadObject<SDK::UDataTable>("/Game/Athena/Items/Weapons/AthenaMeleeWeapons.AthenaMeleeWeapons");
					FindOrLoadObject<SDK::UDataTable>("/Game/Athena/Items/Weapons/AthenaRangedWeapons.AthenaRangedWeapons");
					FindOrLoadObject<SDK::UDataTable>("/Game/Items/Datatables/RangedWeapons.RangedWeapons");
					FindOrLoadObject<SDK::UDataTable>("/Game/Items/Datatables/MeleeWeapons.MeleeWeapons");
					FindOrLoadObject<SDK::UDataTable>("/Game/Items/Datatables/Traps.Traps");

					// to wiktor: if you want your polaris compile to automatically load in all husk variants,
					// please add ENABLE_THE_FUNNY_HUSK to your preprocessor definitions
#ifdef ENABLE_THE_FUNNY_HUSK
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn.HuskPawn_C");
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Beehive.HuskPawn_Beehive_C");
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Bombshell.HuskPawn_Bombshell_C");
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Bombshell_Poison.HuskPawn_Bombshell_Poison_C");
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Dwarf.HuskPawn_Dwarf_C");
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Dwarf_Fire.HuskPawn_Dwarf_Fire_C");
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Dwarf_Ice.HuskPawn_Dwarf_Ice_C");
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Dwarf_Lightning.HuskPawn_Dwarf_Lightning_C");
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Fire.HuskPawn_Fire_C");
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Ice.HuskPawn_Ice_C");
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Lightning.HuskPawn_Lightning_C");
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Pitcher.HuskPawn_Pitcher_C");
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Sploder.HuskPawn_Sploder_C");
					FindOrLoadObject<SDK::UBlueprintGeneratedClass>("/Game/Missions/Secondary/Mimic/HuskPawn_Mimic.HuskPawn_Mimic_C");
#endif // ENABLE_THE_FUNNY_HUSK

					if (!gpAthena->m_pPlayer)
					{
						// Create a new Player Pawn.
						gpAthena->m_pPlayer = new Player;
						gpAthena->m_pPlayer->InitializeHero();

						// Load preset item definitions (Prevents a hitch during weapon swapping)
						gpAthena->m_pHarvestingToolDefinition = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>(gpAthena->m_pPlayer->m_pPlayerPawn->CustomizationLoadout.Character->GetFullName());
						gpAthena->m_pSlot2Definition = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortWeaponRangedItemDefinition WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03");
						gpAthena->m_pSlot3Definition = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortWeaponRangedItemDefinition WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03");
						gpAthena->m_pSlot4Definition = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortWeaponRangedItemDefinition WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03");
						gpAthena->m_pSlot5Definition = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortWeaponRangedItemDefinition WID_Launcher_Rocket_Athena_R_Ore_T03.WID_Launcher_Rocket_Athena_R_Ore_T03");
						gpAthena->m_pSlot6Definition = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortWeaponRangedItemDefinition WID_Sniper_AMR_Athena_SR_Ore_T03.WID_Sniper_AMR_Athena_SR_Ore_T03");

						// load building tools from memory (pivotman)
						gpAthena->m_pEditToolDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortEditToolItemDefinition EditTool.EditTool");
						gpAthena->m_pWallBuildDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortBuildingItemDefinition BuildingItemData_Wall.BuildingItemData_Wall");
						gpAthena->m_pFloorBuildDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortBuildingItemDefinition BuildingItemData_Floor.BuildingItemData_Floor");
						gpAthena->m_pStairBuildDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortBuildingItemDefinition BuildingItemData_Stair_W.BuildingItemData_Stair_W");
						gpAthena->m_pRoofBuildDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortBuildingItemDefinition BuildingItemData_RoofS.BuildingItemData_RoofS");
						
						// Tell the client that we are ready to start the match, this allows the loading screen to drop.
						static_cast<SDK::AAthena_PlayerController_C*>(Globals::gpPlayerController)->ServerReadyToStartMatch();
						static_cast<SDK::AGameMode*>((*Globals::gpWorld)->AuthorityGameMode)->StartMatch();

						// Preload the base Polaris weapon inventory in order to prevent a hitch
						if (!gpAthena->m_bHasPreloadedWeapons) {
							// load this giant hunk of assets into memory
							FindOrLoadObject<SDK::UFortEditToolItemDefinition>("/Game/Items/Weapons/BuildingTools/EditTool.EditTool");
							FindOrLoadObject<SDK::UFortBuildingItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_Wall.BuildingItemData_Wall");
							FindOrLoadObject<SDK::UFortBuildingItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_Floor.BuildingItemData_Floor");
							FindOrLoadObject<SDK::UFortBuildingItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_Floor.BuildingItemData_Floor");
							FindOrLoadObject<SDK::UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/" + gpAthena->m_pPlayer->m_pPlayerPawn->CustomizationLoadout.Character->GetName() + "." + gpAthena->m_pPlayer->m_pPlayerPawn->CustomizationLoadout.Character->GetName());
							FindOrLoadObject<SDK::UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03");
							FindOrLoadObject<SDK::UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03");
							FindOrLoadObject<SDK::UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03");
							FindOrLoadObject<SDK::UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_SR_Ore_T03.WID_Launcher_Rocket_Athena_SR_Ore_T03");
							FindOrLoadObject<SDK::UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_AMR_Athena_SR_Ore_T03.WID_Sniper_AMR_Athena_SR_Ore_T03");
							
							// Equip them afterwards
							SDK::FGuid guid;
							guid.A = 1;
							guid.B = 0;
							guid.C = 0;
							guid.D = 0;
							gpAthena->m_pPlayer->m_pPlayerPawn->EquipWeaponDefinition(gpAthena->m_pSlot2Definition, guid);
							guid.A = 2;
							guid.B = 0;
							guid.C = 0;
							guid.D = 0;
							gpAthena->m_pPlayer->m_pPlayerPawn->EquipWeaponDefinition(gpAthena->m_pSlot3Definition, guid);
							guid.A = 3;
							guid.B = 0;
							guid.C = 0;
							guid.D = 0;
							gpAthena->m_pPlayer->m_pPlayerPawn->EquipWeaponDefinition(gpAthena->m_pSlot4Definition, guid);
							guid.A = 4;
							guid.B = 0;
							guid.C = 0;
							guid.D = 0;
							gpAthena->m_pPlayer->m_pPlayerPawn->EquipWeaponDefinition(gpAthena->m_pSlot5Definition, guid);
							guid.A = 5;
							guid.B = 0;
							guid.C = 0;
							guid.D = 0;
							gpAthena->m_pPlayer->m_pPlayerPawn->EquipWeaponDefinition(gpAthena->m_pSlot6Definition, guid);
							guid.A = 0;
							guid.B = 0;
							guid.C = 0;
							guid.D = 0;
							gpAthena->m_pPlayer->m_pPlayerPawn->EquipWeaponDefinition(gpAthena->m_pHarvestingToolDefinition, guid);
							gpAthena->m_pSlotLastUsedDefinition = gpAthena->m_pHarvestingToolDefinition;

							gpAthena->m_bHasPreloadedWeapons = true;
						}

						gpAthena->m_iLastUsedItem = 0;
						gpAthena->m_iLastUsedBuildingItem = 6;
						gpAthena->m_iQuickbarLastUsedBuilding = 0;
						gpAthena->m_pLastUsedBuildingDef = gpAthena->m_pWallBuildDef;
						gpAthena->m_pSlotLastUsedDefinition = gpAthena->m_pHarvestingToolDefinition;
					}
				}
			}

			// Hooks for Athena_Terrain
			else
			{
				SDK::EFortQuickBars qbPrimary = SDK::EFortQuickBars::Primary;
				SDK::EFortQuickBars qbSecondary = SDK::EFortQuickBars::Secondary;
				if (pFunction->GetName() == "Fly") {
					switch (gpAthena->m_pPlayer->m_pPlayerPawn->CharacterMovement->MovementMode) {
					default:
						gpAthena->m_pPlayer->m_pPlayerPawn->CharacterMovement->SetMovementMode(SDK::EMovementMode::MOVE_Flying, 0);
						break;
					case SDK::EMovementMode::MOVE_Flying:
						gpAthena->m_pPlayer->m_pPlayerPawn->CharacterMovement->SetMovementMode(SDK::EMovementMode::MOVE_Walking, 0);
						break;
					}
				}
				// Called every frame.
				if (pFunction->GetName().find("Tick") != std::string::npos)
				{
					if (gpAthena->m_pPlayer && gpAthena->m_pPlayer->m_pPlayerPawn && !static_cast<SDK::AFortPlayerControllerAthena*>(Globals::gpPlayerController)->IsInAircraft() && gpInventoryMapper != nullptr)
					{

						if (GetAsyncKeyState('1') & 0x8000)
						{
							SDK::FGuid guid;
							guid.A = 0;
							guid.B = 0;
							guid.C = 0;
							guid.D = 0;

							gpAthena->m_iLastUsedItem = 0;
							gpAthena->m_iSlotLastUsedRevision = 0;
							gpAthena->m_pSlotLastUsedDefinition = gpAthena->m_pHarvestingToolDefinition;

							gpAthena->m_pPlayer->m_pPlayerPawn->EquipWeaponDefinition(gpAthena->m_pHarvestingToolDefinition, guid);
						}
						if (GetAsyncKeyState('2') & 0x8000)
						{
							if (gpAthena->m_pSlot2Definition->GetFullName() != gpInventoryMapper->m_aInventoryItems[1].c_str())
							{
								auto newItemDef = SDK::UObject::FindObject<SDK::UFortWeaponRangedItemDefinition>(gpInventoryMapper->m_aInventoryItems[1]);

								gpAthena->m_pSlot2Definition = newItemDef;
								gpAthena->m_iSlot2Revision++;
							}

							SDK::FGuid guid;
							guid.A = 1;
							guid.B = gpAthena->m_iSlot2Revision;
							guid.C = 0;
							guid.D = 0;

							gpAthena->m_iLastUsedItem = 1;
							gpAthena->m_iSlotLastUsedRevision = gpAthena->m_iSlot2Revision;
							gpAthena->m_pSlotLastUsedDefinition = gpAthena->m_pSlot2Definition;
						
							gpAthena->m_pPlayer->m_pPlayerPawn->EquipWeaponDefinition(gpAthena->m_pSlot2Definition, guid);

							/*auto mum = Util::FindActor<SDK::AFortInventory>();
							auto mum2 = Util::FindActor<SDK::AFortInventory>(1);
							SDK::AFortInventory* actualInv = nullptr;

							// joe nuts
							if (mum && mum->InventoryType == SDK::EFortInventoryType::World)
							{
								actualInv = mum;
							}

							if (mum2 && mum2->InventoryType == SDK::EFortInventoryType::World)
							{
								actualInv = mum2;
							}

							if (actualInv)
							{
								auto myInventory = actualInv;

								SDK::FFortItemList* inv = &myInventory->Inventory;

								SDK::TArray<struct SDK::FFortItemEntry>* repentries = &inv->ReplicatedEntries;
								repentries->Count = 1;
								repentries->Max = 1;
								repentries->Data = (struct SDK::FFortItemEntry*)::malloc(repentries->Count * sizeof(SDK::FFortItemEntry));

								auto w00t = new SDK::FFortItemEntry;
								w00t->Count = 1;
								w00t->ItemDefinition = gpAthena->m_pSlot2Definition;
								w00t->Durability = 1;
								w00t->Level = 1;
								w00t->ItemGuid = guid;

								repentries->operator[](0) = *w00t;

								printf("repentries = %i %i\n", repentries->Num(), repentries->Max);

								SDK::TArray<class SDK::UFortWorldItem*>* iteminst = &inv->ItemInstances;
								iteminst->Count = 1;
								iteminst->Max = 1;
								iteminst->Data = (class SDK::UFortWorldItem**)::malloc(iteminst->Count * sizeof(SDK::UFortWorldItem*));

								auto lolwhat = SDK::UFortWorldItem::StaticClass()->CreateDefaultObject<SDK::UFortWorldItem>();
								lolwhat->OwnerInventory = myInventory;
								lolwhat->ItemEntry = *w00t;

								iteminst->operator[](0) = lolwhat;

								printf("iteminst = %i %i\n", iteminst->Num(), iteminst->Max);

								static_cast<SDK::AAthena_PlayerController_C*>(Globals::gpPlayerController)->bHasInitializedWorldInventory = true;
							}*/
						}
						if (GetAsyncKeyState('3') & 0x8000)
						{
							if (gpAthena->m_pSlot3Definition->GetFullName() != gpInventoryMapper->m_aInventoryItems[2].c_str())
							{
								auto newItemDef = SDK::UObject::FindObject<SDK::UFortWeaponRangedItemDefinition>(gpInventoryMapper->m_aInventoryItems[2]);

								gpAthena->m_pSlot3Definition = newItemDef;
								gpAthena->m_iSlot4Revision++;
							}

							SDK::FGuid guid;
							guid.A = 2;
							guid.B = gpAthena->m_iSlot3Revision;
							guid.C = 0;
							guid.D = 0;

							gpAthena->m_iLastUsedItem = 2;
							gpAthena->m_iSlotLastUsedRevision = gpAthena->m_iSlot3Revision;
							gpAthena->m_pSlotLastUsedDefinition = gpAthena->m_pSlot3Definition;

							gpAthena->m_pPlayer->m_pPlayerPawn->EquipWeaponDefinition(gpAthena->m_pSlot3Definition, guid);
						}
						if (GetAsyncKeyState('4') & 0x8000)
						{
							if (gpAthena->m_pSlot4Definition->GetFullName() != gpInventoryMapper->m_aInventoryItems[3].c_str())
							{
								auto newItemDef = SDK::UObject::FindObject<SDK::UFortWeaponRangedItemDefinition>(gpInventoryMapper->m_aInventoryItems[3]);
								
								gpAthena->m_pSlot4Definition = newItemDef;
								gpAthena->m_iSlot4Revision++;
							}

							SDK::FGuid guid;
							guid.A = 3;
							guid.B = gpAthena->m_iSlot4Revision;
							guid.C = 0;
							guid.D = 0;

							gpAthena->m_iLastUsedItem = 3;
							gpAthena->m_iSlotLastUsedRevision = gpAthena->m_iSlot4Revision;
							gpAthena->m_pSlotLastUsedDefinition = gpAthena->m_pSlot4Definition;

							gpAthena->m_pPlayer->m_pPlayerPawn->EquipWeaponDefinition(gpAthena->m_pSlot4Definition, guid);
						}
						if (GetAsyncKeyState('5') & 0x8000)
						{
							if (gpAthena->m_pSlot5Definition->GetFullName() != gpInventoryMapper->m_aInventoryItems[4].c_str())
							{
								auto newItemDef = SDK::UObject::FindObject<SDK::UFortWeaponRangedItemDefinition>(gpInventoryMapper->m_aInventoryItems[4]);
								
								gpAthena->m_pSlot5Definition = newItemDef;
								gpAthena->m_iSlot5Revision++;
							}

							SDK::FGuid guid;
							guid.A = 4;
							guid.B = gpAthena->m_iSlot5Revision;
							guid.C = 0;
							guid.D = 0;
							
							gpAthena->m_iLastUsedItem = 4;
							gpAthena->m_iSlotLastUsedRevision = gpAthena->m_iSlot5Revision;
							gpAthena->m_pSlotLastUsedDefinition = gpAthena->m_pSlot5Definition;

							gpAthena->m_pPlayer->m_pPlayerPawn->EquipWeaponDefinition(gpAthena->m_pSlot5Definition, guid);
						}
						if (GetAsyncKeyState('6') & 0x8000)
						{
							if (gpAthena->m_pSlot6Definition->GetFullName() != gpInventoryMapper->m_aInventoryItems[5].c_str())
							{
								auto newItemDef = SDK::UObject::FindObject<SDK::UFortWeaponRangedItemDefinition>(gpInventoryMapper->m_aInventoryItems[5]);

								gpAthena->m_pSlot6Definition = newItemDef;
								gpAthena->m_iSlot6Revision++;
							}

							SDK::FGuid guid;
							guid.A = 5;
							guid.B = gpAthena->m_iSlot6Revision;
							guid.C = 0;
							guid.D = 0;

							gpAthena->m_iLastUsedItem = 5;
							gpAthena->m_iSlotLastUsedRevision = gpAthena->m_iSlot6Revision;
							gpAthena->m_pSlotLastUsedDefinition = gpAthena->m_pSlot6Definition;

							gpAthena->m_pPlayer->m_pPlayerPawn->EquipWeaponDefinition(gpAthena->m_pSlot6Definition, guid);
						}
					}
				}

				// Called once the player jumps from the battle bus, or when they're supposed to be kicked out.
				if (pFunction->GetName().find("ServerAttemptAircraftJump") != std::string::npos || 
					pFunction->GetName().find("OnAircraftExitedDropZone") != std::string::npos)
				{
					if (static_cast<SDK::AFortPlayerControllerAthena*>(Globals::gpPlayerController)->IsInAircraft())
					{
						// Destroy all player pawns.
						Globals::gpPlayerController->CheatManager->DestroyPawns(SDK::APlayerPawn_Athena_C::StaticClass());
						// Create a new player pawn.
						gpAthena->m_pPlayer = new Player;
						gpAthena->m_pPlayer->InitializeHero(); // This causes a massive hitch.

						// Reset the pawn rotation, due to weird summon properties.
						SDK::AFortPlayerPawnAthena* playerPawn = gpAthena->m_pPlayer->m_pPlayerPawn;
						SDK::FRotator actorRotation = gpAthena->m_pPlayer->m_pPlayerPawn->K2_GetActorRotation();

						actorRotation.Pitch = 0;
						actorRotation.Roll = 0;
						playerPawn->K2_SetActorLocationAndRotation(playerPawn->K2_GetActorLocation(), actorRotation, false, true, new SDK::FHitResult());

						// Don't return null if the aircraft exited drop zone, or the storm won't start.
						if (pFunction->GetName().find("OnAircraftExitedDropZone") == std::string::npos)
							return NULL;
					}
				}
			}
		}

		return ProcessEvent(pObject, pFunction, pParams);
	}

	// Update loop, runs at 60hz, 1 tick every 16ms.
	DWORD UpdateThread(LPVOID lpParam)
	{
		while (1)
		{
			if (gpAthena->m_pPlayer != nullptr && gpAthena->m_pPlayer->m_pPlayerPawn && !static_cast<SDK::AFortPlayerControllerAthena*>(Globals::gpPlayerController)->IsInAircraft())
			{
				// Keybind to jump (only run if not skydiving, might need to fix this more though):
				if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !gpAthena->m_pPlayer->m_pPlayerPawn->IsSkydiving() && !gpAthena->m_pPlayer->m_pPlayerPawn->IsJumpProvidingForce())
				{
					gpAthena->m_pPlayer->m_pPlayerPawn->Jump();
					Sleep(200);
				}
				else if (GetAsyncKeyState(VK_SPACE) & 0x8000 && gpAthena->m_pPlayer->m_pPlayerPawn->IsSkydiving() && !gpAthena->m_pPlayer->m_pPlayerPawn->IsParachuteOpen() && !gpAthena->m_pPlayer->m_pPlayerPawn->IsParachuteForcedOpen())
				{
					gpAthena->m_pPlayer->m_pPlayerPawn->CharacterMovement->SetMovementMode(SDK::EMovementMode::MOVE_Custom, 2U);
					gpAthena->m_pPlayer->m_pPlayerPawn->OnRep_IsParachuteOpen(false);
					Sleep(200);
				}
				else if (GetAsyncKeyState(VK_SPACE) & 0x8000 && gpAthena->m_pPlayer->m_pPlayerPawn->IsSkydiving() && gpAthena->m_pPlayer->m_pPlayerPawn->IsParachuteOpen() && !gpAthena->m_pPlayer->m_pPlayerPawn->IsParachuteForcedOpen())
				{
					gpAthena->m_pPlayer->m_pPlayerPawn->CharacterMovement->SetMovementMode(SDK::EMovementMode::MOVE_Custom, 3U);
					gpAthena->m_pPlayer->m_pPlayerPawn->OnRep_IsParachuteOpen(true);
					Sleep(200);
				}

				// Keybind to sprint (only run if not skydiving & not targeting, else walk):
				if (static_cast<SDK::AAthena_PlayerController_C*>(Globals::gpPlayerController)->bWantsToSprint && gpAthena->m_pPlayer->m_pPlayerPawn && gpAthena->m_pPlayer->m_pPlayerPawn->CurrentWeapon && !gpAthena->m_pPlayer->m_pPlayerPawn->CurrentWeapon->bIsTargeting)
				{
					// If reloading, only allow sprinting if Sprinting Cancels Reload is on.
					if (gpAthena->m_pPlayer->m_pPlayerPawn->CurrentWeapon->IsReloading())
					{
						if (static_cast<SDK::AAthena_PlayerController_C*>(Globals::gpPlayerController)->bSprintCancelsReload)
						{
							gpAthena->m_pPlayer->m_pPlayerPawn->CurrentWeapon->bIsReloadingWeapon = false;
							gpAthena->m_pPlayer->m_pPlayerPawn->CurrentMovementStyle = SDK::EFortMovementStyle::Sprinting;
						}
					}
					else
						gpAthena->m_pPlayer->m_pPlayerPawn->CurrentMovementStyle = SDK::EFortMovementStyle::Sprinting;
				}
				else if (gpAthena->m_pPlayer->m_pPlayerPawn && gpAthena->m_pPlayer->m_pPlayerPawn->CurrentWeapon && !gpAthena->m_pPlayer->m_pPlayerPawn->CurrentWeapon->bIsTargeting)
					gpAthena->m_pPlayer->m_pPlayerPawn->CurrentMovementStyle = SDK::EFortMovementStyle::Running;
				else if (gpAthena->m_pPlayer->m_pPlayerPawn && gpAthena->m_pPlayer->m_pPlayerPawn->CurrentWeapon && gpAthena->m_pPlayer->m_pPlayerPawn->CurrentWeapon->bIsTargeting)
					gpAthena->m_pPlayer->m_pPlayerPawn->CurrentMovementStyle = SDK::EFortMovementStyle::Walking;
			}

			// Update thread only runs at 60hz, so we don't rape CPUs.
			Sleep(1000 / 60);
		}

		return NULL;
	}

	// Initializes the ProcessEventHook and starts the Update thread.
	DWORD WINAPI LoaderThread(LPVOID lpParam)
	{
		Util::InitSdk();
		Util::InitCore();

		auto pProcessEventAddress = Util::FindPattern("\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x6C\x24\x00\x48\x89\x9D\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC5\x48\x89\x85\x00\x00\x00\x00\x48\x63\x41\x0C", "xxxxxxxxxxxxxxx????xxxx?xxx????xxx????xxxxxx????xxxx");
		if (!pProcessEventAddress)
			Util::ThrowFatalError(L"Finding pattern for ProcessEvent has failed. Please relaunch Fortnite and try again!");

		MH_CreateHook(static_cast<LPVOID>(pProcessEventAddress), ProcessEventHook, reinterpret_cast<LPVOID*>(&ProcessEvent));
		MH_EnableHook(static_cast<LPVOID>(pProcessEventAddress));

		CreateThread(0, 0, UpdateThread, 0, 0, 0); // Create thread to handle input, etc...

		return FALSE;
	}

	Athena::Athena()
	{
		// Athena is basically a singleton, so we can't have it more than once.
		if (gpAthena)
			Util::ThrowFatalError(L"Athena is already initialized!");

		gpAthena = this;
		CreateThread(0, 0, LoaderThread, 0, 0, 0);
	}
}
