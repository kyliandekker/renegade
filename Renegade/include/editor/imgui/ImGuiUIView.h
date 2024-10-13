#pragma once

#ifdef __EDITOR__

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
				virtual void Render() = 0;

				ImGuiWindow& Window() const;
			protected:
				ImGuiWindow& m_Window;
			};
		}
	}
}

#endif // __EDITOR__