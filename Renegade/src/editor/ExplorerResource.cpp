#ifdef __EDITOR__

#include "editor/ExplorerResource.h"

#include <unordered_map>
#include <filesystem>
// GetFileAttributes, FILE_ATTRIBUTE_HIDDEN, FILE_ATTRIBUTE_SYSTEM
#include <wtypes.h>
#include <rapidjson/utils.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <imgui/imgui_helpers.h>

#include "utils/string_extensions.h"
#include "core/datatypes/DataStream.h"
#include "file/FileLoader.h"
#include "logger/Logger.h"
#include "core/Engine.h"
#include "editor/imgui/ImGuiDefines.h"
#include "utils/string_extensions.h"
#include "editor/imgui/StringTextInput.h"

namespace fs = std::filesystem;

// General Metadata Variables
#define JSON_ASSETTYPE_VAR "assetType"

namespace renegade
{
	namespace editor
	{
		std::string getMetadataPath(const std::string& a_Path)
		{
			return a_Path + ".meta";
		}

		bool loadMetadata(std::string a_Path, rapidjson::Document& a_Document)
		{
			core::DataStream data;
			if (!file::FileLoader::LoadFile(getMetadataPath(a_Path), data))
			{
				return false;
			}

			a_Document.Parse(reinterpret_cast<char*>(data.data()), data.size());

			if (a_Document.HasParseError())
			{
				LOGF(LOGSEVERITY_ERROR, "Something went wrong when trying to load meta file '%s'.", getMetadataPath(a_Path).c_str());
				return false;
			}

			return true;
		}

		bool saveMetadata(const rapidjson::Document& a_JsonFile, const std::string& a_Path)
		{
			rapidjson::StringBuffer buffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
			a_JsonFile.Accept(writer);

			return file::FileLoader::SaveFile(getMetadataPath(a_Path), core::Data(buffer.GetString(), buffer.GetSize()));
		}

		ExplorerResource::~ExplorerResource()
		{
			for (auto resource : m_Resources)
			{
				delete resource;
			}
		}

		const std::string& ExplorerResource::GetPath() const
        {
            return m_Path;
        }

		ExplorerResourceType ExplorerResource::GetResourceType() const
		{
			return m_ResourceType;
		}

		ExplorerResource* ExplorerResource::GetParent() const
		{
			return m_Parent;
		}

		//imgui::StringTextInput EXPLORER_TEXT_INPUT;
		//void ExplorerResource::RenderSelectable()
		//{
		//	ImVec2 toolbarSize = ImVec2(ImGui::GetContentRegionAvail().x, core::ENGINE.GetEditor().GetImGuiWindow().HeaderSize().y * 2);
		//	ImGui::BeginToolbar(toolbarSize);

		//	ImVec2 padding = core::ENGINE.GetEditor().GetImGuiWindow().GetWindowPadding();
		//	ImGui::EndToolbar(padding);

		//	float fontSize = core::ENGINE.GetEditor().GetImGuiWindow().FontSize();

		//	float y = ImGui::GetCursorPosY();
		//	float x = ImGui::GetCursorPosX() + fontSize;
		//	ImGui::SetCursorPos(ImVec2(x, y + fontSize));
		//	ImGui::PushFont(core::ENGINE.GetEditor().GetImGuiWindow().CapitalIcon());
		//	ImGui::Text(m_Icon.c_str());
		//	ImGui::PopFont();

		//	ImGui::SetCursorPosY(y + (fontSize / 2));
		//	ImGui::SetCursorPosX(x + (fontSize * 3));

		//	EXPLORER_TEXT_INPUT.SetString(m_Name);
		//	if (EXPLORER_TEXT_INPUT.Render(imgui::IMGUI_FORMAT_ID("", INPUT_ID, "NAME_INSPECTOR").c_str(), ImGuiInputTextFlags_EnterReturnsTrue))
		//	{
		//		Rename(EXPLORER_TEXT_INPUT.GetString());
		//	}

		//	ImGui::SetCursorPosY(y + toolbarSize.y);
		//	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().ItemSpacing.x);

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
		//		RenderInnerProperties();
		//	}
		//	ImGui::PopStyleVar();
		//	ImGui::EndChild();
		//}

		bool ExplorerResource::Scan()
		{
			// TODO: Fix
			const std::unordered_map<std::string, std::vector<assets::AssetType>> WAT =
			{
				{ "cfg", { assets::AssetType::Cfg }  },
				{ "scene", { assets::AssetType::Scene }  },
				{ "mat", { assets::AssetType::Material }  },
				{ "png", { assets::AssetType::Texture, assets::AssetType::Sprite, assets::AssetType::Font }  },
				{ "tif", { assets::AssetType::Texture, assets::AssetType::Sprite, assets::AssetType::Font }  },
				{ "bmp", { assets::AssetType::Texture, assets::AssetType::Sprite, assets::AssetType::Font }  },
				{ "wav", { assets::AssetType::Sound, assets::AssetType::Song }  },
				{ "anim", { assets::AssetType::Animation }  },
				{ "loc", { assets::AssetType::Localization }  },
				{ "gltf", { assets::AssetType::Model } },
			};

			// Replace the slashes so that all paths use the same slash.
			m_Path = string_extensions::ReplaceSlashes(m_Path);

			if (!fs::exists(m_Path))
			{
				return false;
			}

			m_ResourceType = fs::is_directory(m_Path) ? ExplorerResourceType::Folder : ExplorerResourceType::File;

			// Scan the folder.
			if (m_ResourceType == ExplorerResourceType::Folder)
			{
				// Clear all resources first.
				for (auto resource : m_Resources)
				{
					delete resource;
				}
				m_Resources.clear();

				// Go through each file/folder and check their status.
				fs::directory_iterator ds = fs::directory_iterator(m_Path, std::filesystem::directory_options::skip_permission_denied);
				for (const auto& dirEntry : ds)
				{
					std::string path = dirEntry.path().string().c_str();
					std::wstring wsPath = std::wstring(path.begin(), path.end());

					// Check for hidden or system files. These should not be added.
					DWORD attributes = GetFileAttributes(wsPath.c_str());
					if (attributes & FILE_ATTRIBUTE_HIDDEN || attributes & FILE_ATTRIBUTE_SYSTEM)
					{
						continue;
					}

					// If it is not a directory, it is a file and needs to get past the meta checks.
					if (!fs::is_directory(path))
					{
						std::string extension = string_extensions::StringToLower(string_extensions::GetExtensionFromPath(path));

						// Get the extension. If the extension is not recognized, it will just be ignored.
						if (WAT.find(extension) == WAT.end())
						{
							continue;
						}

						ExplorerResource* resource = nullptr;

						auto it = WAT.find(extension);
						assets::AssetType assetType = it->second[0];

						rapidjson::Document document;
						bool hasMetadata = loadMetadata(path, document);
						if (hasMetadata)
						{
							int iAssetType = 0;
							rapidjson::GetInt(document, JSON_ASSETTYPE_VAR, iAssetType);
							assetType = (assets::AssetType)iAssetType;
						}

						switch (assetType)
						{
							case assets::AssetType::Cfg:
							{
								resource = new ConfigExplorerResource();
								break;
							}
							case assets::AssetType::Scene:
							{
								resource = new SceneExplorerResource();
								break;
							}
							case assets::AssetType::Material:
							{
								resource = new MaterialExplorerResource();
								break;
							}
							case assets::AssetType::Texture:
							{
								resource = new TextureExplorerResource();
								break;
							}
							case assets::AssetType::Sprite:
							{
								resource = new SpriteExplorerResource();
								break;
							}
							case assets::AssetType::Font:
							{
								resource = new FontExplorerResource();
								break;
							}
							case assets::AssetType::Sound:
							{
								resource = new SoundExplorerResource();
								break;
							}
							case assets::AssetType::Song:
							{
								resource = new SongExplorerResource();
								break;
							}
							case assets::AssetType::VO:
							{
								resource = new VOExplorerResource();
								break;
							}
							case assets::AssetType::Animation:
							{
								resource = new AnimationExplorerResource();
								break;
							}
							case assets::AssetType::Localization:
							{
								resource = new LocalizationExplorerResource();
								break;
							}
							case assets::AssetType::Model:
							{
								resource = new ModelExplorerResource();
								break;
							}
							case assets::AssetType::Prefab:
							{
								resource = new PrefabExplorerResource();
								break;
							}
						}

						if (resource != nullptr)
						{
							resource->m_Path = path;
							resource->m_Parent = this;
							resource->m_ResourceType = ExplorerResourceType::File;
							resource->m_AssetType = assetType;
							resource->Initialize();
							if (hasMetadata)
							{
								resource->ProcessMetadata(document);
							}
							else
							{
								resource->SaveMetadata();
							}
							m_Resources.push_back(resource);
						}
					}
					else
					{
						// Create the resource that will be added.
						ExplorerResource* explorerResource = new ExplorerResource();
						explorerResource->m_Path = path;
						explorerResource->m_Parent = this;
						explorerResource->m_ResourceType = ExplorerResourceType::Folder;

						m_Resources.push_back(explorerResource);
					}
				}
			}

			for (auto& resource : m_Resources)
			{
				resource->Scan();
			}

			return true;
		}

		bool ExplorerResource::Rename(const std::string& a_Name)
		{
			std::string complete_name = a_Name;
			if (m_ResourceType == ExplorerResourceType::File)
			{
				complete_name += string_extensions::GetExtensionFromPath(m_Path, true);
			}
			std::string new_path = string_extensions::GetPath(m_Path) + "/" + complete_name;
			if (std::rename(m_Path.c_str(), new_path.c_str()) == 0)
			{
				m_Path = new_path;

				Scan();
				return true;
			}
			return false;
		}

		void ExplorerResource::Delete()
		{
			fs::remove(m_Path.c_str());
		}

		assets::AssetType ExplorerResource::GetAssetType() const
		{
			return m_AssetType;
		}

		void ExplorerResource::SetAssetType(assets::AssetType a_AssetType)
		{
			m_AssetType = a_AssetType;
		}

		bool ExplorerResource::LoadMetadata()
		{
			rapidjson::Document document;
			loadMetadata(m_Path, document);
			return ProcessMetadata(document);
		}

		bool ExplorerResource::ProcessMetadata(const rapidjson::Document& a_JsonFile)
		{
			int assetType = 0;
			rapidjson::GetInt(a_JsonFile, JSON_ASSETTYPE_VAR, assetType);
			m_AssetType = (assets::AssetType)assetType;

			return true;
		}
		
		bool ExplorerResource::SaveMetadata() const
		{
			rapidjson::Document document = PrepareMetadata();
			return saveMetadata(document, m_Path);
		}

		rapidjson::Document ExplorerResource::PrepareMetadata() const
		{
			rapidjson::Document document;
			document.SetObject();
			rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

			int assetType = (int)m_AssetType;
			document.AddMember(JSON_ASSETTYPE_VAR, assetType, allocator);

			return document;
		}

		std::string ExplorerResource::GetUniqueName(const ExplorerResource& a_Resource, const std::string& a_Name)
		{
			std::string name = a_Name;
			int i = 0;
			bool found = true;
			while (found)
			{
				found = false;
				for (auto& link : a_Resource.m_Resources)
				{
					std::string linkName = string_extensions::GetFileWithoutExtension(string_extensions::GetFileName(link->m_Path));
					if (linkName == a_Name && link->m_ResourceType == a_Resource.m_ResourceType && link->m_AssetType == a_Resource.m_AssetType)
					{
						found = true;
						i++;
					}
				}
				if (found)
				{
					name = a_Name + " (" + std::to_string(i) + ")";
				}
			}
			return name;
		}

		ImageExplorerResource::~ImageExplorerResource()
		{
			if (m_DescHandle)
			{
				m_DescHandle->Release();
			}
		}

		bool ImageExplorerResource::Initialize()
		{
			if (!core::ENGINE.GetWindow().GetDX12Window().LoadTexture(m_Path, m_DescHandle))
			{
				return false;
			}

			return true;
		}
}
}

#endif // __EDITOR__