#include "editor/imgui/ExplorerResourceUIView.h"

#include <imgui/imgui_helpers.h>

#include "editor/imgui/ImGuiDefines.h"
#include "utils/string_extensions.h"
#include "assets/AssetType.h"
#include "editor/ExplorerResource.h"
#include "core/Engine.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			std::vector<std::string> RESOURCE_ICONS =
			{
				ICON_FA_ASSET_CFG,
				ICON_FA_ASSET_SCENE,
				ICON_FA_ASSET_MAT,
				ICON_FA_ASSET_TEX,
				ICON_FA_ASSET_SPR,
				ICON_FA_ASSET_FONT,
				ICON_FA_ASSET_SND,
				ICON_FA_ASSET_SONG,
				ICON_FA_ASSET_VO,
				ICON_FA_ASSET_ANIM,
				ICON_FA_ASSET_LOC,
				ICON_FA_ASSET_MOD,
			};

			void CreateExplorerResourceUIViewChild(ExplorerResourceUIView& a_ResourceView, ExplorerResource* a_Resource, ImGuiWindow& a_Window)
			{
				for (ExplorerResource* resource : a_Resource->m_Resources)
				{
					ExplorerResourceUIView resourceToAdd = ExplorerResourceUIView(a_Window);
					resourceToAdd.SetResource(resource);
					resourceToAdd.m_Parent = &a_ResourceView;
					CreateExplorerResourceUIViewChild(resourceToAdd, resource, a_Window);
					a_ResourceView.m_Resources.push_back(resourceToAdd);
				}
			}

			ExplorerResourceUIView::ExplorerResourceUIView(ImGuiWindow& a_Window) : EditorSelectable(a_Window)
			{
			}

			ExplorerResourceUIView::ExplorerResourceUIView() : EditorSelectable(core::ENGINE.GetEditor().GetImGuiWindow())
			{ }

			void ExplorerResourceUIView::SetResource(ExplorerResource* a_Resource)
			{
				if (!a_Resource)
				{
					return;
				}

				m_Resource = a_Resource;

				switch (m_Resource->GetAssetType())
				{
					case assets::AssetType::Sprite:
					case assets::AssetType::Texture:
					case assets::AssetType::Font:
					{
						m_ImageResource = dynamic_cast<ImageExplorerResource*>(a_Resource);
						m_AssetTypeDropdown = StringDropdown<assets::AssetType>(
							m_Resource->GetAssetType(),
							{
								assets::AssetType::Texture,
								assets::AssetType::Sprite,
								assets::AssetType::Font,
							},
							{
								assets::AssetTypeToString(assets::AssetType::Texture),
								assets::AssetTypeToString(assets::AssetType::Sprite),
								assets::AssetTypeToString(assets::AssetType::Font),
							}
						);
						break;
					}
					case assets::AssetType::Sound:
					case assets::AssetType::Song:
					case assets::AssetType::VO:
					{
						//m_SoundResource = dynamic_cast<ImageExplorerResource*>(a_Resource);
						m_AssetTypeDropdown = StringDropdown<assets::AssetType>(
							m_Resource->GetAssetType(),
							{
								assets::AssetType::Sound,
								assets::AssetType::Song,
								assets::AssetType::VO,
							},
							{
								assets::AssetTypeToString(assets::AssetType::Sound),
								assets::AssetTypeToString(assets::AssetType::Song),
								assets::AssetTypeToString(assets::AssetType::VO),
							}
						);
						break;
					}
					default:
					{
						break;
					}
				}

				m_Name = string_extensions::GetFileWithoutExtension(string_extensions::GetFileName(m_Resource->GetPath()));
				m_Icon = m_Resource->GetResourceType() == ExplorerResourceType::Folder ? ICON_FA_FOLDER : RESOURCE_ICONS[(int)m_Resource->GetAssetType()];

				GetChildren();
			}

			void ExplorerResourceUIView::ClearChildren()
			{
				m_Resources.clear();
			}

			void ExplorerResourceUIView::GetChildren()
			{
				if (!m_Resource)
				{
					return;
				}

				ClearChildren();
				CreateExplorerResourceUIViewChild(*this, m_Resource, m_Window);
			}

			void ExplorerResourceUIView::RenderIcon()
			{
				if (!m_Resource)
				{
					return;
				}

				switch (m_Resource->GetAssetType())
				{
					case assets::AssetType::Sprite:
					case assets::AssetType::Texture:
					case assets::AssetType::Font:
					{
						if (!m_ImageResource)
						{
							return;
						}

						if (!m_ImageResource->m_DescHandle->Invalid())
						{
							const float width_new = 15;
							const float height_new = (m_ImageResource->m_DescHandle->Height * (1.0f / m_ImageResource->m_DescHandle->Width * width_new));
							ImGui::Image((void*)m_ImageResource->m_DescHandle->GpuHandle.ptr, ImVec2(width_new, height_new));
						}
						break;
					}
					default:
					{
						ImGui::Text(m_Resource->GetResourceType() == ExplorerResourceType::Folder ? ICON_FA_FOLDER : m_Icon.c_str());
						break;
					}
				}
			}

			void ExplorerResourceUIView::Render(bool& clicked, bool& right_clicked, bool& double_clicked, bool selected)
			{
				if (!m_Resource)
				{
					return;
				}

				ImVec2 pos = ImGui::GetCursorScreenPos();

				clicked = ImGui::Selectable(IMGUI_FORMAT_ID("", SELECTABLE_ID, "RESOURCE_" + string_extensions::StringToUpper(m_Name)).c_str(), &selected);
				right_clicked = ImGui::IsItemHovered() && ImGui::IsItemClicked(1);
				double_clicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);

				ImGui::SetCursorScreenPos(pos);
				ImGui::SetCursorScreenPos(ImVec2(pos.x + 10, pos.y));
				RenderIcon();
				ImGui::SetCursorScreenPos(ImVec2(pos.x + 35, pos.y));
				ImGui::Text(m_Name.c_str());

				ImGui::SetCursorScreenPos(ImVec2(pos.x + 300, pos.y));
				ImVec4 textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
				textColor.w = 0.5f;
				ImGui::TextColored(textColor, 
					m_Resource->GetResourceType() == ExplorerResourceType::Folder ? "" : assets::AssetTypeToString(m_Resource->GetAssetType()).c_str());
			}

			bool ExplorerResourceUIView::HasFolders() const
			{
				if (!m_Resource)
				{
					return false;
				}

				for (auto& resource : m_Resources)
				{
					if (resource.m_Resource->GetResourceType() == ExplorerResourceType::Folder)
					{
						return true;
					}
				}
				return false;
			}

			void ExplorerResourceUIView::RenderSelectable()
			{
				ImVec2 toolbarSize = ImVec2(ImGui::GetContentRegionAvail().x, core::ENGINE.GetEditor().GetImGuiWindow().HeaderSize().y * 2);
				ImGui::BeginToolbar(toolbarSize);

				ImVec2 padding = core::ENGINE.GetEditor().GetImGuiWindow().GetWindowPadding();
				ImGui::EndToolbar(padding);
				
				float fontSize = core::ENGINE.GetEditor().GetImGuiWindow().FontSize();

				float y = ImGui::GetCursorPosY();
				float x = ImGui::GetCursorPosX() + fontSize;
				ImGui::SetCursorPos(ImVec2(x, y + fontSize));
				ImGui::PushFont(core::ENGINE.GetEditor().GetImGuiWindow().CapitalIcon());
				ImGui::Text(m_Icon.c_str());
				ImGui::PopFont();

				ImGui::SetCursorPosY(y + (fontSize / 2));
				ImGui::SetCursorPosX(x + (fontSize * 3));

				m_NameInput.SetString(m_Name);
				if (m_NameInput.Render(imgui::IMGUI_FORMAT_ID("", INPUT_ID, "NAME_INSPECTOR").c_str(), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					if (m_Resource->Rename(m_NameInput.GetString()))
					{
						SetResource(m_Resource);
					}
				}

				if (m_Resource->GetResourceType() == ExplorerResourceType::Folder)
				{
					return;
				}

				ImGui::SetCursorPosY(y + toolbarSize.y);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().ItemSpacing.x);

				ImVec2 framePadding = core::ENGINE.GetEditor().GetImGuiWindow().GetFramePadding();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(framePadding.x * 2, framePadding.y * 2));
				if (ImGui::BeginChild(
					imgui::IMGUI_FORMAT_ID("", CHILD_ID, "EXPLORER_RESOURCE_INSPECTOR").c_str(),
					ImVec2(
						ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x,
						ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y
					),
					ImGuiChildFlags_Borders
				))
				{
					ImGui::DisplayHeader(core::ENGINE.GetEditor().GetImGuiWindow().Bold(), "Type");
					ImGui::SameLine();

					switch (m_Resource->GetAssetType())
					{
						case assets::AssetType::Sprite:
						case assets::AssetType::Texture:
						case assets::AssetType::Font:
						case assets::AssetType::Sound:
						case assets::AssetType::Song:
						case assets::AssetType::VO:
						{
							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
							if (m_AssetTypeDropdown.Render(imgui::IMGUI_FORMAT_ID("", COMBO_ID, "ASSETTYPE_INSPECTOR").c_str()))
							{
								m_Resource->SetAssetType(m_AssetTypeDropdown.GetValue());
								m_Resource->SaveMetadata();
							}
							ImGui::PopStyleVar();
							break;
						}
						default:
						{
							ImGui::Text(assets::AssetTypeToString(m_Resource->GetAssetType()).c_str());
							break;
						}
					}


					switch (m_Resource->GetAssetType())
					{
						case assets::AssetType::Sprite:
						case assets::AssetType::Texture:
						case assets::AssetType::Font:
						{
							if (!m_ImageResource->m_DescHandle->Invalid())
							{
								const float width_new = ImGui::GetContentRegionAvail().x;
								const float height_new = (m_ImageResource->m_DescHandle->Height * (1.0f / m_ImageResource->m_DescHandle->Width * width_new));
								ImGui::Image((void*)m_ImageResource->m_DescHandle->GpuHandle.ptr, ImVec2(width_new, height_new));
							}
							break;
						}
						default:
						{
							break;
						}
					}
				}
				ImGui::PopStyleVar();
				ImGui::EndChild();
			}
		}
	}
}