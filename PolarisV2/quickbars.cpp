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

#include "quickbars.h"
#include "globals.h"
#include "error_utils.h"
#include "sdk_utils.h"
#include "program.h"
#include <MinHook.h>
#include <vector>
#include <map>
#include <iostream>

struct AFortAsQuickBars
{
public:
    unsigned char                                      UnknownData00[0x1A88];
    class SDK::AFortQuickBars* QuickBars;
};
namespace polaris::inventory
{
    void inventory::quickbars::SetupQuickbars()
    {
        auto controller = static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController);
        auto asfortquickbars = reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController);
        polaris::globals::gpPlayerController->CheatManager->Summon(TEXT("FortQuickBars"));
        asfortquickbars->QuickBars = static_cast<SDK::AFortQuickBars*>(utilities::SDKUtils::FindActor(SDK::AFortQuickBars::StaticClass()));
        asfortquickbars->QuickBars->SetOwner(polaris::globals::gpPlayerController);
        auto pQuickBars = static_cast<SDK::AFortQuickBars*>(utilities::SDKUtils::FindActor(SDK::AFortQuickBars::StaticClass()));
        controller->Role = SDK::ENetRole::ROLE_None;
        controller->OnRep_QuickBar();
        controller->Role = SDK::ENetRole::ROLE_Authority;
        asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 0);
        asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 1);
        asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 2);
        asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 3);
        asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 4);
        asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 5);
        asfortquickbars->QuickBars->EnableSlot(SDK::EFortQuickBars::Primary, 0);
        pQuickBars->ServerAddItemInternal((*m_pgWallBuild), SDK::EFortQuickBars::Secondary, 0);
        pQuickBars->ServerAddItemInternal((*m_pgFloorBuild), SDK::EFortQuickBars::Secondary, 1);
        pQuickBars->ServerAddItemInternal((*m_pgStairBuild), SDK::EFortQuickBars::Secondary, 2);
        pQuickBars->ServerAddItemInternal((*m_pgRoofBuild), SDK::EFortQuickBars::Secondary, 3);
        pQuickBars->ServerAddItemInternal((*m_pgPickaxe), SDK::EFortQuickBars::Primary, 0);
        pQuickBars->ServerActivateSlotInternal(SDK::EFortQuickBars::Primary, 0, 0, true);
    }
}
