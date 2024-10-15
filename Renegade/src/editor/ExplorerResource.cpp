#ifdef __EDITOR__

#include "editor/ExplorerResource.h"

#include <unordered_map>
#include <filesystem>
// GetFileAttributes, FILE_ATTRIBUTE_HIDDEN, FILE_ATTRIBUTE_SYSTEM
#include <wtypes.h>
#include <rapidjson/utils.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include "utils/string_extensions.h"
#include "core/datatypes/DataStream.h"
#include "file/FileLoader.h"
#include "logger/Logger.h"
#include "core/Engine.h"
#include "editor/imgui/ImGuiDefines.h"
#include "utils/string_extensions.h"

namespace fs = std::filesystem;

// General Metadata Variables
#define JSON_ASSETTYPE_VAR "assetType"

namespace renegade
{
	namespace editor
	{
		std::vector<ExplorerResource*> GetParents(ExplorerResource& a_Resource)
		{
			std::vector<ExplorerResource*> parents;
			ExplorerResource* parent = &a_Resource;
			while (parent)
			{
				parents.push_back(parent);
				parent = parent->m_Parent;
			}
			std::reverse(parents.begin(), parents.end());
			return parents;
		}

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

		void ExplorerResource::RenderIcon(const char* icon)
		{
			ImGui::Text(icon);
		}

		void ExplorerResource::Render(bool& clicked, bool& right_clicked, bool& double_clicked, bool selected, const char* icon, const char* label2)
		{
			ImVec2 pos = ImGui::GetCursorScreenPos();

			clicked = ImGui::Selectable(imgui::IMGUI_FORMAT_ID("", SELECTABLE_ID, "RESOURCE_" + string_extensions::StringToUpper(m_Name)).c_str(), &selected);
			right_clicked = ImGui::IsItemHovered() && ImGui::IsItemClicked(1);
			double_clicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);

			ImGui::SetCursorScreenPos(pos);
			ImGui::SetCursorScreenPos(ImVec2(pos.x + 10, pos.y));
			RenderIcon(icon);
			ImGui::SetCursorScreenPos(ImVec2(pos.x + 35, pos.y));
			ImGui::Text(m_Name.c_str());

			ImGui::SetCursorScreenPos(ImVec2(pos.x + 300, pos.y));
			ImVec4 textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
			textColor.w = 0.5f;
			ImGui::TextColored(textColor, label2);
		}

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

			m_NameWithExtension = string_extensions::GetFileName(m_Path);
			if (m_NameWithExtension.empty())
			{
				m_NameWithExtension = m_Path;
			}

			m_Name = string_extensions::GetFileWithoutExtension(m_NameWithExtension);

			// This is for the editor so that we can show the path like this: "Root -> Images -> Haan.png"
			m_Parents = GetParents(*this);

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
			if (!string_extensions::EndsWith(a_Name, ".MD"))
			{
				complete_name = a_Name + ".MD";
			}
			std::string new_path = string_extensions::GetPath(m_Path) + "/" + complete_name;
			return std::rename(m_Path.c_str(), new_path.c_str());
		}

		void ExplorerResource::Delete()
		{
			fs::remove(m_Path.c_str());
		}

		bool ExplorerResource::HasFolders() const
		{
			for (ExplorerResource* resource : m_Resources)
			{
				if (resource->m_ResourceType == ExplorerResourceType::Folder)
				{
					return true;
				}
			}
			return false;
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
					if (link->m_Name == a_Name && link->m_ResourceType == a_Resource.m_ResourceType && link->m_AssetType == a_Resource.m_AssetType)
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

		TextureExplorerResource::~TextureExplorerResource()
		{
			if (m_DescHandle)
			{
				m_DescHandle->Release();
			}
		}

		void TextureExplorerResource::RenderIcon(const char*)
		{
			if (!m_DescHandle->Invalid())
			{
				const float width_new = 15;
				const float height_new = (m_DescHandle->Height * (1.0f / m_DescHandle->Width * width_new));
				ImGui::Image((void*)m_DescHandle->GpuHandle.ptr, ImVec2(width_new, height_new));
			}
		}

		bool TextureExplorerResource::Initialize()
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