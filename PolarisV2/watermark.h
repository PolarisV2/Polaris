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

#ifndef WATERMARK_H
#define WATERMARK_H

#include "ui_window.h"

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
            namespace windows
            {
                class Watermark : public UIWindow
                {
                public:
                    const ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoFocusOnAppearing |
                                                          ImGuiWindowFlags_AlwaysAutoResize   |
                                                          ImGuiWindowFlags_NoSavedSettings    |
                                                          ImGuiWindowFlags_NoDecoration       |
                                                          ImGuiWindowFlags_NoMove             |
                                                          ImGuiWindowFlags_NoNav;

                    const int CORNER = 2; /*
                                            0 = Top left
                                            1 = Top right
                                            2 = Bottom left
                                            3 = Bottom right
                                          */

                    const float DISTANCE_TO_CORNER = 15.0f;

                    Watermark();
                    void Draw() override;
                };
            }
        }
    }
}

#endif // WATERMARK_H