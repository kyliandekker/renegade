#pragma once

#ifdef __EDITOR__

#include "editor/imgui/windows/BaseWindow.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class LoadProjectWindow : public BaseWindow
			{
			public:
				LoadProjectWindow(ImGuiWindow& a_Window);
				void Render() override;
			};
		}
	}
}

#endif // __EDITOR__