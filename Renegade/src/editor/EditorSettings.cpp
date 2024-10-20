#include "editor/EditorSettings.h"

// # Rapidjson
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/utils.h>
#include <string>

#include "core/datatypes/DataStream.h"
#include "file/FileLoader.h"
#include "editor/Editor.h"

#define SETTINGS_PATH "settings.json"

#define JSON_CONSOLE_VAR "console"
#define JSON_CONSOLE_SCROLLTOBOTTOM_VAR "scrollToBottom"
#define JSON_CONSOLE_INFO_VAR "info"
#define JSON_CONSOLE_WARNING_VAR "warning"
#define JSON_CONSOLE_ERROR_VAR "error"
#define JSON_CONSOLE_ASSERT_VAR "assert"
#define JSON_CONSOLE_SUCCESS_VAR "success"
#define JSON_CONSOLE_INFOSUCCESS_VAR "infosuccess"
#define JSON_CONSOLE_AWESOME_VAR "awesome"

#define JSON_WINDOW_SIZE_VAR "windowSize"
#define JSON_WINDOW_SIZE_X_VAR "x"
#define JSON_WINDOW_SIZE_Y_VAR "y"
#define JSON_PREVIOUS_PROJECTS_VAR "previousProjects"

namespace renegade
{
	namespace editor
	{
		bool EditorSettings::Load()
		{
			std::string path = std::string(file::FileLoader::GetAppDataPath() + SETTINGS_FOLDER + std::string(SETTINGS_PATH));

			core::DataStream data;
			if (!file::FileLoader::LoadFile(path, data))
			{
				return false;
			}

			m_Size = glm::vec2(DEFAULT_SIZE_X, DEFAULT_SIZE_Y);

			rapidjson::Document document;
			document.Parse(reinterpret_cast<char*>(data.data()), data.size());

			if (document.HasParseError())
			{
				Save();
				return false;
			}

			if (document.HasMember(JSON_CONSOLE_VAR) && document[JSON_CONSOLE_VAR].IsObject())
			{
				// Scroll to Bottom
				GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_SCROLLTOBOTTOM_VAR, m_ScrollToBottom);

				GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_INFO_VAR, m_Info);
				GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_WARNING_VAR, m_Warning);
				GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_ERROR_VAR, m_Error);
				GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_ASSERT_VAR, m_Assert);
				GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_SUCCESS_VAR, m_Success);
				GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_INFOSUCCESS_VAR, m_InfoSuccess);
				GetBool(document[JSON_CONSOLE_VAR], JSON_CONSOLE_AWESOME_VAR, m_Awesome);
			}

			// Window size
			{
				glm::vec2 temp;
				GetFloat(document[JSON_WINDOW_SIZE_VAR], JSON_WINDOW_SIZE_X_VAR, temp.x);
				GetFloat(document[JSON_WINDOW_SIZE_VAR], JSON_WINDOW_SIZE_Y_VAR, temp.y);

				if (temp.x != 0)
				{
					m_Size.x = temp.x;
				}
				if (temp.y != 0)
				{
					m_Size.y = temp.y;
				}
			}

			return true;
		}

		bool EditorSettings::Save() const
		{
			rapidjson::Document document;
			document.SetObject();
			rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

			document.AddMember(JSON_CONSOLE_VAR, rapidjson::Value().SetObject(), allocator);
			document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_SCROLLTOBOTTOM_VAR, m_ScrollToBottom, allocator);
			document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_INFO_VAR, m_Info, allocator);
			document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_WARNING_VAR, m_Warning, allocator);
			document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_ERROR_VAR, m_Error, allocator);
			document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_ASSERT_VAR, m_Assert, allocator);
			document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_SUCCESS_VAR, m_Success, allocator);
			document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_INFOSUCCESS_VAR, m_InfoSuccess, allocator);
			document[JSON_CONSOLE_VAR].AddMember(JSON_CONSOLE_AWESOME_VAR, m_Awesome, allocator);

			document.AddMember(JSON_WINDOW_SIZE_VAR, rapidjson::Value().SetObject(), allocator);
			document[JSON_WINDOW_SIZE_VAR].AddMember(JSON_WINDOW_SIZE_X_VAR, m_Size.x, allocator);
			document[JSON_WINDOW_SIZE_VAR].AddMember(JSON_WINDOW_SIZE_Y_VAR, m_Size.y, allocator);

			rapidjson::StringBuffer buffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
			document.Accept(writer);

			std::string path = std::string(file::FileLoader::GetAppDataPath() + SETTINGS_FOLDER + std::string(SETTINGS_PATH));

			return file::FileLoader::SaveFile(path, core::Data(buffer.GetString(), buffer.GetSize()));

			return true;
		}

		void EditorSettings::SetSize(const glm::vec2& a_Size)
		{
			m_Size = a_Size;

			Save();
		}

		const glm::vec2& EditorSettings::Size() const
		{
			return m_Size;
		}

		void EditorSettings::SetScrollToBottom(bool a_ScrollToBottom)
		{
			m_ScrollToBottom = a_ScrollToBottom;

			Save();
		}

		bool EditorSettings::ScrollToBottom() const
		{
			return m_ScrollToBottom;
		}

		void EditorSettings::SetInfo(bool a_Info)
		{
			m_Info = a_Info;

			Save();
		}

		bool EditorSettings::Info() const
		{
			return m_Info;
		}

		void EditorSettings::SetWarning(bool a_Warning)
		{
			m_Warning = a_Warning;

			Save();
		}

		bool EditorSettings::Warning() const
		{
			return m_Warning;
		}

		void EditorSettings::SetError(bool a_Error)
		{
			m_Error = a_Error;

			Save();
		}

		bool EditorSettings::Error() const
		{
			return m_Error;
		}

		void EditorSettings::SetAssert(bool a_Assert)
		{
			m_Assert = a_Assert;

			Save();
		}

		bool EditorSettings::Assert() const
		{
			return m_Assert;
		}

		void EditorSettings::SetSuccess(bool a_Success)
		{
			m_Success = a_Success;

			Save();
		}

		bool EditorSettings::Success() const
		{
			return m_Success;
		}

		void EditorSettings::SetInfoSuccess(bool a_InfoSuccess)
		{
			m_InfoSuccess = a_InfoSuccess;

			Save();
		}

		bool EditorSettings::InfoSuccess() const
		{
			return m_InfoSuccess;
		}

		void EditorSettings::SetAwesome(bool a_Awesome)
		{
			m_Awesome = a_Awesome;

			Save();
		}

		bool EditorSettings::Awesome() const
		{
			return m_Awesome;
		}
	}
}