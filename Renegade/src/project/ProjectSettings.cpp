#include "project/ProjectSettings.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/utils.h>
#include <algorithm>

#include "core/datatypes/DataStream.h"
#include "file/FileLoader.h"

#define PROJECT_SETTINGS_PATH "project-settings.json"

#define JSON_LOCALIZATION_VAR "localization"
#define JSON_LANGUAGES_VAR "languages"

namespace renegade
{
	namespace project
	{
		bool ProjectSettings::Load(const std::string& a_Path)
		{
			std::string path = std::string(a_Path + "/" + PROJECT_SETTINGS_PATH);

			core::DataStream data;
			if (!file::FileLoader::LoadFile(path, data))
			{
				Save(a_Path);
				return false;
			}

			rapidjson::Document document;
			document.Parse(reinterpret_cast<char*>(data.data()), data.size());

			if (document.HasMember(JSON_LOCALIZATION_VAR) && document[JSON_LANGUAGES_VAR].IsObject())
			{
				if (document[JSON_LOCALIZATION_VAR].HasMember(JSON_LANGUAGES_VAR) && document[JSON_LOCALIZATION_VAR][JSON_LANGUAGES_VAR].IsArray())
				{
					for (auto& element : document[JSON_LOCALIZATION_VAR][JSON_LANGUAGES_VAR].GetArray())
					{
						if (!element.IsString())
						{
							continue;
						}
						if (std::find(m_Languages.begin(), m_Languages.end(), element.GetInt()) == m_Languages.end())
						{
							m_Languages.push_back(element.GetInt());
						}
					}
				}
			}
		}
		
		bool ProjectSettings::Save(const std::string& a_Path) const
		{
			rapidjson::Document document;
			document.SetObject();
			rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

			document.AddMember(JSON_LOCALIZATION_VAR, rapidjson::Value().SetObject(), allocator);
			document[JSON_LOCALIZATION_VAR].AddMember(JSON_LANGUAGES_VAR, rapidjson::Value().SetArray(), allocator);
			for (auto& previousProject : m_Languages)
			{
				document[JSON_LOCALIZATION_VAR][JSON_LOCALIZATION_VAR].PushBack(previousProject, allocator);
			}

			rapidjson::StringBuffer buffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
			document.Accept(writer);

			std::string path = std::string(a_Path + "/" + PROJECT_SETTINGS_PATH);

			return file::FileLoader::SaveFile(path, core::Data(buffer.GetString(), buffer.GetSize()));
		}

		void ProjectSettings::AddLanguage(int a_Language)
		{
			if (std::find(m_Languages.begin(), m_Languages.end(), a_Language) == m_Languages.end())
			{
				m_Languages.push_back(a_Language);
			}
		}

		void ProjectSettings::RemoveLanguage(int a_Language)
		{
			m_Languages.erase(std::find(m_Languages.begin(), m_Languages.end(), a_Language));
		}
	}
}