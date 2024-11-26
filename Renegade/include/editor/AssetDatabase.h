#pragma once

#ifdef __EDITOR__

#include <string>

#include "editor/ExplorerResource.h"
#include "core/System.h"
#include "core/datatypes/Event.h"
#include "project/ProjectSettings.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class EditorSelectable;
		}

		class AssetDatabase : public core::System
		{
		public:
			bool Initialize(int a_NumArgs = 0, ...) override;
			bool Destroy() override;

			bool LoadProject(const std::string& a_Path);

			void Rescan();
			void CheckAssetDatabase();

			ExplorerResource& GetRoot();
			project::ProjectSettings& GetProjectSettings();

			SimpleEvent<> m_OnScanCompleted;
			SimpleEvent<> m_OnBeforeScan;
			Event<> m_OnProjectLoaded;
		private:
			bool m_Rescan;
			bool Scan();
			ExplorerResource m_AssetsRoot;
			project::ProjectSettings m_ProjectSettings;

			std::string m_ProjectPath;
		};
	}
}

#endif // __EDITOR__