#pragma once

#ifdef __EDITOR__

#include "editor/imgui/windows/BaseWindow.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class ImGuiWindow;

			class SceneWindow : public BaseWindow
			{
			public:
				SceneWindow(ImGuiWindow& a_Window);

				void Render() override;
			};
		}
	}
}

#endif // __EDITOR__