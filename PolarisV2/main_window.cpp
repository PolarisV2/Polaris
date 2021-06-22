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

#include "main_window.h"
#include "program.h"
#include "sdk_utils.h"
#include "globals.h"
#include "ui_renderer.h"
#include "changelog_window.h"

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
            {
                MainWindow::MainWindow()
                {
                    m_pAboutWindow = new AboutWindow;
                    m_pWorldInspector = new WorldInspector;
                    m_pWatermark = new Watermark;
                    new ChangelogWindow;

                    m_bIsUE4ConsoleEnabled = false;
                    m_pAboutWindow->m_bIsOpen = false;
                    m_pWorldInspector->m_bIsOpen = false;
                }

                void MainWindow::Draw()
                {
                    ImGui::BeginMainMenuBar();
                    {
                        if (ImGui::BeginMenu("Player"))
                        {
                            if (!globals::gpLocalPlayer->ViewportClient->ViewportConsole || !m_bIsUE4ConsoleEnabled)
                            {
                                if (ImGui::MenuItem("Enable Console"))
                                {
                                    utilities::SDKUtils::ToggleUnrealConsole(true);
                                    m_bIsUE4ConsoleEnabled = true;
                                }
                            }
                            else if(globals::gpLocalPlayer->ViewportClient->ViewportConsole || m_bIsUE4ConsoleEnabled)
                            {
                                if (ImGui::MenuItem("Disable Console"))
                                {
                                    utilities::SDKUtils::ToggleUnrealConsole(false);
                                    m_bIsUE4ConsoleEnabled = false;
                                }
                            }

                            if (ImGui::MenuItem("Exit"))
                            {
                                gpProgram->~Program();
                            }
                            ImGui::EndMenu();
                        }
                        if (ImGui::BeginMenu("Windows"))
                        {
                            if (ImGui::MenuItem("World Inspector"))
                            {
                                m_pWorldInspector->m_bIsOpen = !m_pWorldInspector->m_bIsOpen;
                            }
                            ImGui::EndMenu();
                        }
                        if (ImGui::BeginMenu("Help"))
                        {
                            if (ImGui::MenuItem("About"))
                            {
                                m_pAboutWindow->m_bIsOpen = !m_pAboutWindow->m_bIsOpen;
                            }
                            ImGui::EndMenu();
                        }
                        ImGui::EndMainMenuBar();
                    }
                }
                void MainWindow::Update()
                {
                    m_bIsOpen = gpRenderer->m_bLockFortInput;
                }
            }
        }
    }
}