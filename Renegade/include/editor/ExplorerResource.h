#pragma once

#include <vector>
#include <string>
#include <rapidjson/document.h>
#include <imgui/imgui.h>

#include "assets/AssetType.h"
#include "graphics/DescHandle.h"

#ifdef __EDITOR__

namespace renegade
{
	namespace editor
	{
		enum class ExplorerResourceType
		{
			Folder,
			File,
		};

		// TODO: This is a monster class and needs to be split up asap. The explorer functionality should not be mixed with the imgui functionality in any sort of way.
		class ExplorerResource
		{
		public:
			virtual ~ExplorerResource();

			std::vector<ExplorerResource*> m_Resources;
			std::string m_Path;
			ExplorerResourceType m_ResourceType = ExplorerResourceType::Folder;

			ExplorerResource* m_Parent = nullptr;

			virtual bool Initialize() { return true; };

			bool Scan();
			bool Rename(const std::string& a_Name);
			void Delete();

			assets::AssetType GetAssetType() const;
			void SetAssetType(assets::AssetType a_AssetType);

			bool LoadMetadata();
			virtual bool ProcessMetadata(const rapidjson::Document& a_JsonFile);

			bool SaveMetadata() const;
			virtual rapidjson::Document PrepareMetadata() const;

			static std::string GetUniqueName(const ExplorerResource& a_Resource, const std::string& a_Name);
		protected:
			assets::AssetType m_AssetType;
		};

		class ConfigExplorerResource : public ExplorerResource
		{
		public:
		};

		class SceneExplorerResource : public ExplorerResource
		{
		public:
		};

		class MaterialExplorerResource : public ExplorerResource
		{
		public:
		};

		class ImageExplorerResource : public ExplorerResource
		{
		public:
			virtual ~ImageExplorerResource() override;

			bool Initialize() override;
			graphics::DescHandle* m_DescHandle;
		};

		class TextureExplorerResource : public ImageExplorerResource
		{
		public:
		};

		class SpriteExplorerResource : public ImageExplorerResource
		{
		public:
		};

		class FontExplorerResource : public ImageExplorerResource
		{
		public:
		};

		class SoundExplorerResource : public ExplorerResource
		{
		public:
		};

		class SongExplorerResource : public ExplorerResource
		{
		public:
		};

		class VOExplorerResource : public ExplorerResource
		{
		public:
		};

		class AnimationExplorerResource : public ExplorerResource
		{
		public:
		};

		class LocalizationExplorerResource : public ExplorerResource
		{
		public:
		};

		class ModelExplorerResource : public ExplorerResource
		{
		public:
		};

		class PrefabExplorerResource : public ExplorerResource
		{
		public:
		};
	}
}

#endif // __EDITOR__