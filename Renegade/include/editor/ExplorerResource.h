#pragma once

#include <vector>
#include <string>
#include <rapidjson/document.h>
#include <imgui/imgui.h>

#include "assets/AssetType.h"
#include "graphics/DescHandle.h"
#include "editor/EditorSelectable.h"

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

		// TODO: This is a monster class and needs to be split up asap. The explorer functionality should not be mixed with the imgui functionality in any sort of way.
		class ExplorerResource : public EditorSelectable
		{
		public:
			virtual ~ExplorerResource();

			std::string m_Name;
			std::string m_Icon;
			std::vector<ExplorerResource*> m_Resources;
			std::string m_Path;
			std::string m_NameWithExtension;
			ExplorerResourceType m_ResourceType = ExplorerResourceType::Folder;

			ExplorerResource* m_Parent = nullptr;

			bool m_FoldedOut = false;
			bool m_Show = true;
			std::vector<ExplorerResource*> m_Parents;
			ExplorerResourceEditMode m_ResourceEditMode = ExplorerResourceEditMode::None;

			virtual bool Initialize() { return true; };
			virtual void RenderIcon(const char* icon);
			void Render(bool& clicked, bool& right_clicked, bool& double_clicked, bool selected, const char* icon, const char* label2);
			void RenderSelectable() override;
			virtual void RenderInnerProperties();

			bool Scan();
			bool Rename(const std::string& a_Name);
			void Delete();

            bool HasFolders() const;

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

		class TextureExplorerResource : public ExplorerResource
		{
		public:
			~TextureExplorerResource() override;

			void RenderInnerProperties() override;

			void RenderIcon(const char* icon) override;
			bool Initialize() override;
			graphics::DescHandle* m_DescHandle;
		};

		class SpriteExplorerResource : public TextureExplorerResource
		{
		public:
		};

		class FontExplorerResource : public TextureExplorerResource
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