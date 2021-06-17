#include "player.h"

struct AFortAsQuickBars
{
public:
	unsigned char                                      UnknownData00[0x1A88];
	class SDK::AFortQuickBars*                         QuickBars;
};

namespace polaris
{
	static SDK::UCustomCharacterPart* m_pCustomCharacterPartHead;
	static SDK::UCustomCharacterPart* m_pCustomCharacterPartBody;
	static SDK::UCustomCharacterPart* m_pCustomCharacterPartHat;
	std::map<std::string, std::string> mPickaxeAsWid
	{
		{"DefaultPickaxe", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01"},
		{"BoltonPickaxe", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_BoltOn_Athena_C_T01.WID_Harvest_Pickaxe_BoltOn_Athena_C_T01"},
		{"HalloweenScythe", "FortWeaponMeleeItemDefinition WID_Harvest_HalloweenScythe_Athena_C_T01.WID_Harvest_HalloweenScythe_Athena_C_T01"},
		{"HappyPickaxe", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Smiley_Athena_C_T01.WID_Harvest_Pickaxe_Smiley_Athena_C_T01"},
		{"Pickaxe_Deathvalley", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Deathvalley_Athena_C_T01.WID_Harvest_Pickaxe_Deathvalley_Athena_C_T01"},
		{"Pickaxe_Flamingo", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Flamingo_Athena_C_T01.WID_Harvest_Pickaxe_Flamingo_Athena_C_T01"},
		{"Pickaxe_Lockjaw", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Lockjaw_Athena_C_T01.WID_Harvest_Pickaxe_Lockjaw_Athena_C_T01"},
		{"SickleBatPickaxe", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_SickleBat_Athena_C_T01.WID_Harvest_Pickaxe_SickleBat_Athena_C_T01"},
		{"SkiIcePickaxe", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_IcePick_Athena_C_T01.WID_Harvest_Pickaxe_IcePick_Athena_C_T01"},
		{"SpikyPickaxe", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Spikey_Athena_C_T01.WID_Harvest_Pickaxe_Spikey_Athena_C_T01"}
	};

	// Search an object in the UObject cache.
	template<typename T>
	static T* FindObject(const std::string& sClassName, const std::string& sQuery)
	{
		for (int i = 0; i < SDK::UObject::GetGlobalObjects().Num(); ++i)
		{
			auto pObject = SDK::UObject::GetGlobalObjects().GetByIndex(i);
			if (pObject != nullptr && pObject->GetFullName().find("F_Med_Head1") == std::string::npos)
			{
				if (pObject->GetFullName().rfind(sClassName, 0) == 0 && pObject->GetFullName().find(sQuery) != std::string::npos)
					return static_cast<T*>(pObject);
			}
		}

		return nullptr;
	}

	Player::Player()
	{
		std::string sQuickBarsClassName = "FortQuickBars";
		Globals::gpPlayerController->CheatManager->Summon(SDK::FString(std::wstring(sQuickBarsClassName.begin(), sQuickBarsClassName.end()).c_str()));

		reinterpret_cast<AFortAsQuickBars*>(Globals::gpPlayerController)->QuickBars = static_cast<SDK::AFortQuickBars*>(Util::FindActor<SDK::AFortQuickBars>());
		reinterpret_cast<AFortAsQuickBars*>(Globals::gpPlayerController)->QuickBars->SetOwner(Globals::gpPlayerController);

		static_cast<SDK::AAthena_PlayerController_C*>(Globals::gpPlayerController)->Role = SDK::ENetRole::ROLE_None;
		static_cast<SDK::AAthena_PlayerController_C*>(Globals::gpPlayerController)->OnRep_QuickBar();
		static_cast<SDK::AAthena_PlayerController_C*>(Globals::gpPlayerController)->Role = SDK::ENetRole::ROLE_Authority;

		// Summon a new PlayerPawn.
		std::string sPawnClassName = "PlayerPawn_Athena_C";
		Globals::gpPlayerController->CheatManager->Summon(SDK::FString(std::wstring(sPawnClassName.begin(), sPawnClassName.end()).c_str()));

		m_pPlayerPawn = static_cast<SDK::APlayerPawn_Athena_C*>(Util::FindActor<SDK::APlayerPawn_Athena_C>());
		if (!m_pPlayerPawn)
		{
			Util::ThrowFatalError(L"Failed to spawn PlayerPawn!");
		}
		else
		{
			Globals::gpPlayerController->Possess(m_pPlayerPawn);

			auto controller = static_cast<SDK::AFortPlayerControllerAthena*>(Globals::gpPlayerController);
			controller->bIsLocalPlayerController = true;
			controller->SetReplicates(false);

			// Tell the PlayerPawn that our customization was replicated.
			m_pPlayerPawn->OnRep_CustomizationLoadout();

			// Assign our PlayerPawn to a team.
			auto playerState = static_cast<SDK::AFortPlayerStateAthena*>(Globals::gpPlayerController->PlayerState);

			playerState->TeamIndex = SDK::EFortTeam::HumanPvP_Team1;
			playerState->OnRep_TeamIndex();

			// Give the player a pickaxe.
			EquipWeapon(mPickaxeAsWid[m_pPlayerPawn->CustomizationLoadout.Character->GetName()].c_str());
		}
	}
	
	// FIXME (irma) Replace this with a proper Skin Loader.
	void Player::InitializeHero()
	{
		auto pPlayerState = static_cast<SDK::AFortPlayerStateAthena*>(Globals::gpPlayerController->PlayerState);
		
		if (!m_pCustomCharacterPartHead)
			m_pCustomCharacterPartHead = FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart", "Head");
		if (!m_pCustomCharacterPartBody)
			m_pCustomCharacterPartBody = FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart", "Body");
		if (!m_pCustomCharacterPartHat)
			m_pCustomCharacterPartHat = FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart", "Hat_");

		// Assign custom character parts to the player.
		pPlayerState->CharacterParts[0] = m_pCustomCharacterPartHead;
		pPlayerState->CharacterParts[1] = m_pCustomCharacterPartBody;
		pPlayerState->CharacterParts[3] = m_pCustomCharacterPartHat;

		// If no head was found, force Ramirez's head.
		if (!pPlayerState->CharacterParts[0])
			pPlayerState->CharacterParts[0] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1");
		
		// If no body was found, force Ramirez's body.
		if (!pPlayerState->CharacterParts[1])
			pPlayerState->CharacterParts[1] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01");

		static_cast<SDK::AFortPlayerStateAthena*>(Globals::gpPlayerController->PlayerState)->OnRep_CharacterParts();
	}

	// Equip a weapon.
	void Player::EquipWeapon(const char* cItemDef, int iGuid)
	{
		SDK::FGuid guid;
		guid.A = iGuid;
		guid.B = iGuid;
		guid.C = iGuid;
		guid.D = iGuid;

		m_pPlayerPawn->EquipWeaponDefinition(SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>(cItemDef), guid)->SetOwner(m_pPlayerPawn);
	}
}