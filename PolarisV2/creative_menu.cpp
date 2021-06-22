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

#include "creative_menu.h"
#include "imgui_text_extension.h"
#include "athena_plate.h"
#include "sdk_utils.h"
#include "program.h"
#include "json.hpp"
#include "SDK.hpp"
#include "trap_build.h"

#include <iostream>
#include <fstream>

namespace fs = std::filesystem;
using trapbuild = polaris::models::offsetfixes::TrapBuild;
using json = nlohmann::json;

namespace polaris::ui::window::windows
{
    static char cName[128];

    CreativeMenu::CreativeMenu(tables::plates::CreativePlate* creativePlate)
    {
        m_pCreativePlate = creativePlate;
    }

    void CreativeMenu::Draw()
    {
        ImGuiIO* io = &ImGui::GetIO();

        // Background darkener window
        ImGui::SetNextWindowSize(ImVec2(5000, 5000));
        ImGui::SetNextWindowPos(ImVec2(-(io->DisplaySize.x / 2), -(io->DisplaySize.y / 2)));
        ImGui::SetNextWindowBgAlpha(0.65f);
        ImGui::Begin("Poglaris Moment", &m_bIsOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus);
        {
            ImGui::End();
        }

        ImGui::SetNextWindowSize(ImVec2(631, 410), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(((io->DisplaySize.x / 2) - (631 / 2)), ((io->DisplaySize.y / 2) - (410 / 2))));
        ImGui::Begin("Creations Hub", &m_bIsOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
        {
            ImGui::Header2("Creations Hub");
            ImGui::Text("What would you like to play?");

            ImGui::BeginChild("Gamemode List", ImVec2(600, 250), true);
            {
                std::string path = "creations";

                for (const auto& entry : fs::directory_iterator(path))
                {
                    std::ifstream streamData(entry.path());
                    json jsonData;
                    streamData >> jsonData;

                    if (ImGui::Selectable(std::string(jsonData["info"]["name"]).c_str()))
                    {
                        m_pCreativePlate->LoadCreation(entry.path());
                    }
                }
                ImGui::EndChild();
            }

            if (ImGui::Button("New creation..."))
            {
                CreateNew();
            }
            ImGui::SameLine();
            ImGui::Button("Open creations folder");
#ifndef _PROD
            ImGui::SameLine();
            if (ImGui::Button("Debug Quick Save"))
            {
                m_pCreativePlate->SaveCreation("Debug");
            }
#endif // !_PROD

            ImGui::End();
        }

        if (m_bCreatingNewProject)
        {
            ImGui::Begin("New Creation", &m_bCreatingNewProject);
            {
                ImGui::InputTextWithHint("Name", "e.g. My Epic Creation", cName, IM_ARRAYSIZE(cName));
                if (ImGui::Button("Create!"))
                {
                    m_pCreativePlate->SaveCreation(cName);
                    m_bCreatingNewProject = false;
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    m_bCreatingNewProject = false;
                }

                ImGui::End();
            }
        }
    }

    void CreativeMenu::CreateNew()
    {
        m_bCreatingNewProject = true;
    }
}
