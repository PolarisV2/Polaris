#ifndef _PROD
#ifndef _BETA

#ifndef FARMSTEAD_PLATE_H
#define FARMSTEAD_PLATE_H

#include "peh_plate.h"
#include "farmstead_pawn.h"

namespace polaris
{
    namespace tables
    {
        namespace plates
        {
            //!  Manages Farmstead's custom behavior.
            /*!
            * ez pirate StW with this plate.
            * Not included in prod or beta builds.
            */
            class FarmsteadPlate : public PehPlate
            {
            private:
                pawn::pawns::FarmsteadPawn* m_pPlayerPawn;
                bool m_bIsInitialized;

            public:
                void ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams) override;
                void Update() override;

                void OnEnabled() override;

                void Initialize(); //!< Initialize spawns a pawn gives it a pickaxe.
            };
        }
    }
}

#endif // !FARMSTEAD_PLATE_H

#endif
#endif