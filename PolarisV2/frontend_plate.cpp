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

#include "frontend_plate.h"
#include "globals.h"
#include "program.h"
#include "athena_plate.h"

#include <iostream>

namespace polaris
{
    namespace tables
    {
        namespace plates
        {
            void FrontendPlate::OnEnabled()
            {
                std::cout << "We're now in the Frontend State." << std::endl;

                if (globals::gpLevel->URL.Map.ToString() != "/Game/Maps/FortniteEntry")
                    globals::gpPlayerController->SwitchLevel(TEXT("Frontend"));
            }

            void FrontendPlate::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
            {
                if (pFunction->GetName().find("BP_PlayButton") != std::string::npos)
                    gpProgram->m_pMainTable->PushPlate(new AthenaPlate);
            }
        }
    }
}