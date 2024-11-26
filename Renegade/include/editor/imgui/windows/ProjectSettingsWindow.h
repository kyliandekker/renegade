#pragma once

#ifdef __EDITOR__

#include "editor/imgui/windows/BaseWindow.h"

#include "project/ProjectSettings.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class ImGuiWindow;

			class ProjectSettingsWindow : public BaseWindow
			{
			public:
				ProjectSettingsWindow(ImGuiWindow& a_Window);

				void Render() override;
			private:
			};
		}
	}
}

#endif // __EDITOR__