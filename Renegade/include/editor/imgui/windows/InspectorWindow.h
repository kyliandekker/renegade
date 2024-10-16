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

			class InspectorWindow : public BaseWindow
			{
			public:
				InspectorWindow(ImGuiWindow& a_Window);

				bool Initialize() override;
				
				void Render() override;
			};
		}
	}
}

#endif // __EDITOR__