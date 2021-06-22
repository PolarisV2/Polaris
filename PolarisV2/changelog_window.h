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

#ifndef CHANGELOG_WINDOW_H
#define CHANGELOG_WINDOW_H

#include "ui_window.h"

#include <vector>
#include <string>

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
            {
                class ChangelogEntry
                {
                public:
                    const char* m_ccHeader;
                    const char* m_ccBody;

                    ChangelogEntry(const char* header, const char* body)
                    {
                        m_ccHeader = header;
                        m_ccBody = body;
                    }
                };

                class ChangelogField
                {
                public:
                    std::string m_sHeader;
                    std::vector<ChangelogEntry*> m_vEntries;

                    ChangelogField(std::string header, ChangelogEntry* entries[])
                    {
                        m_sHeader = header;

                        for (int i = 0; i < (sizeof(entries) / sizeof(*entries) + 1); i++)
                            m_vEntries.push_back(entries[i]);
                    }
                };

                class ChangelogWindow : public UIWindow
                {
                private:
                    bool m_bShouldUnlockFortUIInput = false;
                public:
                    const char* FIELD_HEADER_FORMAT = "-  %s";
                    const char* FIELD_BODY_FORMAT = "     %s";
                    const char* BUILD_DATE = "November 1st, 2020";

                    ChangelogField* m_aFields[3] = {
                        new ChangelogField("New Features", new ChangelogEntry* [2] {
                            new ChangelogEntry("Building", "It's finally here, build to your heart's content. Make something unbelievable!"),
                            new ChangelogEntry("Inventory", "The Inventory Mapper has been replaced, simply open your Inventory (normally the \"I\" key) and drag your favorite weapon onto your quickbar.")
                        }),
                        new ChangelogField("Fixes and Updates", new ChangelogEntry* [2] {
                            new ChangelogEntry("Codebase", "The entire Polaris codebase has been rewritten (codenamed PolarisV2), and provides more stability and ease-of-use for developers."),
                            new ChangelogEntry("Jumping", "You will no longer constantly jump while holding space.")
                        }),
                        new ChangelogField("Notes", new ChangelogEntry* [2] {
                            new ChangelogEntry("Discord", "Join and Talk to us over at discord.gg/polaris!"),
                            new ChangelogEntry("Issues", "There *might* be some issues, while we iron out the new codebase.")
                        })
                    };

                    ChangelogWindow();
                    
                    void Draw() override;
                    void Update() override;
                };
            }
        }
    }
}

#endif // !CHANGELOG_WINDOW_H
