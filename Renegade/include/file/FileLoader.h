#pragma once

#include <string>

namespace renegade
{
	namespace core
	{
		class Data;
		class DataStream;
	}

	namespace file
	{
		class FileLoader
		{
		public:
			static bool LoadFile(const std::string& a_Path, core::DataStream& a_Data);
			static bool SaveFile(const std::string& a_Path, const core::Data& a_Data);
			static bool DoesFileExist(const std::string& a_Path);
			static bool CreateFolder(const std::string& a_Path);
			static bool OpenInExplorer(const std::string& a_Path);
			static std::string GetAppDataPath();
		};
	}
}