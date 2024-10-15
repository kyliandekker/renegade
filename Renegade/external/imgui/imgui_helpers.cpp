#include "imgui/imgui_helpers.h"

#ifdef __EDITOR__

#include <cmath>
#include <cstdio>
#include <string>
#include <imgui/imgui_internal.h>
#include <editor/imgui/ImguiDefines.h>
#include <algorithm>
#include <editor/imgui/ImGuiWindow.h>

namespace ImGui
{
    ImVec2 operator+(const ImVec2& a, const ImVec2& b)
    {
        return ImVec2(a.x + b.x, a.y + b.y);
    }

    ImVec2 operator-(const ImVec2& a, const ImVec2& b)
    {
        return ImVec2(a.x - b.x, a.y - b.y);
    }

	bool CheckboxButton(const char* label, bool* p_value, const ImVec2& size_arg)
    {
        ImVec4 color = *p_value ? ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive) : ImVec4(0, 0, 0, 0);
		ImGui::PushStyleColor(ImGuiCol_Button, color);
        bool b = ImGui::Button(label, size_arg);
        if (b)
            *p_value = !(*p_value);
        ImGui::PopStyleColor();

        return b;
    }

	bool TransparentButton(const char* label, const ImVec2& size_arg, ImVec4 color)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));        // Set button color to white
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));  // Disable active effect
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));  // Disable active effect

		ImGui::PushStyleColor(ImGuiCol_Text, color);
		bool b = ImGui::Button(label, size_arg);
		ImGui::PopStyleColor();

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		return b;
	}

	bool TransparentCheckboxButton(const char* label, bool* p_value, const ImVec2& size_arg, ImVec4 color)
	{
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));  // Disable active effect

		ImGui::PushStyleColor(ImGuiCol_Text, color);
		bool b = ImGui::CheckboxButton(label, p_value, size_arg);
		ImGui::PopStyleColor();

		ImGui::PopStyleColor();

		return b;
	}

	//bool HierarchyItem(const char* label, const ImVec2& size_arg, bool selected, ImU32 button_color_hover)
	//{
	//	// Get the current draw list (this is where you'll draw your custom button)
	//	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	//	// Define the position and size of your button
	//	ImVec2 button_pos = ImGui::GetCursorScreenPos();

	//	// Calculate the rectangle for the button
	//	ImVec2 button_min = button_pos;
	//	ImVec2 button_max = ImVec2(button_pos.x + size_arg.x, button_pos.y + size_arg.y);

	//	// Detect if the mouse is hovering over the button rectangle
	//	bool is_hovered = ImGui::IsMouseHoveringRect(button_min, button_max);

	//	// Set button color based on hover or click state
	//	ImU32 button_color = is_hovered ? button_color_hover : IM_COL32(150, 150, 150, 0);

	//	if (selected)
	//	{
	//		ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_Button);
	//		color.w = 0.35f;
	//		button_color = ImGui::ImColorConvertFloat4ToU32(color);
	//	}

	//	// Draw the button
	//	draw_list->AddRectFilled(button_min, button_max, button_color, 0);  // 10.0f is the rounding radius

	//	return InvisibleButton(label, size_arg);
	//}

	bool EngineTreeNodeExS(const char* id, const char* icon, const char* label, bool& clicked, bool& right_clicked, bool selected, const ImVec2& size, ImGuiTreeNodeFlags flags)
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		ImVec2 pos = ImGui::GetCursorScreenPos();

		const bool fold = ImGui::TreeNodeExS(id, size, flags);

		clicked = ImGui::IsItemHovered() && ImGui::IsItemClicked(0);
		right_clicked = ImGui::IsItemHovered() && ImGui::IsItemClicked(1);

		ImVec2 itemSize = ImGui::GetItemRectSize();

		ImVec2 select_min = pos;
		ImVec2 select_max = ImVec2(pos.x + itemSize.x, pos.y + itemSize.y);

		if (selected)
		{
			ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_Button);
			ImU32 select_color = ImGui::ColorConvertFloat4ToU32(color);

			draw_list->AddRectFilled(select_min, select_max, select_color, 0);
		}

		// Adjust icon and label positions after the tree node is drawn
		float iconPos = pos.x + 35;
		ImGui::SetCursorScreenPos(ImVec2(iconPos, pos.y));  // Set icon position using screen coordinates
		ImGui::Text(icon);
		ImGui::SetCursorScreenPos(ImVec2(iconPos + 25, pos.y));  // Set label position
		ImGui::Text(label);

		// Return whether the node is open/closed
		return fold;
	}

	bool EngineResourceNode(const char* id, const char* icon, const char* label, const char* label2, bool& clicked, bool& right_clicked, bool& double_clicked, bool selected)
	{
		ImVec2 pos = ImGui::GetCursorScreenPos();

		clicked = ImGui::Selectable(id, &selected);
		right_clicked = ImGui::IsItemHovered() && ImGui::IsItemClicked(1);
		double_clicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);

		ImGui::SetCursorScreenPos(pos);
		ImGui::SetCursorScreenPos(ImVec2(pos.x + 10, pos.y));
		ImGui::Text(icon);
		ImGui::SetCursorScreenPos(ImVec2(pos.x + 35, pos.y));
		ImGui::Text(label);

		ImGui::SetCursorScreenPos(ImVec2(pos.x + 300, pos.y));
		ImVec4 textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
		textColor.w = 0.5f;
		ImGui::TextColored(textColor, label2);

		return clicked;
	}

    bool SearchBar(float fontSize, const char* label, char* buf, size_t buf_size, const ImVec2& size, float padding, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
		float x = ImGui::GetCursorPosX();
		float y = ImGui::GetCursorPosY();

		float cursorX = ImGui::GetCursorScreenPos().x;
		float cursorY = ImGui::GetCursorScreenPos().y;

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 16);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(fontSize * 2.0f, padding));
		ImGui::SetNextItemWidth(size.x);
		bool result = ImGui::InputText(label, buf, buf_size, flags, callback, user_data);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		ImGui::SetCursorPosX(x + (fontSize / 2));
		ImGui::SetCursorPosY(y + (padding * 2));
		ImGui::Text(ICON_FA_SEARCH);

		//ImDrawList* draw_list = ImGui::GetWindowDrawList();

		ImVec2 header_size = ImVec2(fontSize, size.y - (padding * 3));

		ImVec2 button_min = ImVec2((cursorX + size.x) - (fontSize + padding), cursorY + padding);
		ImVec2 button_max = ImVec2(button_min.x + header_size.x, button_min.y + header_size.y);

		//draw_list->AddRectFilled(button_min, button_max, ImGui::ImColorConvertFloat4ToU32(ImVec4(1, 0, 0, 1)), 0);

		ImVec2 mousePos = ImGui::GetMousePos();
		bool is_button_hovered = (mousePos.x >= button_min.x && mousePos.x <= button_max.x &&
			mousePos.y >= button_min.y && mousePos.y <= button_max.y);

		bool is_button_clicked = ImGui::IsMouseClicked(0) && is_button_hovered;

		if (is_button_clicked)
		{
			ImGui::ClearActiveID();
			buf[0] = '\0';
			ImGui::SetKeyboardFocusHere(-1);
			result = true;
		}

		ImGui::SetCursorPosX((x + size.x) - (fontSize + padding));
		ImGui::SetCursorPosY(y + (padding * 2));
		ImVec4 text_hovered = ImGui::GetStyleColorVec4(ImGuiCol_Button);
		text_hovered.z = 0.5f;
		ImGui::TextColored(is_button_hovered ? text_hovered : ImGui::GetStyleColorVec4(ImGuiCol_Text),ICON_FA_CROSS);

		ImGui::SetCursorPosX((x + size.x) - (fontSize + padding));
		ImGui::SetCursorPosY(y);
		if (ImGui::InvisibleButton("###ERASE_BUTTON", ImVec2(fontSize, size.y)))
		{ }

        return result;
    }
}

#endif // __EDITOR__