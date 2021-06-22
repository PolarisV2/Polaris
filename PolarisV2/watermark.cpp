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

#include "watermark.h"
#include "imgui_text_extension.h"
#include "build.h"

#include <string>

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
            {
                Watermark::Watermark()
                {
                    m_bIsOpen = true;
                }

                void Watermark::Draw()
                {
                    ImGuiIO& io = ImGui::GetIO();

                    ImGui::SetNextWindowBgAlpha(0.45f); // Make window background invisible.

                    ImVec2 window_pos = ImVec2((CORNER & 1) ? io.DisplaySize.x - DISTANCE_TO_CORNER : DISTANCE_TO_CORNER, (CORNER & 2) ? io.DisplaySize.y - DISTANCE_TO_CORNER : DISTANCE_TO_CORNER);
                    ImVec2 window_pos_pivot = ImVec2((CORNER & 1) ? 1.0f : 0.0f, (CORNER & 2) ? 1.0f : 0.0f);
                    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

                    std::string verNum = POLARIS_VER;
                    std::string verEnv = POLARIS_ID;
                    std::string combined = verNum + "-" + verEnv;

                    // (irma) We want this to always render, thus the reinterpret cast.
                    // I don't care if this makes it crash trying to hide the window, cus it's probably
                    // a skid trying to hide it, anyway, so it's a nice anti-skid measure, I suppose.
                    ImGui::Begin("Watermark", reinterpret_cast<bool*>(true), WINDOW_FLAGS);
                    {
                        ImGui::Header2("Polaris ");
                        ImGui::SameLine(0, 0);
                        ImGui::Header2(combined.c_str());
                        ImGui::Text("Twitter: @ProjectPolaris_ | Discord: discord.gg/polaris");

                        ImGui::End();
                    }
                }
            }
        }
    }
}