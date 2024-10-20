#ifdef __EDITOR__

#include "editor/imgui/windows/ExplorerWindow.h"

#include "editor/imgui/ImGuiDefines.h"
#include "utils/string_extensions.h"
#include "file/FileLoader.h"
#include "editor/imgui/ImGuiWindow.h"
#include "imgui/imgui_helpers.h"
#include "editor/Editor.h"
#include "core/Engine.h"
#include "editor/ExplorerResource.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			ExplorerWindow::ExplorerWindow(ImGuiWindow& a_Window) : BaseWindow(a_Window, ImGuiWindowFlags_NoCollapse, std::string(ICON_FA_EXPLORER) + " Explorer", "Explorer"), 
				m_AssetRoot(ExplorerResourceUIView(m_Window, core::ENGINE.GetEditor().GetAssetDatabase().GetRoot()))
			{ }

			bool ExplorerWindow::Initialize()
			{
				OnScanCompleted();

				core::ENGINE.GetEditor().GetAssetDatabase().m_OnBeforeScan += std::bind(&ExplorerWindow::OnBeforeScan, this);
				core::ENGINE.GetEditor().GetAssetDatabase().m_OnScanCompleted += std::bind(&ExplorerWindow::OnScanCompleted, this);

				return true;
			}

			void ExplorerWindow::SetExplorerRoot(ExplorerResourceUIView* a_Resource)
			{
				if (!a_Resource)
				{
					return;
				}

				m_FolderRoot = a_Resource;
				m_FolderRoot->GetChildren();
			}

			void ExplorerWindow::RenderFolder(ExplorerResourceUIView& a_Resource)
			{
				if (a_Resource.GetName().empty() ||
					a_Resource.GetResource().GetPath().empty())
				{
					return;
				}

				if (a_Resource.GetResource().GetResourceType() != ExplorerResourceType::Folder)
				{
					return;
				}

				std::string name = a_Resource.GetName();
				std::string icon = ICON_FA_FOLDER_OPEN;
				if (a_Resource.GetResource().GetResourceType() == ExplorerResourceType::Folder && a_Resource.IsFoldedOut() && a_Resource.HasFolders())
				{
					icon = ICON_FA_FOLDER_OPEN;
				}
				else if (a_Resource.GetResource().GetResourceType() == ExplorerResourceType::Folder)
				{
					icon = ICON_FA_FOLDER;
				}
				std::string id = IMGUI_FORMAT_ID("", TREE_NODE_ID, "TREE_NODE_" + string_extensions::StringToUpper(a_Resource.GetResource().GetPath()) + "_EXPLORER");

				bool clicked, right_clicked;

				int flags = ImGuiTreeNodeFlags_OpenOnArrow;
				if (!a_Resource.HasFolders())
				{
					flags = ImGuiTreeNodeFlags_Leaf;
				}
				const bool fold = ImGui::EngineTreeNodeExS(id.c_str(), icon.c_str(), name.c_str(), clicked, right_clicked, &a_Resource == m_FolderRoot, ImVec2(ImGui::GetContentRegionAvail().x, m_Window.FontSize()), flags);

				if (clicked)
				{
					m_NewFolderRoot = &a_Resource;
				}

				if (a_Resource.IsFoldedOut() != fold)
				{
					a_Resource.SetFoldedOut(fold);
					if (a_Resource.IsFoldedOut())
					{
						a_Resource.GetChildren();
					}
				}

				if (a_Resource.IsFoldedOut())
				{
					for (auto& resource : a_Resource.m_Resources)
					{
						RenderFolder(resource);
					}
				}

				if (fold)
				{
					ImGui::TreePop();
				}
			}

			void ExplorerWindow::Render()
			{
				// If the asset database is not ready, do not continue.
				if (!core::ENGINE.GetEditor().GetAssetDatabase().Ready())
				{
					return;
				}

				// This needs to be done at the start of the frame to avoid errors.
				// We set the root directory of the second window that shows the assets.
				if (m_NewFolderRoot)
				{
					if (m_NewFolderRoot != m_FolderRoot)
					{
						if (ExplorerResourceUIView* derivedPtr = dynamic_cast<ExplorerResourceUIView*>(core::ENGINE.GetEditor().GetSelectable()))
						{
							core::ENGINE.GetEditor().SetSelectable(nullptr);
						}

						SetExplorerRoot(m_NewFolderRoot);
						m_NeedsRefresh = true;
						m_NewFolderRoot = nullptr;
					}
				}

				// This needs to be done at the start of the frame to avoid errors.
				// We refresh the assets that show up based on the searchbar and the root directory.
				if (m_NeedsRefresh)
				{
					m_FilteredResources.clear();

					m_NeedsRefresh = false;

					bool isEmptyString = m_SearchBar.GetString().empty();

					for (auto& resource : m_FolderRoot->m_Resources)
					{
						if (isEmptyString || string_extensions::StringToLower(resource.GetName()).find(m_SearchBar.GetString()) != std::string::npos)
						{
							m_FilteredResources.push_back(&resource);
						}
					}
				}
				if (!m_FolderRoot)
				{
					return;
				}

				ImVec2 toolbarSize = ImVec2(ImGui::GetContentRegionAvail().x, m_Window.HeaderSize().y);
				ImGui::BeginToolbar(toolbarSize);

				if (ImGui::TransparentButton(
					IMGUI_FORMAT_ID(std::string(ICON_FA_REFRESH), BUTTON_ID, "RESCAN_EXPLORER").c_str(), ImVec2(0, toolbarSize.y)))
				{
					core::ENGINE.GetEditor().GetAssetDatabase().Rescan();
				}

				ImGui::SameLine();

				float searchbarWidth = 200;
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (searchbarWidth + m_Window.GetWindowPadding().x));
				if (m_SearchBar.Render(IMGUI_FORMAT_ID("", INPUT_ID, "SEARCHBAR_EXPLORER").c_str(), ImVec2(searchbarWidth, toolbarSize.y), m_Window.GetFramePadding().x / 2))
				{
					m_NeedsRefresh = true;
				}

				ImGui::EndToolbar(m_Window.GetWindowPadding());

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(m_Window.GetFramePadding().x, m_Window.GetFramePadding().y));
				if (ImGui::BeginChild(
					IMGUI_FORMAT_ID("", CHILD_ID, "DIRECTORIES_EXPLORER").c_str(),
					ImVec2(
						(ImGui::GetContentRegionAvail().x - m_Window.GetWindowPadding().x) * 0.15f,
						ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y
					),
					ImGuiChildFlags_Borders
				))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					RenderFolder(m_AssetRoot);
					ImGui::PopStyleVar();
				}
				ImGui::EndChild();

				ImGui::PopStyleVar();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(m_Window.GetFramePadding().x * 2, m_Window.GetFramePadding().y * 2));
				ImGui::SameLine();
				if (ImGui::BeginChild(
					IMGUI_FORMAT_ID("", CHILD_ID, "FILES_EXPLORER").c_str(),
					ImVec2(
						ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x,
						ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y
					),
					ImGuiChildFlags_Borders
				))
				{
					if (m_FolderRoot)
					{
						if (ImGui::BeginChild(
							IMGUI_FORMAT_ID("", CHILD_ID, "FILES_INNER_EXPLORER").c_str(),
							ImVec2(
								0,
								0
							)
						))
						{
							if (m_FolderRoot->m_Parent)
							{
								bool clicked, right_clicked, double_clicked;
								ImGui::EngineResourceNode(
									IMGUI_FORMAT_ID("", SELECTABLE_ID, "PREVIOUS_FOLDER_EXPLORER").c_str(),
									ICON_FA_FOLDER,
									"../",
									"",
									clicked,
									right_clicked,
									double_clicked,
									false
								);

								if (double_clicked)
								{
									m_NewFolderRoot = m_FolderRoot->m_Parent;
								}
							}

							for (ExplorerResourceUIView* item : m_FilteredResources)
							{
								if (!item)
								{
									continue;
								}

								bool clicked, right_clicked, double_clicked;
								item->Render(clicked, right_clicked, double_clicked, item == core::ENGINE.GetEditor().GetSelectable());

								if (clicked)
								{
									core::ENGINE.GetEditor().SetSelectable(item);
								}
								if (double_clicked && item->GetResource().GetResourceType() == ExplorerResourceType::Folder)
								{
									m_NewFolderRoot = item;
								}
							}
						}
						ImGui::EndChild();
					}
				}
				ImGui::EndChild();
				ImGui::PopStyleVar();
			}

			void ExplorerWindow::OnScanCompleted()
			{
				m_AssetRoot = ExplorerResourceUIView(m_Window, core::ENGINE.GetEditor().GetAssetDatabase().GetRoot());
				SetExplorerRoot(&m_AssetRoot);
				m_NeedsRefresh = true;
			}

			void ExplorerWindow::OnBeforeScan()
			{
				m_FilteredResources.clear();
				if (ExplorerResourceUIView* derivedPtr = dynamic_cast<ExplorerResourceUIView*>(core::ENGINE.GetEditor().GetSelectable()))
				{
					core::ENGINE.GetEditor().SetSelectable(nullptr);
				}
			}
		}
	}
}

#endif // __EDITOR__