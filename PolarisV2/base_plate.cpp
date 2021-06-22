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

#include "base_plate.h"
#include "sdk_utils.h"
#include "program.h"
#include "frontend_plate.h"
#include "error_utils.h"
#include "ui_renderer.h"
#include "main_window.h"

namespace polaris
{
    namespace tables
    {
        namespace plates
        {
            DWORD LoadThread(LPVOID lpParam)
            {
                utilities::SDKUtils::InitSdk();

                uintptr_t pBaseAddress = utilities::SDKUtils::BaseAddress();
                if (!pBaseAddress)
                    utilities::ErrorUtils::ThrowException(L"BaseAddress was not found.");

                // NOTE: We wait until this is not null. This becomes a valid pointer as soon as
                // the initial loading screen drops. From then, we can continue initializing Polaris.
                while ((*globals::gpWorld) == nullptr)
                    Sleep(1000 / 60);

                // NOTE: For some reason if you don't wait a bit here, everything will be nullptr.
                Sleep(500);

                utilities::SDKUtils::InitGlobals();
                gpProgram->m_pMainTable->PushPlate(new FrontendPlate);

                // Initialize the renderer and main window.
                std::cout << "Initializing UI." << std::endl;
                new ui::UIRenderer;
                new ui::window::windows::MainWindow;
            }

            void BasePlate::OnEnabled()
            {
                std::cout << "We're now in the Base State.\nWaiting for game to load..." << std::endl;

                // Check the NOTE in LoadThread to see the reasoning as to
                // why this is in its separate thread.
                CreateThread(0, 0, LoadThread, 0, 0, 0);
            }
        }
    }
}