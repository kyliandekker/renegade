#pragma once

#ifdef __EDITOR__

#include <vector>
#include <string>
#include <rapidjson/document.h>
#include <imgui/imgui.h>

#include "assets/AssetType.h"
#include "graphics/DescHandle.h"

namespace renegade
{
	namespace editor
	{
		enum class ExplorerResourceType
		{
			Folder,
			File,
		};

		class ExplorerResource
		{
			friend class AssetDatabase;
		public:
			virtual ~ExplorerResource();

			const std::string& GetPath() const;
			ExplorerResourceType GetResourceType() const;
			ExplorerResource* GetParent() const;

			std::vector<ExplorerResource*> m_Resources;

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

			virtual void SetDirty();
			virtual void ResetDirty();
			bool IsDirty() const;
		protected:
			bool m_IsDirty = false;
			std::string m_Path;
			assets::AssetType m_AssetType;
			ExplorerResourceType m_ResourceType = ExplorerResourceType::Folder;
			ExplorerResource* m_Parent = nullptr;
		};

		class ConfigExplorerResource : public ExplorerResource
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

		class AudioExplorerResource : public ExplorerResource
		{
		public:
			virtual ~AudioExplorerResource() override;

			bool Initialize() override;
			double* m_LeftSamples = nullptr;
			double* m_RightSamples = nullptr;
			size_t m_NumSamples = 0;
		};

		class SoundExplorerResource : public AudioExplorerResource
		{
		public:
		};

		class SongExplorerResource : public AudioExplorerResource
		{
		public:
		};

		class VOExplorerResource : public AudioExplorerResource
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