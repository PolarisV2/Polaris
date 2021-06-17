#include "objectcache.h"

#include "globals.h"


namespace polaris
{
	ObjectCache::ObjectCache()
	{
		Console::Log("Initializing ObjectCache");
	}

	void ObjectCache::Draw()
	{
		auto pObjectCache = SDK::UObject::GetGlobalObjects();

		ImGui::SetNextWindowSize(ImVec2(820, 440), ImGuiCond_Appearing);
		ImGui::Begin("Object Cache", &m_bShowWindow, ImGuiWindowFlags_Modal);
		{
			// Actor Hierarchy
			static int selected = 0;
			{
				ImGui::BeginChild("Hierarchy", ImVec2(300, 0), true);
				{
					for (int i = 0; i < pObjectCache.Num(); i++)
					{
						// Absolutely make sure the actor exists, Unreal Engine is very prone to crashing
						if (pObjectCache.GetByIndex(i))
						{
							// Create a button for the actor, and set the selected actor once clicked
							if (ImGui::Selectable(pObjectCache.GetByIndex(i)->GetName().c_str(), selected == i))
								selected = i;
						}
					}
					ImGui::EndChild();
				}
			}

			auto selectedActor = pObjectCache.GetByIndex(selected);

			// Inspector
			// Only draw the inspector screen if the selected actor exists
			if (selectedActor)
			{
				ImGui::SameLine();

				ImGui::BeginGroup();
				ImGui::BeginChild("Inspector", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
				ImGui::Text(selectedActor->GetName().c_str());
				ImGui::Separator();
				ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None);
				{
					if (ImGui::BeginTabItem("Properties"))
					{
						ImGui::TextWrapped("Address: %s", selectedActor->GetFullName().c_str());
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
				ImGui::EndChild();
				ImGui::EndGroup();
			}
		}
		ImGui::End();
	}
}