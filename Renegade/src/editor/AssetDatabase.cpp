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
				LOGF(LOGSEVERITY_SUCCESS, "Initialized AssetDatabase.");
				return System::Initialize();
			}
			return false;
		}

		bool AssetDatabase::Destroy()
		{
			return System::Destroy();
		}

		bool AssetDatabase::Scan()
		{
			m_Ready = false;
			m_Root.m_Path = ExePath() + "/assets";
			m_Root.m_FoldedOut = true;
			if (m_Root.Scan())
			{
				LOGF(LOGSEVERITY_SUCCESS, "Scanned asset database.");
				m_Ready = true;
			}
			return m_Ready;
		}
	}
}