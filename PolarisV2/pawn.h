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

#ifndef PAWN_H
#define PAWN_H

#include "SDK.hpp"

#include <windows.h>

namespace polaris
{
    namespace pawn
    {
        //!  Pawn class for a PehPlate.
        /*!
        * Used to separate Pawn logic from the a PehPlate's update loops. Call a pawn's Update and ProcessEventHook functions in a PehPlate.
        */
        class Pawn
        {
        public:
            SDK::AFortPlayerPawn* m_pPawnActor = nullptr; //!< The Pawn's actor.
            bool m_bHasJumped = false; //!< True when space is being held down and the pawn has already finished a jump.

            virtual void ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams); //!< ProcessEventHook is used to hook behavior to ProcessEvent calls.
            virtual void Update(); //!< Update should be called in a PehPlate's Update function.
        };
    }
}

#endif // !PAWN_H
