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

#ifndef ATHENA_PLATE_H
#define ATHENA_PLATE_H

#include "peh_plate.h"
#include "athena_pawn.h"

namespace polaris
{
    namespace tables
    {
        namespace plates
        {
            //!  Manages Athena's custom behavior.
            /*!
            * This Plate is in charge of essential Athena stuff, like spawning a pawn or dropping the loading screen.
            */
            class AthenaPlate : public PehPlate
            {
            private:
                pawn::pawns::AthenaPawn* m_pPlayerPawn;
                bool m_bIsInitialized;
            public:
                SDK::FGuid* m_pgPickaxe;
                SDK::FGuid* m_pgWallBuild;
                SDK::FGuid* m_pgFloorBuild;
                SDK::FGuid* m_pgStairBuild;
                SDK::FGuid* m_pgRoofBuild;
                SDK::UFortWeaponMeleeItemDefinition* m_pPickaxeDef;
                SDK::UFortBuildingItemDefinition* m_pWallBuildDef;
                SDK::UFortBuildingItemDefinition* m_pFloorBuildDef;
                SDK::UFortBuildingItemDefinition* m_pStairBuildDef;
                SDK::UFortBuildingItemDefinition* m_pRoofBuildDef;
                SDK::ABuildingActor* m_pTrapC;
                bool bTrapDone = false;
                std::map<SDK::FGuid*, SDK::UFortWeaponItemDefinition*> m_mItems;
                std::map<SDK::FGuid*, SDK::UFortTrapItemDefinition*> m_mTraps;
                int iInventoryIteration = 0;
                bool m_bOnWall = false;
                bool m_bOnFloor = false;
                bool m_bOnStair = false;
                bool m_bOnRoof = false;

                void ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams) override;
                void Update() override;

                void OnEnabled() override;

                void Initialize(); //!< Initialize spawns a pawn and drops the loading screen.
                void SetupInventory(); //!< SetupInventory initializes the inventory.
                void SetupQuickbars(); //!< SetupQuickbars initializes the quickbars.

                bool AreGuidsTheSame(SDK::FGuid guidA,SDK::FGuid guidB); //!< AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
            };
        }
    }
}

#endif // !ATHENA_PLATE_H
