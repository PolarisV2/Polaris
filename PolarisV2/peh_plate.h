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

#ifndef PEH_PLATE_H
#define PEH_PLATE_H

#include "plate.h"
#include "SDK.hpp"

#include <windows.h>

namespace polaris
{
    namespace tables
    {
        namespace plates
        {
            //!  The base class for a PehPlate.
            /*!
            * A PehPlate is a type of Plate which can subscribe to a PehTable (ProcessEventHook Table).
            */
            class PehPlate : public Plate
            {
            public:
                virtual void ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams); //!< ProcessEventHook is called every time an event is triggered while the plate is primary on the stack.
            };
        }
    }
}

#endif // !PEH_PLATE_H
