#include "actorlist.h"

#include "globals.h"

namespace polaris
{
	bool highFpsMode = false;

	// Even though this is empty, it's still required.
	ActorList::ActorList()
	{
	}

	void ActorList::Draw()
	{
		ImGui::SetNextWindowSize(ImVec2(820, 440), ImGuiCond_Appearing);
		ImGui::Begin("Actors", &m_bShowWindow, ImGuiWindowFlags_Modal);
		{
			ImGui::Checkbox("FPS Saver mode", &highFpsMode);

			// High Performance Mode defaults
			std::string defaultLevelName = (*polaris::Globals::gpWorld)->Levels[0]->GetFullName();
			static std::string selectedLevelName = defaultLevelName.substr(0, defaultLevelName.find(".")).substr(5, defaultLevelName.length() - 1);

			// Actor Hierarchy
			static int selectedLevel = 0;
			static int selectedActor = 0;
			{
				ImGui::BeginChild("Hierarchy", ImVec2(300, 0), true);
				{
					if (highFpsMode == true)
					{
						if (ImGui::BeginCombo("Map", selectedLevelName.c_str()))
						{
							for (int i = 0; i < (*polaris::Globals::gpWorld)->Levels.Num(); i++)
							{
								SDK::ULevel* level = (*polaris::Globals::gpWorld)->Levels[i];

								// GetName only returns "PersistentLevel" for some reason, so we do this hacky garbage :D
								std::string name = level->GetFullName();
								std::string formattedName = name.substr(0, name.find(".")).substr(5, name.length() - 1);

								if(ImGui::Selectable(formattedName.c_str(), selectedLevel == i))
								{
									selectedLevelName = formattedName;
									selectedLevel = i;
								}
							}

							ImGui::EndCombo();
						}

						ImGui::Separator();

						SDK::ULevel* level = (*polaris::Globals::gpWorld)->Levels[selectedLevel];
						for (int i = 0; i < level->Actors.Num(); i++)
						{
							// Absolutely make sure the actor exists, Unreal Engine is very prone to crashing
							if (level->Actors.IsValidIndex(i) && level->Actors[i])
							{
								SDK::AActor* actor = level->Actors[i];

								// Create a button for the actor, and set the selected actor once clicked
								if (ImGui::Selectable(actor->GetName().c_str(), selectedActor == i))
								{
									selectedActor = i;
								}
							}
						}
					}
					else
					{
						for (int i = 0; i < (*polaris::Globals::gpWorld)->Levels.Num(); i++)
						{
							if ((*polaris::Globals::gpWorld)->Levels.IsValidIndex(i) && (*polaris::Globals::gpWorld)->Levels[i])
							{
								SDK::ULevel* level = (*polaris::Globals::gpWorld)->Levels[i];

								// GetName only returns "PersistentLevel" for some reason, so we do this hacky garbage :D
								std::string name = level->GetFullName();
								std::string formattedName = name.substr(0, name.find(".")).substr(5, name.length() - 1);
								if (ImGui::TreeNode(formattedName.c_str()))
								{
									for (int j = 0; j < level->Actors.Num(); j++)
									{
										// Absolutely make sure the actor exists, Unreal Engine is very prone to crashing
										if (level->Actors.IsValidIndex(j) && level->Actors[j])
										{
											SDK::AActor* actor = level->Actors[j];

											// Create a button for the actor, and set the selected actor once clicked
											if (ImGui::Selectable(actor->GetName().c_str(), selectedActor == j && selectedLevel == i))
											{
												selectedLevel = i;
												selectedActor = j;
											}
										}
									}

									ImGui::TreePop();
								}
							}
						}
					}
					ImGui::EndChild();
				}
			}

			auto level = (*polaris::Globals::gpWorld)->Levels[selectedLevel];
			auto actor = level->Actors[selectedActor];

			// Inspector
			// Only draw the inspector screen if the selected actor exists
			if (actor)
			{
				ImGui::SameLine();

				ImGui::BeginGroup();
				ImGui::BeginChild("Inspector", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
				ImGui::Text(actor->GetName().c_str());
				ImGui::Separator();
				ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None);
				{
					if (ImGui::BeginTabItem("Properties"))
					{
						ImGui::TextWrapped("ID: %s", actor->GetFullName().c_str());

						// Position and Rotation field
						ImGui::TextWrapped("Position: (%d, %d, %d)", actor->K2_GetActorLocation().X, actor->K2_GetActorLocation().Y, actor->K2_GetActorLocation().Z);
						ImGui::TextWrapped("Rotation: (%d, %d, %d)", actor->K2_GetActorRotation().Pitch, actor->K2_GetActorRotation().Roll, actor->K2_GetActorRotation().Yaw);

						// Owner field
						auto ownerLabel = actor->Owner != nullptr ? actor->Owner->GetName().c_str() : "None";
						ImGui::TextWrapped("Owner: %s", ownerLabel);
						
						// Authority field
						auto authorityLabel = actor->HasAuthority() ? "Yes" : "No";
						ImGui::TextWrapped("Has Authority: %s", authorityLabel);

						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
				ImGui::EndChild();

				if (ImGui::Button("Destroy")) 
					actor->K2_DestroyActor();

				ImGui::SameLine();

				if (ImGui::Button("Toggle Authority"))
					actor->Role = actor->HasAuthority() ? SDK::ENetRole::ROLE_None : SDK::ENetRole::ROLE_Authority;

				ImGui::EndGroup();
			}
		}
		ImGui::End();
	}
}