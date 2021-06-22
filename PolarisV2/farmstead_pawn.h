#ifndef _PROD
#ifndef _BETA

#ifndef FARMSTEAD_PAWN_H
#define FARMSTEAD_PAWN_H

#include "pawn.h"

namespace polaris
{
    namespace pawn
    {
        namespace pawns
        {
            //!  Player Pawn class for a FarmsteadPlate.
            class FarmsteadPawn : public Pawn
            {
            public:
                FarmsteadPawn();
                ~FarmsteadPawn();

                void Update() override;
            };
        }
    }
}

#endif // !FARMSTEAD_PAWN_H

#endif
#endif