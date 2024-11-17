#pragma once

#ifdef __EDITOR__

#include <string>
#include <unordered_set>
#include <glm/vec2.hpp>

#define DEFAULT_SIZE_X 1080
#define DEFAULT_SIZE_Y 720

namespace renegade
{
	namespace editor
	{
		class EditorSettings
		{
		public:
			bool Load();
			bool Save() const;

			void SetSize(const glm::vec2& a_Size);
			const glm::vec2& Size() const;

			void SetScrollToBottom(bool a_ScrollToBottom);
			bool ScrollToBottom() const;
			void SetInfo(bool a_Info);
			bool Info() const;
			void SetWarning(bool a_Warning);
			bool Warning() const;
			void SetError(bool a_Error);
			bool Error() const;
			void SetAssert(bool a_Assert);
			bool Assert() const;
			void SetSuccess(bool a_Success);
			bool Success() const;
			void SetInfoSuccess(bool a_InfoSuccess);
			bool InfoSuccess() const;
			void SetAwesome(bool a_Awesome);
			bool Awesome() const;

			void SetPreviousProjects(const std::unordered_set<std::string>& a_PreviousProjects);
			const std::unordered_set<std::string>& GetPreviousProjects() const;
			void AddToPreviousProjects(const std::string& a_PreviousProject);
			void ErasePreviousProject(const std::string& a_PreviousProject);
		private:
			bool m_ScrollToBottom = false;
			bool m_Info = true, m_Warning = true, m_Error = true, m_Assert = true, m_Success = true, m_InfoSuccess = true, m_Awesome = true;

			glm::vec2 m_Size = glm::vec2(DEFAULT_SIZE_X, DEFAULT_SIZE_Y);
			std::unordered_set<std::string> m_PreviousProjects;
		};
	}
}

#endif // __EDITOR__