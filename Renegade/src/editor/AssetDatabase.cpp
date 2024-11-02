#ifdef __EDITOR__

#include "editor/AssetDatabase.h"

#include "logger/Logger.h"
#include <windows.h>

namespace renegade
{
	namespace editor
	{
		std::string ExePath()
		{
			CHAR buffer[MAX_PATH] = { 0 };
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			std::string::size_type pos = std::string(buffer).find_last_of("\\/");
			return std::string(buffer).substr(0, pos);
		}

		bool AssetDatabase::Initialize(int, ...)
		{
			if (Scan())
			{
				LOG(LOGSEVERITY_SUCCESS, "AssetDatabase initialized.");
				return System::Initialize();
			}
			return false;
		}

		bool AssetDatabase::Destroy()
		{
			LOG(LOGSEVERITY_SUCCESS, "AssetDatabase destroyed.");
			return System::Destroy();
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
			return m_Root;
		}

		bool AssetDatabase::Scan()
		{
			m_OnBeforeScan();

			m_Ready = false;
			m_Root.m_Path = ExePath() + "/assets";
			if (m_Root.Scan())
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