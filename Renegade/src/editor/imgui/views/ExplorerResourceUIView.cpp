#ifdef __EDITOR__

#include "editor/imgui/views/ExplorerResourceUIView.h"

#include <imgui/imgui_helpers.h>

#include "editor/imgui/ImGuiDefines.h"
#include "utils/string_extensions.h"
#include "assets/AssetType.h"
#include "editor/ExplorerResource.h"
#include "editor/explorer_resources/AudioExplorerResource.h"
#include "editor/explorer_resources/SceneExplorerResource.h"
#include "core/Engine.h"
#include "file/FileLoader.h"

#include "editor/imgui/views/ExplorerResourceViews/DefaultExplorerResourceUIView.h"
#include "editor/imgui/views/ExplorerResourceViews/TextureExplorerResourceUIView.h"
#include "editor/imgui/views/ExplorerResourceViews/AudioExplorerResourceUIView.h"
#include "editor/imgui/views/ExplorerResourceViews/SceneExplorerResourceUIView.h"

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

			void CreateExplorerResourceUIViewChild(ExplorerResourceUIView* a_ResourceView, ExplorerResource* a_Resource, ImGuiWindow& a_Window)
			{
				a_ResourceView->ClearChildren();
				for (ExplorerResource* resource : a_Resource->m_Resources)
				{
					ExplorerResourceUIView* resourceToAdd = ExplorerResourceUIView::CreateViewFromExplorerResource(resource, a_Window);
					resourceToAdd->m_Parent = a_ResourceView;
					CreateExplorerResourceUIViewChild(resourceToAdd, resource, a_Window);
					a_ResourceView->m_Resources.push_back(resourceToAdd);
				}
			}

			ExplorerResourceUIView::~ExplorerResourceUIView()
			{
				ClearChildren();
			}

			ExplorerResourceUIView* ExplorerResourceUIView::CreateViewFromExplorerResource(ExplorerResource* a_Resource, ImGuiWindow& a_Window)
			{
				ExplorerResourceUIView* explorerView = nullptr;
				switch (a_Resource->GetAssetType())
				{
					case assets::AssetType::Texture:
					{
						explorerView = new TextureExplorerResourceUIView(a_Window, dynamic_cast<TextureExplorerResource*>(a_Resource));
						break;
					}
					case assets::AssetType::Sprite:
					{
						explorerView = new SpriteExplorerResourceUIView(a_Window, dynamic_cast<SpriteExplorerResource*>(a_Resource));
						break;
					}
					case assets::AssetType::Font:
					{
						explorerView = new FontExplorerResourceUIView(a_Window, dynamic_cast<FontExplorerResource*>(a_Resource));
						break;
					}
					case assets::AssetType::Sound:
					{
						explorerView = new SoundExplorerResourceUIView(a_Window, dynamic_cast<SoundExplorerResource*>(a_Resource));
						break;
					}
					case assets::AssetType::Song:
					{
						explorerView = new SongExplorerResourceUIView(a_Window, dynamic_cast<SongExplorerResource*>(a_Resource));
						break;
					}
					case assets::AssetType::VO:
					{
						explorerView = new VOExplorerResourceUIView(a_Window, dynamic_cast<VOExplorerResource*>(a_Resource));
						break;
					}
					case assets::AssetType::Scene:
					{
						explorerView = new SceneExplorerResourceUIView(a_Window, dynamic_cast<SceneExplorerResource*>(a_Resource));
						break;
					}
					default:
					{
						explorerView = new DefaultExplorerResourceUIView(a_Window, a_Resource);
						break;
					}
				}
				explorerView->SetData(a_Resource);

				return explorerView;
			}

			void ExplorerResourceUIView::ClearChildren()
			{
				for (ExplorerResourceUIView* resource : m_Resources)
				{
					delete resource;
				}
				m_Resources.clear();
			}

			ExplorerResourceUIView::ExplorerResourceUIView(ImGuiWindow& a_Window) : EditorSelectable(a_Window)
			{ }

			const std::string& ExplorerResourceUIView::GetName() const
			{
				return m_Name;
			}

			const std::string& ExplorerResourceUIView::GetIcon() const
			{
				return m_Icon;
			}

			bool ExplorerResourceUIView::IsFoldedOut() const
			{
				return m_FoldedOut;
			}

			void ExplorerResourceUIView::SetFoldedOut(bool a_FoldedOut)
			{
				m_FoldedOut = a_FoldedOut;
			}

			void ExplorerResourceUIView::SetData(ExplorerResource* a_Resource)
			{
				m_Name = string_extensions::GetFileWithoutExtension(string_extensions::GetFileName(a_Resource->GetPath()));
				m_Icon = a_Resource->GetResourceType() == ExplorerResourceType::Folder ? ICON_FA_FOLDER : RESOURCE_ICONS[(int)a_Resource->GetAssetType()];
				m_StrResourceType = a_Resource->GetResourceType() == ExplorerResourceType::Folder ? "" : assets::AssetTypeToString(a_Resource->GetAssetType());
				m_ResourceType = a_Resource->GetResourceType();

				CreateExplorerResourceUIViewChild(this, a_Resource, m_Window);
			}

			void ExplorerResourceUIView::RenderIcon()
			{
				ImGui::Text(m_Icon.c_str());
			}

			void ExplorerResourceUIView::Render(bool& clicked, bool& right_clicked, bool& double_clicked, bool selected)
			{
				ImVec2 pos = ImGui::GetCursorScreenPos();

				clicked = ImGui::Selectable(IMGUI_FORMAT_ID("", SELECTABLE_ID, "RESOURCE_" + string_extensions::StringToUpper(m_Name)).c_str(), &selected);
				right_clicked = ImGui::IsItemHovered() && ImGui::IsItemClicked(1);
				double_clicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);

				if (double_clicked)
				{
					DoubleClicked();
				}

				ImGui::SetCursorScreenPos(pos);
				ImGui::SetCursorScreenPos(ImVec2(pos.x + 10, pos.y));
				RenderIcon();
				ImGui::SetCursorScreenPos(ImVec2(pos.x + 35, pos.y));
				ImGui::Text(m_Name.c_str());

				ImGui::SetCursorScreenPos(ImVec2(pos.x + 300, pos.y));
				ImVec4 textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
				textColor.w = 0.5f;
				ImGui::TextColored(textColor, m_StrResourceType.c_str());
			}

			void ExplorerResourceUIView::RenderBaseSelectable(ExplorerResource* a_Resource)
			{
				ImVec2 toolbarSize = ImVec2(ImGui::GetContentRegionAvail().x, core::ENGINE.GetEditor().GetImGuiWindow().HeaderSize().y * 2.5f);
				ImGui::BeginToolbar(toolbarSize);

				ImVec2 padding = core::ENGINE.GetEditor().GetImGuiWindow().GetWindowPadding();

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
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x);
				if (m_NameInput.Render(imgui::IMGUI_FORMAT_ID("", INPUT_ID, "NAME_INSPECTOR").c_str(), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					if (a_Resource->Rename(m_NameInput.GetString()))
					{
						SetData(a_Resource);
					}
				}

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

				ImGui::SetCursorPosY(y + (core::ENGINE.GetEditor().GetImGuiWindow().HeaderSize().y * 1.5f));

				if (ImGui::TransparentButton(IMGUI_FORMAT_ID(std::string(ICON_FA_ARROW_TURN_UP), BUTTON_ID, "SHOW_IN_EXPLORER_INSPECTOR").c_str(), ImVec2(core::ENGINE.GetEditor().GetImGuiWindow().HeaderSize().y, core::ENGINE.GetEditor().GetImGuiWindow().HeaderSize().y)))
				{
					file::FileLoader::OpenInExplorer(a_Resource->GetParent()->GetPath().c_str());
				}
				ImGui::SameLine();
				if (ImGui::TransparentButton(IMGUI_FORMAT_ID(std::string(ICON_FA_DELETE), BUTTON_ID, "DELETE_INSPECTOR").c_str(), ImVec2(core::ENGINE.GetEditor().GetImGuiWindow().HeaderSize().y, core::ENGINE.GetEditor().GetImGuiWindow().HeaderSize().y)))
				{
					core::ENGINE.GetEditor().SetSelectable(nullptr);

					a_Resource->Delete();

					core::ENGINE.GetEditor().GetAssetDatabase().Rescan();
				}
				ImGui::EndToolbar(padding);

				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
			}

			void ExplorerResourceUIView::EndBaseSelectable()
			{
			}

			bool ExplorerResourceUIView::HasFolders() const
			{
				for (ExplorerResourceUIView* resource : m_Resources)
				{
					if (resource->m_ResourceType == ExplorerResourceType::Folder)
					{
						return true;
					}
				}
				return false;
			}












































































































			//ExplorerResourceUIView::ExplorerResourceUIView(ImGuiWindow& a_Window, ExplorerResource& a_Resource) : EditorSelectable(a_Window), m_Resource(a_Resource)
			//{
			//	ReadResourceData();
			//}

   //         ExplorerResourceUIView::ExplorerResourceUIView(const ExplorerResourceUIView& a_Other) : EditorSelectable(a_Other), m_Resource(a_Other.m_Resource)
			//{
			//	// Assign simple data members
			//	m_Name = a_Other.m_Name;
			//	m_Icon = a_Other.m_Icon;
			//	m_FoldedOut = a_Other.m_FoldedOut;

			//	// Clean up existing parent pointer if necessary and assign the new one
			//	m_Parent = a_Other.m_Parent;

			//	// Clear and copy the resource children
			//	m_Resources.clear();
			//	m_Resources = a_Other.m_Resources;

			//	// Copy inspector data
			//	m_NameInput = a_Other.m_NameInput;
			//	m_AssetTypeDropdown = a_Other.m_AssetTypeDropdown;

			//	m_ResourceData = a_Other.m_ResourceData;
			//}

			//ExplorerResourceUIView& ExplorerResourceUIView::operator=(const ExplorerResourceUIView& a_Other)
			//{
			//	if (this != &a_Other)
			//	{
			//		// Assign simple data members
			//		m_Name = a_Other.m_Name;
			//		m_Icon = a_Other.m_Icon;
			//		m_FoldedOut = a_Other.m_FoldedOut;

			//		// Assign the resource, assuming shallow copy is fine (if not, handle accordingly)
			//		m_Resource = a_Other.m_Resource;

			//		// Clean up existing parent pointer if necessary and assign the new one
			//		m_Parent = a_Other.m_Parent;

			//		// Clear and copy the resource children
			//		m_Resources.clear();
			//		m_Resources = a_Other.m_Resources;

			//		// Copy inspector data
			//		m_NameInput = a_Other.m_NameInput;
			//		m_AssetTypeDropdown = a_Other.m_AssetTypeDropdown;

			//		m_ResourceData = a_Other.m_ResourceData;
			//	}
			//	return *this;
			//}

			//void ExplorerResourceUIView::ReadResourceData()
			//{
			//	switch (m_Resource.GetAssetType())
			//	{
			//		case assets::AssetType::Sprite:
			//		case assets::AssetType::Texture:
			//		case assets::AssetType::Font:
			//		{
			//			m_ResourceData.image = dynamic_cast<ImageExplorerResource*>(&m_Resource);
			//			m_AssetTypeDropdown = StringDropdown<assets::AssetType>(
			//				m_Resource.GetAssetType(),
			//				{
			//					assets::AssetType::Texture,
			//					assets::AssetType::Sprite,
			//					assets::AssetType::Font,
			//				},
			//				{
			//					assets::AssetTypeToString(assets::AssetType::Texture),
			//					assets::AssetTypeToString(assets::AssetType::Sprite),
			//					assets::AssetTypeToString(assets::AssetType::Font),
			//				}
			//				);
			//			break;
			//		}
			//		case assets::AssetType::Sound:
			//		case assets::AssetType::Song:
			//		case assets::AssetType::VO:
			//		{
			//			//m_SoundResource = dynamic_cast<ImageExplorerResource*>(a_Resource);
			//			m_AssetTypeDropdown = StringDropdown<assets::AssetType>(
			//				m_Resource.GetAssetType(),
			//				{
			//					assets::AssetType::Sound,
			//					assets::AssetType::Song,
			//					assets::AssetType::VO,
			//				},
			//				{
			//					assets::AssetTypeToString(assets::AssetType::Sound),
			//					assets::AssetTypeToString(assets::AssetType::Song),
			//					assets::AssetTypeToString(assets::AssetType::VO),
			//				}
			//				);
			//			break;
			//		}
			//		case assets::AssetType::Scene:
			//		{
			//			m_ResourceData.scene = dynamic_cast<SceneExplorerResource*>(&m_Resource);
			//			break;
			//		}
			//		default:
			//		{
			//			break;
			//		}
			//	}

			//	m_Name = string_extensions::GetFileWithoutExtension(string_extensions::GetFileName(m_Resource.GetPath()));
			//	m_Icon = m_Resource.GetResourceType() == ExplorerResourceType::Folder ? ICON_FA_FOLDER : RESOURCE_ICONS[(int)m_Resource.GetAssetType()];

			//	GetChildren();
			//}

			//const ExplorerResource& ExplorerResourceUIView::GetResource() const
			//{
			//	return m_Resource;
			//}

			//void ExplorerResourceUIView::ClearChildren()
			//{
			//	m_Resources.clear();
			//}

			//void ExplorerResourceUIView::GetChildren()
			//{
			//	ClearChildren();
			//	CreateExplorerResourceUIViewChild(*this, m_Resource, m_Window);
			//}

			//void ExplorerResourceUIView::RenderIcon()
			//{
			//	switch (m_Resource.GetAssetType())
			//	{
			//		case assets::AssetType::Sprite:
			//		case assets::AssetType::Texture:
			//		case assets::AssetType::Font:
			//		{
			//			if (!m_ResourceData.image)
			//			{
			//				return;
			//			}

			//			if (!m_ResourceData.image->m_DescHandle->Invalid())
			//			{
			//				const float width_new = 15;
			//				const float height_new = (m_ResourceData.image->m_DescHandle->Height * (1.0f / m_ResourceData.image->m_DescHandle->Width * width_new));
			//				ImGui::Image((void*)m_ResourceData.image->m_DescHandle->GpuHandle.ptr, ImVec2(width_new, height_new));
			//			}
			//			break;
			//		}
			//		default:
			//		{
			//			ImGui::Text(m_Resource.GetResourceType() == ExplorerResourceType::Folder ? ICON_FA_FOLDER : m_Icon.c_str());
			//			break;
			//		}
			//	}
			//}

   //         void ExplorerResourceUIView::DoubleClicked()
   //         {
			//	switch (m_Resource.GetAssetType())
			//	{
			//		case assets::AssetType::Scene:
			//		{
			//			core::ENGINE.GetEditor().SetCurrentScene(m_ResourceData.scene);
			//			break;
			//		}
			//	}
   //         }

			//bool ExplorerResourceUIView::HasFolders() const
			//{
			//	for (auto& resource : m_Resources)
			//	{
			//		if (resource.m_Resource.GetResourceType() == ExplorerResourceType::Folder)
			//		{
			//			return true;
			//		}
			//	}
			//	return false;
			//}

			//void ExplorerResourceUIView::RenderSelectable()
			//{

			//	popGuard.PopStyleVar();
			//	popGuard.PopStyleVar();

			//	if (m_Resource.GetResourceType() == ExplorerResourceType::Folder)
			//	{
			//		return;
			//	}

			//	ImVec2 framePadding = core::ENGINE.GetEditor().GetImGuiWindow().GetFramePadding();
			//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(framePadding.x * 2, framePadding.y * 2));
			//	if (ImGui::BeginChild(
			//		imgui::IMGUI_FORMAT_ID("", CHILD_ID, "EXPLORER_RESOURCE_INSPECTOR").c_str(),
			//		ImVec2(
			//			ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x,
			//			ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y
			//		),
			//		ImGuiChildFlags_Borders
			//	))
			//	{
			//		ImGui::DisplayHeader(core::ENGINE.GetEditor().GetImGuiWindow().Bold(), "Type");
			//		ImGui::SameLine();

			//		switch (m_Resource.GetAssetType())
			//		{
			//			case assets::AssetType::Sprite:
			//			case assets::AssetType::Texture:
			//			case assets::AssetType::Font:
			//			case assets::AssetType::Sound:
			//			case assets::AssetType::Song:
			//			case assets::AssetType::VO:
			//			{
			//				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
			//				if (m_AssetTypeDropdown.Render(imgui::IMGUI_FORMAT_ID("", COMBO_ID, "ASSETTYPE_INSPECTOR").c_str()))
			//				{
			//					m_Resource.SetAssetType(m_AssetTypeDropdown.GetValue());
			//					m_Resource.SaveMetadata();
			//				}
			//				ImGui::PopStyleVar();
			//				break;
			//			}
			//			default:
			//			{
			//				ImGui::Text(assets::AssetTypeToString(m_Resource.GetAssetType()).c_str());
			//				break;
			//			}
			//		}

			//		switch (m_Resource.GetAssetType())
			//		{
			//			case assets::AssetType::Sprite:
			//			case assets::AssetType::Texture:
			//			case assets::AssetType::Font:
			//			{
			//				if (!m_ResourceData.image->m_DescHandle->Invalid())
			//				{
			//					const float width_new = ImGui::GetContentRegionAvail().x;
			//					const float height_new = (m_ResourceData.image->m_DescHandle->Height * (1.0f / m_ResourceData.image->m_DescHandle->Width * width_new));
			//					ImGui::Image((void*)m_ResourceData.image->m_DescHandle->GpuHandle.ptr, ImVec2(width_new, height_new));
			//				}
			//				break;
			//			}
			//			default:
			//			{
			//				break;
			//			}
			//		}
			//	}
			//	ImGui::PopStyleVar();
			//	ImGui::EndChild();
			//}
		}
	}
}

#endif // __EDITOR__