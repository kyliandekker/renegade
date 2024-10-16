#pragma once

#include <string>

#include "editor/imgui/ImGuiUIView.h"

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

				virtual void RenderSelectable() {};
			};
		}
	}
}