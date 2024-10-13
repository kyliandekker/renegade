#pragma once

#ifdef __EDITOR__

#include <string>
#include <imgui/imgui.h>

#include "editor/imgui/ImGuiUIView.h"

typedef int ImGuiWindowFlags;

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class ImGuiWindow;

			class BaseWindow : public ImGuiUIView
			{
			public:
				BaseWindow(ImGuiWindow& a_Window, ImGuiWindowFlags a_Flags, const std::string& a_Name, std::string a_WindowID, bool a_FullScreen = false);
				~BaseWindow();

				virtual bool WindowBegin();
				virtual void WindowEnd();
				void Update();

				void SetSize(ImVec2 a_Size);
				bool IsFullScreen() const;

				void SetEnabled(bool a_Enabled);
				void ToggleEnabled();
				std::string GetName() const;
				bool IsEnabled() const;

				void BeginToolbar(const ImVec2& a_Size);
				void EndToolbar();
			protected:
				ImGuiWindowFlags m_Flags = 0;
				ImVec2 m_Size = ImVec2(0, 0);
				std::string m_Name, m_WindowID;
				bool m_FullScreen = false;
				bool m_Enabled = true;
				bool m_RenderWindowItself = false;
			};
		}
	}
}

#endif // __EDITOR__