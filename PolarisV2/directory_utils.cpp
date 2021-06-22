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

#include "directory_utils.h"
#include "error_utils.h"
#include "sdk_utils.h"

#include <windows.h>

namespace polaris
{
    namespace utilities
    {
        std::string DirectoryUtils::GetConcatPath(const std::string& sFirst, const std::string& sSecond)
        {
            std::string sTemp = sFirst;

            if (sFirst[sFirst.length()] != '\\')
            {
                sTemp += '\\';

                return(sTemp + sSecond);
            }
            else
                return(sFirst + sSecond);
        }
        std::string DirectoryUtils::GetFontsDir()
        {
            char winDir[MAX_PATH];
            if (!GetWindowsDirectoryA(winDir, sizeof(winDir)))
                ErrorUtils::ThrowException(L"An unknown platform exception occured.");

            return GetConcatPath(winDir, "Fonts");
        }
    }
}