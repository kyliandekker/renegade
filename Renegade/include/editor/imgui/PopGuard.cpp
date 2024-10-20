#include "editor/imgui/PopGuard.h"

namespace ImGui
{
	PopGuard::~PopGuard()
	{
		for (size_t i = 0; i < m_Colors; i++)
		{
			PopStyleColor();
		}
		for (size_t i = 0; i < m_Styles; i++)
		{
			PopStyleVar();
		}
	}

	void PopGuard::PushStyleColor(ImGuiCol idx, ImU32 col)
	{
		ImGui::PushStyleColor(idx, col);
		m_Colors++;
	}

	void PopGuard::PushStyleColor(ImGuiCol idx, const ImVec4& col)
	{
		ImGui::PushStyleColor(idx, col);
		m_Colors++;
	}

	void PopGuard::PushStyleVar(ImGuiStyleVar idx, float val)
	{
		ImGui::PushStyleVar(idx, val);
		m_Styles++;
	}

	void PopGuard::PushStyleVar(ImGuiStyleVar idx, const ImVec2& val)
	{
		ImGui::PushStyleVar(idx, val);
		m_Styles++;
	}

	void PopGuard::PopStyleColor()
	{
		ImGui::PopStyleColor();
		m_Colors--;
	}

	void PopGuard::PopStyleVar()
	{
		ImGui::PopStyleVar();
		m_Styles--;
	}
}