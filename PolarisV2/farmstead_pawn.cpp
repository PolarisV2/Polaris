#ifndef _PROD
#ifndef _BETA

#include "farmstead_pawn.h"
#include "globals.h"
#include "sdk_utils.h"

namespace polaris
{
    namespace pawn
    {
        namespace pawns
        {
            FarmsteadPawn::FarmsteadPawn()
            {
                globals::gpPlayerController->CheatManager->Summon(TEXT("PlayerPawn_Generic_C"));
                m_pPawnActor = static_cast<SDK::APlayerPawn_Generic_C*>(utilities::SDKUtils::FindActor(SDK::APlayerPawn_Generic_C::StaticClass()));
                globals::gpPlayerController->Possess(m_pPawnActor);

                // Load Ramirez onto the pawn and replicate character parts.
                auto playerState = static_cast<SDK::AFortPlayerStateZone*>(globals::gpPlayerController->PlayerState);
                playerState->OnRep_CharacterParts();
                m_pPawnActor->OnCharacterPartsReinitialized();

                //Reset the pawn's actor rotation.
                SDK::FRotator actorRotation = m_pPawnActor->K2_GetActorRotation();
                actorRotation.Pitch = 0;
                actorRotation.Roll = 0;

                m_pPawnActor->K2_SetActorLocationAndRotation(m_pPawnActor->K2_GetActorLocation(), actorRotation, false, true, new SDK::FHitResult());
            }
            FarmsteadPawn::~FarmsteadPawn()
            {
                if (m_pPawnActor != nullptr)
                    m_pPawnActor->K2_DestroyActor();
            }

            void FarmsteadPawn::Update()
            {
                Pawn::Update();

                bool wantsToSprint = static_cast<SDK::AFortPlayerControllerZone*>(globals::gpPlayerController)->bWantsToSprint;
                m_pPawnActor->CurrentMovementStyle = wantsToSprint ? SDK::EFortMovementStyle::Sprinting : SDK::EFortMovementStyle::Running;

            }
        }
    }
}

#endif
#endif