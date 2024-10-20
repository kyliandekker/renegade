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

			class EditorSelectable : public ImGuiUIView
			{
			public:
				EditorSelectable(ImGuiWindow& a_Window);

				virtual void RenderSelectable() { };
				virtual void Select() {};
				virtual void Deselect() {};
			};
		}
	}
}

#endif // __EDITOR__