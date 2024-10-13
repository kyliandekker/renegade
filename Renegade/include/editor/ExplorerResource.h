#pragma once

#include <vector>
#include <string>
#include <rapidjson/document.h>

#include "assets/AssetType.h"

#ifdef __EDITOR__

namespace renegade
{
	namespace editor
	{
		enum class ExplorerResourceEditMode
		{
			None,
			Rename
		};

		enum class ExplorerResourceType
		{
			Folder,
			File,
		};

		class ExplorerResource
		{
		public:
			~ExplorerResource();

			std::vector<ExplorerResource*> m_Resources;
			std::string m_Path;
			std::string m_Name;
			std::string m_NameWithoutExtension;
			ExplorerResourceType m_ResourceType = ExplorerResourceType::Folder;

			ExplorerResource* m_Parent = nullptr;

			bool m_FoldedOut = false;
			bool m_Show = true;
			std::vector<ExplorerResource*> m_Parents;
			ExplorerResourceEditMode m_ResourceEditMode = ExplorerResourceEditMode::None;

			bool Scan();
			bool Rename(const std::string& a_Name);
			void Delete();

            bool HasFolders() const;

			assets::AssetType GetAssetType() const;
			void SetAssetType(assets::AssetType a_AssetType);

			bool LoadMetadata();
			virtual bool ProcessMetadata(const rapidjson::Document& a_JsonFile);

			bool SaveMetadata(const rapidjson::Document& a_JsonFile) const;
			virtual virtual rapidjson::Document PrepareMetadata() const;

			static std::string GetUniqueName(const ExplorerResource& a_Resource, const std::string& a_Name);
		private:
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

		class TextureExplorerResource : public ExplorerResource
		{
		public:
		};

		class SpriteExplorerResource : public ExplorerResource
		{
		public:
		};

		class FontExplorerResource : public ExplorerResource
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