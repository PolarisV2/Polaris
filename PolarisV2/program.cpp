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

#include "program.h"
#include "sdk_utils.h"
#include "base_plate.h"
#include "error_utils.h"
#include "ui_renderer.h"

#include <windows.h>
#include <cstdio>
#include <MinHook.h>
#pragma comment(lib, "libMinHook.x64.lib")

polaris::Program* gpProgram;

namespace polaris
{
	Program::Program()
	{
		if (gpProgram != nullptr || MH_Initialize() != MH_OK)
			utilities::ErrorUtils::ThrowException(L"Initializing failed. Please restart Fortnite and try again!");
		gpProgram = this;

		utilities::SDKUtils::InitConsole();
		std::cout << "Welcome to Polaris!" << std::endl;

		m_pMainTable = new tables::PehTable;
		m_pMainTable->PushPlate(new tables::plates::BasePlate);
	}

	Program::~Program()
	{
		std::cout << "Disposing!" << std::endl;
		
		m_pMainTable->Pop();
	}
}
