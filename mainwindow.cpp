#include "mainwindow.h"
#include "globals.h"
#include "creditswindow.h"
#include "actorlist.h"
#include "renderer.h"
#include "objectcache.h"
#include "watermark.h"
#include "inventorymapper.h"

#include <imgui.h>

namespace polaris
{
	ActorList* pActorList = nullptr;
	CreditsWindow* pCreditsWindow = nullptr;
	ObjectCache* pObjectCache = nullptr;
	Watermark* pWatermark = nullptr;
	InventoryMapper* pInventoryMapper = nullptr;

	MainWindow::MainWindow()
	{
		Console::Log("Initializing UI");

		// Only enable these tools if debugging tools are enabled.
		if (ENABLE_DEBUGGING_TOOLS)
		{
			pActorList = new polaris::ActorList;

			// This is an highly experimental tool.
			if(ENABLE_EXPERIMENTAL_DEBUGGING_TOOLS)
				pObjectCache = new polaris::ObjectCache;
		}

		if (gpMainWindow)
		{
			Util::ThrowFatalError(L"MainWindow is already initialized!");
		}

		gpMainWindow = this;

		// Create window instances.
		pWatermark = new Watermark;
		pInventoryMapper = new InventoryMapper;
		pCreditsWindow = new CreditsWindow;
	}

	void MainWindow::Draw()
	{
		ImGui::BeginMainMenuBar();
		{
			if (ImGui::BeginMenu("Player"))
			{
				// Only give the user this option if experimental & debugging tools are enabled.
				if (ENABLE_DEBUGGING_TOOLS && ENABLE_EXPERIMENTAL_DEBUGGING_TOOLS)
					if (ImGui::MenuItem("Object Cache (Experimental)"))
						pObjectCache->m_bShowWindow = !pObjectCache->m_bShowWindow;
				
				if (ImGui::MenuItem("Inventory Mapper"))
					pInventoryMapper->m_bShowWindow = !pInventoryMapper->m_bShowWindow;

				if (!Globals::gpLocalPlayer->ViewportClient->ViewportConsole)
				{
					if (ImGui::MenuItem("Enable Console"))
						Util::EnableConsole(true);
				}
				else
				{
					if (ImGui::MenuItem("Disable Console"))
						Util::EnableConsole(false);
				}

				if (ImGui::MenuItem("Exit"))
					ExitProcess(EXIT_SUCCESS);

				ImGui::EndMenu();
			}

			if(ENABLE_DEBUGGING_TOOLS)
				if (ImGui::BeginMenu("World"))
				{
					if (ImGui::MenuItem("Actor Inspector"))
						pActorList->m_bShowWindow = !pActorList->m_bShowWindow;

					ImGui::EndMenu();
				}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Credits"))
					pCreditsWindow->m_bShowWindow = !pCreditsWindow->m_bShowWindow;

				if (ImGui::MenuItem("Irma Burger"))
					system("start https://www.youtube.com/watch?v=egYTYpleY0U");

				ImGui::EndMenu();
			}

			ImGui::End();
		}
	}
}
