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

#include "player_category.h"
#include "globals.h"
#include "program.h"
#include "sdk_utils.h"

namespace polaris::ui::window::windows::mainwindow::category::categories
{
    PlayerCategory::PlayerCategory()
    {
        m_cCategoryName = "Player";
    }

    void PlayerCategory::DrawButtons()
    {
        if (!globals::gpLocalPlayer->ViewportClient->ViewportConsole)
        {
            if (ImGui::MenuItem("Enable Console"))
            {
                utilities::SDKUtils::ToggleUnrealConsole(true);
            }
        }
        else if (globals::gpLocalPlayer->ViewportClient->ViewportConsole)
        {
            if (ImGui::MenuItem("Disable Console"))
            {
                utilities::SDKUtils::ToggleUnrealConsole(false);
            }
        }

        if (ImGui::MenuItem("Exit"))
        {
            ExitProcess(0);
        }
    }
}