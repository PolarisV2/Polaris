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

#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "ui_window.h"

#include <vector>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

namespace polaris
{
    namespace ui
    {
        //!  Renders D3D11 on top of the Fortnite window.
        /*!
        * In charge of rendering ImGui windows on top of the Fortnite window using D3D11.
        */
        class UIRenderer
        {
        public:
            std::vector<window::UIWindow*> m_vpWindows;
            ID3D11Device* m_pCurrentDevice = nullptr;
            ID3D11DeviceContext* m_pCurrentContext = nullptr;
            ID3D11RenderTargetView* m_pCurrentView = nullptr;
            HWND m_hWnd = 0;
            WNDPROC m_lpPrevWndFunc;
            bool m_bLockFortInput;

            UIRenderer();
            ~UIRenderer();

            void SetTheme(); //!< Applies the custom Polaris theme to ImGui.
        };
    }
}

#endif // !UI_RENDERER_H

extern polaris::ui::UIRenderer* gpRenderer;