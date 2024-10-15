#pragma once

#include <string>

#include "editor/imgui/ImGuiUIView.h"

namespace renegade
{
	namespace editor
	{
		class EditorSelectable : public imgui::ImGuiUIView
		{
		public:
			std::string m_Name;
			std::string m_Icon;

			virtual void Render() override;
		};
	}
}