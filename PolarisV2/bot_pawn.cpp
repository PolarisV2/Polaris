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

#include "bot_pawn.h"
#include "athena_pawn.h"
#include "globals.h"
#include "sdk_utils.h"

#include <iostream>
#include <fstream>

namespace polaris::pawn::pawns
{
    static SDK::UCustomCharacterPart* pCharacterPartBody;
    static SDK::UCustomCharacterPart* pCharacterPartHead;
    static SDK::UCustomCharacterPart* pCharacterPartHat;

    template<typename T>
    T* FindObject(const std::string& sClassName, const std::string& sQuery)
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
    void BotPawn::Construct() 
    {
        //Spawn the pawn and player controller
        m_pBotPawnActor = static_cast<SDK::AFortPlayerPawnAthena*>(utilities::SDKUtils::SpawnActor(SDK::APlayerPawn_Generic_C::StaticClass(), &m_pPos, &m_pRot));
        if (m_pBotPawnActor != nullptr) {
            m_pBotPawnController = static_cast<SDK::AFortPlayerControllerAthena*>(utilities::SDKUtils::SpawnActor(SDK::AAthena_PlayerController_C::StaticClass(), &m_pPos, &m_pRot));
            m_pBotPawnController->Player = static_cast<SDK::UPlayer*>(globals::StaticConstructObject_Internal(SDK::UPlayer::StaticClass(),m_pBotPawnController, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
            m_pBotPawnController->Player->PlayerController = m_pBotPawnController;
            //Set the player controller's profile, inventory and possess the pawn
            m_pBotPawnController->AthenaProfile = m_pControllerToGetProfile->AthenaProfile;
            m_pBotPawnController->MainMcpProfile = m_pControllerToGetProfile->MainMcpProfile;
            m_pBotPawnController->McpProfileGroup = m_pControllerToGetProfile->McpProfileGroup;
            m_pBotPawnController->MetadataProfile = m_pControllerToGetProfile->MetadataProfile;
            m_pBotPawnController->OutpostInventory = m_pInventory;
            m_pBotPawnController->ClientForceProfileQuery();
            m_pBotPawnController->Possess(m_pBotPawnActor);
            m_pBotPawnActor->PawnUniqueID = rand() % 1000;
            m_pBotPawnActor->OnRep_PawnUniqueID();
            //Spawn the player state and apply it
            m_pBotPawnActor->PlayerState = static_cast<SDK::AFortPlayerStateAthena*>(utilities::SDKUtils::SpawnActor(SDK::AFortPlayerStateAthena::StaticClass(), &m_pPos, &m_pRot));
            m_pBotPawnActor->OnRep_PlayerState();
            m_pBotPawnController->PlayerState = m_pBotPawnActor->PlayerState;
            m_pBotPawnController->OnRep_PlayerState();
            //set the name
            m_pBotPawnController->ServerChangeName(SDK::FString(std::wstring(m_sName.begin(), m_sName.end()).c_str()));
            m_pPlayerState = static_cast<SDK::AFortPlayerStateAthena*>(m_pBotPawnController->PlayerState);
            //set hero shit
            m_pPlayerState->HeroType = static_cast<SDK::AFortPlayerStateAthena*>(globals::gpPlayerController->PlayerState)->HeroType;
            m_pPlayerState->HeroId = static_cast<SDK::AFortPlayerStateAthena*>(globals::gpPlayerController->PlayerState)->HeroId;
            m_pPlayerState->OnRep_HeroId();
            m_pPlayerState->OnRep_HeroType();
            //set health and movement style
            m_pBotPawnActor->SetMaxHealth(m_iHealth);
            m_pBotPawnActor->SetHealth(m_iHealth);
            m_pBotPawnActor->CurrentMovementStyle = SDK::EFortMovementStyle::Sprinting;
            m_pBotPawnActor->PlayerState->bIsSpectator = false;
            m_pBotPawnActor->PlayerState->bIsABot = true;
            //Set values to prevent you from being kicked from the game due to a bot not being "loaded in"
            m_pPlayerState->bHasFinishedLoading = true;
            m_pPlayerState->bHasStartedPlaying = true;
            m_pPlayerState->OnRep_bHasStartedPlaying();
            m_pBotPawnController->ServerSetClientHasFinishedLoading(true);
            m_pBotPawnController->ServerLoadingScreenDropped();
            m_pBotPawnController->bHasInitiallySpawned = true;
            m_pBotPawnController->bAssignedStartSpawn = true;
            m_pBotPawnController->bReadyToStartMatch = true;
            m_pBotPawnController->bClientPawnIsLoaded = true;
            m_pBotPawnController->bHasClientFinishedLoading = true;
            m_pBotPawnController->bHasServerFinishedLoading = true;
            //toggle infinite ammo
            static_cast<SDK::UFortCheatManager*>(m_pBotPawnController->CheatManager)->ToggleInfiniteAmmo();

            auto playerState = static_cast<SDK::AFortPlayerStateAthena*>(m_pBotPawnController->PlayerState);

            playerState->TeamIndex = m_pTeam;
            static_cast<SDK::AFortGameModeAthena*>((*globals::gpWorld)->AuthorityGameMode)->bPlayersInvincible = false;
            playerState->OnRep_TeamIndex();

            //auto pawn = static_cast<SDK::AFortPlayerPawnAthena*>(m_pBotPawnActor);
            //pawn->OnRep_CustomizationLoadout();

            // Give the player the chosen weapon.
            if (m_pWep != nullptr)
            {
                m_pBotPawnActor->EquipWeaponDefinition(m_pWep, SDK::FGuid());
            }
            // Apply customization loadout.
            ApplyCustomizationLoadout();
            if (m_bIsFiring) {
                m_pBotPawnActor->PawnStartFire(1);
            }
        }
        m_bHasConstructed = true;
    }
    BotPawn::BotPawn(SDK::FVector pos, SDK::FRotator rot, std::string name, int health, SDK::TEnumAsByte<SDK::EFortTeam> Team, SDK::UFortWeaponItemDefinition* wep, SDK::AFortPlayerController* controllerToGetProfile, SDK::AFortInventory* inventory, bool isFiring)
    {
       m_pPos = pos;
       m_pRot = rot;
       m_sName = name;
       m_iHealth = health;
       m_pTeam = Team;
       m_pWep = wep;
       m_pControllerToGetProfile = controllerToGetProfile;
       m_pInventory = inventory;
       m_bIsFiring = isFiring;
    }
    BotPawn::~BotPawn()
    {
        if (m_pBotPawnActor != nullptr)
            m_pBotPawnActor->K2_DestroyActor();
    }
    void BotPawn::ApplyCustomizationLoadout()
    {
        auto botplayerState = static_cast<SDK::AFortPlayerStateAthena*>(m_pBotPawnController->PlayerState);
        auto playerState = static_cast<SDK::AFortPlayerState*>(globals::gpPlayerController->PlayerState);

        if (!pCharacterPartHead)
            pCharacterPartHead = FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart", "Head");
        if (!pCharacterPartBody)
            pCharacterPartBody = FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart", "Body");
        if (!pCharacterPartHat)
            pCharacterPartHat = FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart", "Hat_");

        // Assign custom character parts to the player.
        botplayerState->CharacterParts[0] = pCharacterPartHead;
        botplayerState->CharacterParts[1] = pCharacterPartBody;
        botplayerState->CharacterParts[3] = pCharacterPartHat;
        
        // If no head was found, force Ramirez's head.
        if (!botplayerState->CharacterParts[0])
            botplayerState->CharacterParts[0] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1");
        
        // If no body was found, force Ramirez's body.
        if (!botplayerState->CharacterParts[1])
            botplayerState->CharacterParts[1] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01");

        botplayerState->OnRep_CharacterParts();
    }
    void BotPawn::EquipWeapon(const char* cItemDef, int iGuid)
    {
        SDK::FGuid guid;
        guid.A = iGuid;
        guid.B = iGuid;
        guid.C = iGuid;
        guid.D = iGuid;

        m_pBotPawnActor->EquipWeaponDefinition(SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>(cItemDef), guid)->SetOwner(m_pBotPawnActor);
    }
}