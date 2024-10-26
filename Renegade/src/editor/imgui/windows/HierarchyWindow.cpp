#ifdef __EDITOR__

#include "editor/imgui/windows/HierarchyWindow.h"

#include <imgui/imgui_internal.h>
#include <imgui/imgui_helpers.h>

#include "editor/imgui/ImGuiWindow.h"
#include "editor/imgui/ImGuiDefines.h"
#include "logger/Logger.h"
#include "utils/string_extensions.h"
#include "core/Engine.h"
#include "gameplay/systems/EntityDetailComponent.h"
#include "gameplay/systems/EntityDetailSystem.h"
#include "editor/explorer_resources/SceneExplorerResource.h"
#include "core/Header.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			HierarchyWindow::HierarchyWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_HIERARCHY) + " Hierarchy", "Hierarchy")
			{ }

            bool HierarchyWindow::Initialize()
            {
				core::ENGINE.GetECS().m_OnEntitiesUpdated += std::bind(&HierarchyWindow::UpdateEntities, this);
				core::ENGINE.GetECS().m_OnEntityComponentsUpdated += std::bind(&HierarchyWindow::UpdateEntityComponents, this);
				return BaseWindow::Initialize();
            }

			bool HierarchyWindow::Destroy()
			{
				core::ENGINE.GetECS().m_OnEntitiesUpdated -= std::bind(&HierarchyWindow::UpdateEntities, this);
				core::ENGINE.GetECS().m_OnEntityComponentsUpdated -= std::bind(&HierarchyWindow::UpdateEntityComponents, this);
				return BaseWindow::Destroy();
			}

			void HierarchyWindow::Render()
			{
				std::lock_guard<std::mutex> lock(core::m_EntityMutex);

				if (ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S) && core::ENGINE.GetEditor().GetCurrentScene() && core::ENGINE.GetEditor().GetCurrentScene()->IsDirty())
				{
					core::ENGINE.GetEditor().GetCurrentScene()->Save();
				}

				ImVec2 toolbarSize = ImVec2(ImGui::GetContentRegionAvail().x, m_Window.HeaderSize().y);
				ImGui::BeginToolbar(toolbarSize);

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

				if (ImGui::TransparentButton(IMGUI_FORMAT_ID(std::string(ICON_FA_CUBE), BUTTON_ID, "SPAWN_ENTITY_HIERARCHY").c_str(), ImVec2(toolbarSize.y, toolbarSize.y)))
				{
					core::ENGINE.GetECS().CreateEntity(core::ENGINE.GetECS().GetUniqueName("New GameObject"));
					m_NeedsRefresh = true;
					if (core::ENGINE.GetEditor().GetCurrentScene() && !core::ENGINE.GetECS().HasStarted())
					{
						core::ENGINE.GetEditor().SetDirty();
					}
				}
				ImGui::SameLine();

				bool wasDirty = !core::ENGINE.GetEditor().GetCurrentScene() || !core::ENGINE.GetEditor().GetCurrentScene()->IsDirty();
				if (wasDirty)
				{
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				}
				
				if (ImGui::TransparentButton(IMGUI_FORMAT_ID(std::string(ICON_FA_SAVE), BUTTON_ID, "SAVE_HIERARCHY").c_str(), ImVec2(toolbarSize.y, toolbarSize.y)))
				{
					core::ENGINE.GetEditor().GetCurrentScene()->Save();
				}

				if (wasDirty)
				{
					ImGui::PopItemFlag();
					ImGui::PopStyleVar();
				}

				ImGui::PopStyleVar();
				ImGui::PopStyleVar();

				ImGui::SameLine();
				float searchbarWidth = 200;
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (searchbarWidth + m_Window.GetWindowPadding().x));
				if (m_SearchBar.Render(IMGUI_FORMAT_ID("", INPUT_ID, "SEARCHBAR_HIERARCHY").c_str(), ImVec2(searchbarWidth, toolbarSize.y), m_Window.GetFramePadding().x / 2))
				{
					m_NeedsRefresh = true;
				}

				ImGui::EndToolbar(ImVec2(ImGui::GetStyle().ItemSpacing.x, 0));

				if (m_NeedsRefresh)
				{
					bool wasEntity = dynamic_cast<EntityUIView*>(core::ENGINE.GetEditor().GetSelectable());
					if (wasEntity)
					{
						core::ENGINE.GetEditor().SetSelectable(nullptr);
					}

					m_FilteredEntities.clear();

					m_NeedsRefresh = false;

					bool isEmptyString = m_SearchBar.GetString().empty();

					for (auto& entity : core::ENGINE.GetECS().GetEntities())
					{
						gameplay::EntityDetailComponent& detailComponent = core::ENGINE.GetECS().GetSystem<gameplay::EntityDetailSystem>().GetComponent(entity);

						if (isEmptyString || string_extensions::StringToLower(detailComponent.GetName()).find(m_SearchBar.GetString()) != std::string::npos)
						{
							m_FilteredEntities.emplace_back(EntityUIView(m_Window, entity));
						}
					}
					for (EntityUIView& entityView : m_FilteredEntities)
					{
						if (m_LastID == entityView.GetEntityID())
						{
							core::ENGINE.GetEditor().SetSelectable(&entityView);
						}
					}
				}

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(m_Window.GetFramePadding().x * 2, m_Window.GetFramePadding().y * 2));
				if (ImGui::BeginChild(
					IMGUI_FORMAT_ID("", CHILD_ID, "ENTITIES_HIERARCHY").c_str(),
					ImVec2(
						ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x,
						ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y
					),
					ImGuiChildFlags_Borders
				))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					for (EntityUIView& entity : m_FilteredEntities)
					{
						bool clicked = false;
						entity.Render(clicked, &entity == core::ENGINE.GetEditor().GetSelectable());
						if (clicked)
						{
							m_LastID = entity.GetEntityID();
							core::ENGINE.GetEditor().SetSelectable(&entity);
						}
					}
					ImGui::PopStyleVar();
				}
				ImGui::EndChild();
				ImGui::PopStyleVar();
			}

            void HierarchyWindow::UpdateEntities()
            {
				m_NeedsRefresh = true;
            }

            void HierarchyWindow::UpdateEntityComponents()
            {
				if (EntityUIView* derivedPtr = dynamic_cast<EntityUIView*>(core::ENGINE.GetEditor().GetSelectable()))
				{
					core::ENGINE.GetEditor().GetSelectable()->Deselect();
					core::ENGINE.GetEditor().GetSelectable()->Select();
				}
            }
		}
	}
}

#endif // __EDITOR__