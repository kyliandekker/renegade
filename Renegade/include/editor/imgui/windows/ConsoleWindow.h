#pragma once

#ifdef __EDITOR__

#include "editor/imgui/windows/BaseWindow.h"

#include <vector>
#include <mutex>

#include "logger/Logger.h"
#include "editor/imgui/StringTextInput.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			inline std::mutex MESSAGE_MUTEX;

			class ImGuiWindow;

			class ConsoleWindow : public BaseWindow
			{
			public:
				ConsoleWindow(ImGuiWindow& a_Window);
				~ConsoleWindow();
				void Render() override;

				void Clear();
				void AddMessage(const logger::Message& a_Message);

				void LoggerCallback(const logger::Message& a_Message);
			private:
				bool m_NeedsRefresh = true;
				std::vector<logger::Message> m_Messages;
				std::vector<size_t> m_FilteredMessages;

				SearchBarInput m_SearchBar;
			};
		}
	}
}

#endif // __EDITOR__