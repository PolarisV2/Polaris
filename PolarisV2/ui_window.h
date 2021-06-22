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

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            //!  Base class for an ImGui window.
            /*!
            * Contains functions like Draw, Resize and Update, called by the UIRenderer.
            */
            class UIWindow
            {
            public:
                bool m_bIsOpen;

                UIWindow();

                virtual void Draw(); //!< Draw is called each tick while the window is open.
                virtual void Resize(); //!< Resize is called when the Fortnite window is resized, regardless of the window being open or closed.
                virtual void Update(); //!< Update is called each tick, regardless of the window being open or closed.
            };
        }
    }
}

#endif // !UI_WINDOW_H
