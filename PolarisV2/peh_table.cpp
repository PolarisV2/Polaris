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

#include "peh_table.h"
#include "SDK.hpp"
#include "sdk_utils.h"
#include "peh_plate.h"
#include "error_utils.h"

#include <windows.h>
#include <MinHook.h>
#pragma comment(lib, "libMinHook.x64.lib")

polaris::tables::PehTable* gpPehTable;

namespace polaris
{
    namespace tables
    {
        PVOID(*ProcessEvent)(SDK::UObject*, SDK::UFunction*, PVOID) = nullptr;
        PVOID ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
        {
            if (pObject != nullptr && pFunction != nullptr)
            {
                if (gpPehTable->m_lpPlates.size() > 0)
                {
                    tables::plates::PehPlate* primaryTable = dynamic_cast<tables::plates::PehPlate*>(gpPehTable->m_lpPlates.front());

                    primaryTable->Update();
                    primaryTable->ProcessEventHook(pObject, pFunction, pParams);
                }
            }

            return ProcessEvent(pObject, pFunction, pParams);
        }

        PehTable::PehTable()
        {
            if (gpPehTable != nullptr)
                utilities::ErrorUtils::ThrowException(L"gpPehTable is not null.");

            gpPehTable = this;

            auto pProcessEventAddress = utilities::SDKUtils::FindPattern("\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x6C\x24\x00\x48\x89\x9D\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC5\x48\x89\x85\x00\x00\x00\x00\x48\x63\x41\x0C", "xxxxxxxxxxxxxxx????xxxx?xxx????xxx????xxxxxx????xxxx");
            if (!pProcessEventAddress)
                utilities::ErrorUtils::ThrowException(L"ProcessEvent address was not found.");

            MH_CreateHook(static_cast<LPVOID>(pProcessEventAddress), ProcessEventHook, reinterpret_cast<LPVOID*>(&ProcessEvent));
            MH_EnableHook(static_cast<LPVOID>(pProcessEventAddress));
        }
    }
}