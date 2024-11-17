#ifdef __EDITOR__

#include "editor/AssetDatabase.h"

#include "logger/Logger.h"
#include "file/FileLoader.h"
#include "core/Engine.h"

namespace renegade
{
	namespace editor
	{
		bool AssetDatabase::Initialize(int, ...)
		{
			return System::Initialize();
		}

		bool AssetDatabase::Destroy()
		{
			LOG(LOGSEVERITY_SUCCESS, "AssetDatabase destroyed.");
			return System::Destroy();
		}

        bool AssetDatabase::LoadProject(const std::string& a_Path)
        {
			m_ProjectPath = a_Path;

			core::ENGINE.GetEditor().GetEditorSettings().AddToPreviousProjects(m_ProjectPath);

			Scan();
			m_OnProjectLoaded();

			m_ProjectSettings.Load(m_ProjectPath);

            return false;
        }

		void AssetDatabase::Rescan()
		{
			m_Rescan = true;
		}

		void AssetDatabase::CheckAssetDatabase()
		{
			if (m_Rescan)
			{
				m_Rescan = false;

				Scan();
			}
		}

		ExplorerResource& AssetDatabase::GetRoot()
		{
			return m_AssetsRoot;
		}

		bool AssetDatabase::Scan()
		{
			m_OnBeforeScan();

			m_Ready = false;
			std::string path = m_ProjectPath + "/assets";
			if (!file::FileLoader::DoesFolderExist(path))
			{
				file::FileLoader::CreateFolder(path);
			}
			m_AssetsRoot.m_Path = path;
			if (m_AssetsRoot.Scan())
			{
				LOG(LOGSEVERITY_SUCCESS, "Scanned asset database.");
				m_Ready = true;
				m_OnScanCompleted();
			}
			return m_Ready;
		}
	}
}

#endif // __EDITOR__