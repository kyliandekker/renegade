#pragma once

#ifdef __EDITOR__

#include <string>

#include "editor/imgui/views/ImGuiUIView.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class ImGuiWindow;

			class ComponentUIView : public ImGuiUIView
			{
			public:
				ComponentUIView(ImGuiWindow& a_Window) : ImGuiUIView(a_Window) {};
				void Render() override;
			protected:
				virtual void RenderInner() = 0;
				virtual std::string GetName() const = 0;
				bool m_FoldedOut = true;
			};
		}
	}
}

#endif // __EDITOR__