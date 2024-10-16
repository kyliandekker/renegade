#pragma once

#ifdef __EDITOR__

#include <imgui/imgui.h>

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class ImGuiWindow;

			class ImGuiUIView
			{
			public:
				ImGuiUIView(ImGuiWindow& a_Window);
				virtual void Render() {};

				ImGuiWindow& Window() const;
			protected:
				ImGuiWindow& m_Window;
			};
		}
	}
}

#endif // __EDITOR__