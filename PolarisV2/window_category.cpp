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

#include "window_category.h"
#include "globals.h"
#include "program.h"
#include "sdk_utils.h"

namespace polaris::ui::window::windows::mainwindow::category::categories
{
    WindowCategory::WindowCategory()
    {
        m_cCategoryName = "Window";

        m_pWorldInspector = new WorldInspector;
        m_pWorldInspector->m_bIsOpen = false;
    }

    void WindowCategory::DrawButtons()
    {
        if (ImGui::MenuItem("World Inspector"))
        {
            m_pWorldInspector->m_bIsOpen = !m_pWorldInspector->m_bIsOpen;
        }
    }
}