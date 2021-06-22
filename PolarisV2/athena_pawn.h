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

#ifndef ATHENA_PAWN_H
#define ATHENA_PAWN_H

#include "pawn.h"
#include <map>

namespace polaris
{
    namespace pawn
    {
        namespace pawns
        {
            //!  Player Pawn class for an AthenaPlate.
            class AthenaPawn : public Pawn
            {
            public:
                bool m_bTryingToDeployGlider = false;
                bool m_bPressedF1 = false;
                bool m_bPressedF2 = false;
                bool m_bPressedF3 = false;
                bool m_bPressedF4 = false;
                bool m_bPressed1 = false;
                bool m_bSprint = false; // naming it sprint because i have no idea what to name it stay mad
                bool m_bHasAutoCycledWall = false;
                bool m_bHasAutoCycledFloor = false;
                bool m_bHasAutoCycledStair = false;
                bool m_bHasAutoCycledRoof = false;

                SDK::UStaticMesh* m_pStaticRoof;
                SDK::UStaticMesh* m_pStaticWall;
                SDK::UStaticMesh* m_pStaticFloor;
                SDK::UBuildingEditModeMetadata_Wall* m_pMetadataWall;
                SDK::UBuildingEditModeMetadata_Roof* m_pMetadataRoof;
                SDK::UBuildingEditModeMetadata_Stair* m_pMetadataStair;
                SDK::UBuildingEditModeMetadata_Floor* m_pMetadataFloor;
                SDK::UStaticMesh* m_pStaticStair;
                SDK::ABuildingPlayerPrimitivePreview* m_pBuildPreviewRoof;
                SDK::ABuildingPlayerPrimitivePreview* m_pBuildPreviewFloor;
                SDK::ABuildingPlayerPrimitivePreview* m_pBuildPreviewStair;
                SDK::ABuildingPlayerPrimitivePreview* m_pBuildPreviewWall;
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
                AthenaPawn();
                ~AthenaPawn();

                void Update() override;
                void ApplyCustomizationLoadout(); //!< Applies skin, pickaxe and glider to pawn.
                void EquipWeapon(const char* cItemDef, int iGuid);
                void CreateBuildPreviews(); //!< Creates building previews.
            };
        }
    }
}

#endif // !ATHENA_PAWN_H
