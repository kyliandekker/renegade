#pragma once

#include <string>

#include "editor/imgui/ImGuiUIView.h"

namespace renegade
{
	namespace editor
	{
		class EditorSelectable
		{
		public:
			virtual void RenderSelectable() = 0;
		};
	}
}