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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ui_window.h"
#include "about_window.h"
#include "world_inspector.h"
#include "watermark.h"

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
            {
                //!  The main menu bar.
                /*!
                * In charge of giving users access to the Polaris tools.
                */
                class MainWindow : public UIWindow
                {
                public:
                    AboutWindow* m_pAboutWindow;
                    WorldInspector* m_pWorldInspector;
                    Watermark* m_pWatermark;

                    bool m_bIsUE4ConsoleEnabled;

                    MainWindow();

                    void Draw() override;
                    void Update() override;
                };
            }
        }
    }
}

#endif // !MAIN_WINDOW_H
