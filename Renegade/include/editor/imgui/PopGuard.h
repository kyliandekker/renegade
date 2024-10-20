#pragma once

#include <imgui/imgui.h>

namespace ImGui
{
	class PopGuard
	{
	public:
		~PopGuard();

		void PushStyleColor(ImGuiCol idx, ImU32 col);       
		void PushStyleColor(ImGuiCol idx, const ImVec4& col);
		void PushStyleVar(ImGuiStyleVar idx, float val);          
		void PushStyleVar(ImGuiStyleVar idx, const ImVec2& val);
		void PopStyleColor();
		void PopStyleVar();
	private:
		int m_Styles = 0;
		int m_Colors = 0;
	};
}