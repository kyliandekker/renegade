#ifdef __EDITOR__

#include "editor/imgui/ImGuiUIView.h"

#include <imgui/imgui.h>

#include "editor/imgui/ImGuiWindow.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			ImGuiUIView::ImGuiUIView(ImGuiWindow& a_Window) : m_Window(a_Window)
			{ }

			ImGuiWindow& ImGuiUIView::Window() const
			{
				return m_Window;
			}
		}
	}
}

#endif // __EDITOR__