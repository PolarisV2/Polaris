#include "creditswindow.h"

#include <imgui.h>

namespace polaris
{
	CreditsWindow::CreditsWindow()
	{
		Console::Log("Initializing CreditsWindow");
	}

	void CreditsWindow::Draw()
	{
		ImGui::Begin("Credits", &m_bShowWindow, ImGuiWindowFlags_Modal);
		{
			for (const char* credit : credits)
			{
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize(credit).x / 2);
				ImGui::Text(credit);
			}

			ImGui::End();
		}
	}
}
