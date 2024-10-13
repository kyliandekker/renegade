#include "file/FileLoader.h"

#include <filesystem>
#include <ShlObj_core.h>

#include "core/datatypes/Data.h"
#include "core/datatypes/DataStream.h"

namespace fs = std::filesystem;

namespace renegade
{
	namespace file
	{
		bool FileLoader::LoadFile(const std::string& a_Path, core::DataStream& a_Data)
		{
			FILE* file = nullptr;
			fopen_s(&file, a_Path.c_str(), "rb");
			if (!file)
			{
				return false;
			}

			fseek(file, 0, SEEK_END);
			size_t fileSize = ftell(file);
			rewind(file);

			a_Data = core::DataStream(fileSize);
			fread(a_Data.data(), fileSize, 1, file);

			fclose(file);

			return true;
		}

		bool FileLoader::SaveFile(const std::string& a_Path, const core::Data& a_Data)
		{
			FILE* file = nullptr;
			fopen_s(&file, a_Path.c_str(), "wb");
			if (!file)
			{
				return false;
			}

			fwrite(a_Data.data(), a_Data.size(), 1, file);

			fclose(file);

			return true;
		}

		bool FileLoader::DoesFileExist(const std::string& a_Path)
		{
			FILE* file = nullptr;
			fopen_s(&file, a_Path.c_str(), "wb");
			bool success = file != nullptr;
			fclose(file);
			return success;
		}

		bool FileLoader::CreateFolder(const std::string& a_Path)
		{
			return fs::create_directories(a_Path);
		}
		bool FileLoader::OpenInExplorer(const std::string& a_Path)
		{
			ShellExecuteA(NULL, "open", a_Path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
			return false;
		}

        std::string FileLoader::GetAppDataPath()
        {
			PWSTR path_tmp;
			std::filesystem::path path;
			SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_tmp);
			path = path_tmp;

			std::string appDataPath = std::string(path.generic_string());
			fs::create_directories(appDataPath);

			return appDataPath;
        }
	}
}