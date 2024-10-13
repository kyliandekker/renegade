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
    ImVec2 operator+(ImVec2 const& a, ImVec2 const& b)
    {
        return ImVec2(a.x + b.x, a.y + b.y);
    }

    ImVec2 operator-(ImVec2 const& a, ImVec2 const& b)
    {
        return ImVec2(a.x - b.x, a.y - b.y);
    }

    void ShowTooltipOnHover(
        char const* tooltip)
    {
        if (IsItemHovered())
        {
            BeginTooltip();
            Text("%s", tooltip);
            EndTooltip();
        }
    }

    bool DropDown(
        char const* label,
        unsigned char& value,
        char const* const names[],
        unsigned int nameCount,
        char const* tooltip)
    {
        bool value_changed = false;

        auto current_effect_item = names[value];
        if (BeginCombo(label, current_effect_item, ImGuiComboFlags_HeightLarge))
        {
            for (unsigned char n = 0; n < nameCount; n++)
            {
                bool is_selected = (current_effect_item == names[n]);
                if (Selectable(names[n], is_selected))
                {
                    current_effect_item = names[n];
                    value = n;
                    value_changed = true;
                }
            }

            EndCombo();
        }

        ShowTooltipOnHover(tooltip == nullptr ? label : tooltip);

        return value_changed;
    }

    void TextBox(char const* label, ImVec2 const& size)
    {
        ImDrawList* draw_list = GetWindowDrawList();

        ImVec2 pos = GetCursorScreenPos();

        draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), pos, ImColor(255, 255, 0, 255), "Hello World", 0, 0.0f, 0);
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

	bool CheckboxText(const char* label, bool* p_value, const std::string& checked_text, const std::string& unchecked_text, const ImVec2& size_arg)
	{
		ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_Button);
		ImVec4 color_inactive = ImVec4(0, 0, 0, 0);
		bool b = InvisButton(label, size_arg, IM_COL32(0, 0, 0, 0));
		std::string test = (*p_value ? checked_text : unchecked_text);
		bool temp = false;
		ImGui::Text(test.c_str());
		if (b)
		{
			*p_value = !(*p_value);
		}

		return *p_value;
	}

	bool InvisButton(const char* label, const ImVec2& size_arg, ImU32 button_color_hover)
	{
		// Get the current draw list (this is where you'll draw your custom button)
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		// Define the position and size of your button
		ImVec2 button_pos = ImGui::GetCursorScreenPos();
		ImVec2 button_size = ImVec2(size_arg.x, size_arg.y); // Width, Height

		// Calculate the rectangle for the button
		ImVec2 button_min = button_pos;
		ImVec2 button_max = ImVec2(button_pos.x + button_size.x, button_pos.y + button_size.y);

		// Detect if the mouse is hovering over the button rectangle
		bool is_hovered = ImGui::IsMouseHoveringRect(button_min, button_max);

		// Detect if the mouse is clicked
		bool is_clicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left) && is_hovered;

		// Set button color based on hover or click state
		ImU32 button_color = is_hovered ? button_color_hover : IM_COL32(150, 150, 150, 0);

		// Draw the button
		draw_list->AddRectFilled(button_min, button_max, button_color, 0);  // 10.0f is the rounding radius

		return is_clicked;
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

	bool HierarchyItem(const char* label, const ImVec2& size_arg, bool selected, ImU32 button_color_hover)
	{
		// Get the current draw list (this is where you'll draw your custom button)
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		// Define the position and size of your button
		ImVec2 button_pos = ImGui::GetCursorScreenPos();

		// Calculate the rectangle for the button
		ImVec2 button_min = button_pos;
		ImVec2 button_max = ImVec2(button_pos.x + size_arg.x, button_pos.y + size_arg.y);

		// Detect if the mouse is hovering over the button rectangle
		bool is_hovered = ImGui::IsMouseHoveringRect(button_min, button_max);

		// Set button color based on hover or click state
		ImU32 button_color = is_hovered ? button_color_hover : IM_COL32(150, 150, 150, 0);

		if (selected)
		{
			ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_Button);
			color.w = 0.35f;
			button_color = ImGui::ImColorConvertFloat4ToU32(color);
		}

		// Draw the button
		draw_list->AddRectFilled(button_min, button_max, button_color, 0);  // 10.0f is the rounding radius

		return InvisibleButton(label, size_arg);
	}

	bool EngineTreeNodeExS(const char* id, const char* icon, const char* label, bool& clicked, bool& right_clicked, bool selected, ImVec2 size, ImGuiTreeNodeFlags flags)
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

	bool EngineResourceNode(const char* id, const char* icon, const char* label, const char* label2, bool& clicked, bool& right_clicked, bool& double_clicked, bool selected, ImVec2 size, ImGuiTreeNodeFlags flags)
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

	int InvisInputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
		bool b = InputText(label, buf, buf_size, flags, callback, user_data);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		if ((flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0)
		{
			if (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
			{
				return 2;
			}
		}

		return b;
	}

    bool SearchBar(float fontSize, const char* label, char* buf, size_t buf_size, ImVec2 size, float padding, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
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

	void DisplayHeader(ImFont* boldFont, const char* label)
	{
		ImGui::PushFont(boldFont);
		ImGui::Text(label);
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::Text(":");
	}

    float RoundScalarWithFormatFloat(const char* format, ImGuiDataType data_type, float v)
    {
        //return RoundScalarWithFormatT<float, float>(format, data_type, v);
        return 0.0f;
    }

    float SliderCalcRatioFromValueFloat(ImGuiDataType data_type, float v, float v_min, float v_max, float power, float linear_zero_pos)
    {
        return ScaleRatioFromValueT<float, float, float>(data_type, v, v_min, v_max, false, power, linear_zero_pos);
    }

    // ~80% common code with ImGui::SliderBehavior
    bool RangeSliderBehavior(const ImRect& frame_bb, ImGuiID id, float* v1, float* v2, float v_min, float v_max, float power, int decimal_precision, ImGuiSliderFlags flags)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();
        const ImGuiStyle& style = g.Style;

        // Draw frame
        RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_Button), true, style.FrameRounding);

        const bool is_non_linear = (power < 1.0f - 0.00001f) || (power > 1.0f + 0.00001f);
        const bool is_horizontal = (flags & ImGuiSliderFlags_Vertical) == 0;

        const float grab_padding = 2.0f;
        const float slider_sz = is_horizontal ? (frame_bb.GetWidth() - grab_padding * 2.0f) : (frame_bb.GetHeight() - grab_padding * 2.0f);
        float grab_sz;
        if (decimal_precision > 0)
            grab_sz = ImMin(style.GrabMinSize, slider_sz);
        else
            grab_sz = ImMin(ImMax(1.0f * (slider_sz / ((v_min < v_max ? v_max - v_min : v_min - v_max) + 1.0f)), style.GrabMinSize), slider_sz); // Integer sliders, if possible have the grab size represent 1 unit
        const float slider_usable_sz = slider_sz - grab_sz;
        const float slider_usable_pos_min = (is_horizontal ? frame_bb.Min.x : frame_bb.Min.y) + grab_padding + grab_sz * 0.5f;
        const float slider_usable_pos_max = (is_horizontal ? frame_bb.Max.x : frame_bb.Max.y) - grab_padding - grab_sz * 0.5f;

        // For logarithmic sliders that cross over sign boundary we want the exponential increase to be symmetric around 0.0f
        float linear_zero_pos = 0.0f; // 0.0->1.0f
        if (v_min * v_max < 0.0f)
        {
            // Different sign
            const float linear_dist_min_to_0 = powf(fabsf(0.0f - v_min), 1.0f / power);
            const float linear_dist_max_to_0 = powf(fabsf(v_max - 0.0f), 1.0f / power);
            linear_zero_pos = linear_dist_min_to_0 / (linear_dist_min_to_0 + linear_dist_max_to_0);
        }
        else
        {
            // Same sign
            linear_zero_pos = v_min < 0.0f ? 1.0f : 0.0f;
        }

        // Process clicking on the slider
        bool value_changed = false;
        if (g.ActiveId == id)
        {
            if (g.IO.MouseDown[0])
            {
                const float mouse_abs_pos = is_horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
                float clicked_t = (slider_usable_sz > 0.0f) ? ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f) : 0.0f;
                if (!is_horizontal)
                    clicked_t = 1.0f - clicked_t;

                float new_value;
                if (is_non_linear)
                {
                    // Account for logarithmic scale on both sides of the zero
                    if (clicked_t < linear_zero_pos)
                    {
                        // Negative: rescale to the negative range before powering
                        float a = 1.0f - (clicked_t / linear_zero_pos);
                        a = powf(a, power);
                        new_value = ImLerp(ImMin(v_max, 0.0f), v_min, a);
                    }
                    else
                    {
                        // Positive: rescale to the positive range before powering
                        float a;
                        if (fabsf(linear_zero_pos - 1.0f) > 1.e-6f)
                            a = (clicked_t - linear_zero_pos) / (1.0f - linear_zero_pos);
                        else
                            a = clicked_t;
                        a = powf(a, power);
                        new_value = ImLerp(ImMax(v_min, 0.0f), v_max, a);
                    }
                }
                else
                {
                    // Linear slider
                    new_value = ImLerp(v_min, v_max, clicked_t);
                }

                char fmt[64];
                snprintf(fmt, 64, "%%.%df", decimal_precision);

                // Round past decimal precision
                new_value = RoundScalarWithFormatFloat(fmt, ImGuiDataType_Float, new_value);
                if (*v1 != new_value || *v2 != new_value)
                {
                    if (fabsf(*v1 - new_value) < fabsf(*v2 - new_value))
                    {
                        *v1 = new_value;
                    }
                    else
                    {
                        *v2 = new_value;
                    }
                    value_changed = true;
                }
            }
            else
            {
                ClearActiveID();
            }
        }

        // Calculate slider grab positioning
        float grab_t = SliderCalcRatioFromValueFloat(ImGuiDataType_Float, *v1, v_min, v_max, power, linear_zero_pos);

        // Draw
        if (!is_horizontal)
            grab_t = 1.0f - grab_t;
        float grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
        ImRect grab_bb1;
        if (is_horizontal)
            grab_bb1 = ImRect(ImVec2(grab_pos - grab_sz * 0.5f, frame_bb.Min.y + grab_padding), ImVec2(grab_pos + grab_sz * 0.5f, frame_bb.Max.y - grab_padding));
        else
            grab_bb1 = ImRect(ImVec2(frame_bb.Min.x + grab_padding, grab_pos - grab_sz * 0.5f), ImVec2(frame_bb.Max.x - grab_padding, grab_pos + grab_sz * 0.5f));
        window->DrawList->AddRectFilled(grab_bb1.Min, grab_bb1.Max, GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding);

        // Calculate slider grab positioning
        grab_t = SliderCalcRatioFromValueFloat(ImGuiDataType_Float, *v2, v_min, v_max, power, linear_zero_pos);

        // Draw
        if (!is_horizontal)
            grab_t = 1.0f - grab_t;
        grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
        ImRect grab_bb2;
        if (is_horizontal)
            grab_bb2 = ImRect(ImVec2(grab_pos - grab_sz * 0.5f, frame_bb.Min.y + grab_padding), ImVec2(grab_pos + grab_sz * 0.5f, frame_bb.Max.y - grab_padding));
        else
            grab_bb2 = ImRect(ImVec2(frame_bb.Min.x + grab_padding, grab_pos - grab_sz * 0.5f), ImVec2(frame_bb.Max.x - grab_padding, grab_pos + grab_sz * 0.5f));
        window->DrawList->AddRectFilled(grab_bb2.Min, grab_bb2.Max, GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding);

        ImRect connector(grab_bb1.Min, grab_bb2.Max);
        connector.Min.x += grab_sz;
        connector.Min.y += grab_sz * 0.3f;
        connector.Max.x -= grab_sz;
        connector.Max.y -= grab_sz * 0.3f;

        window->DrawList->AddRectFilled(connector.Min, connector.Max, GetColorU32(ImGuiCol_SliderGrab), style.GrabRounding);

        return value_changed;
    }

    // ~95% common code with ImGui::SliderFloat
    bool RangeSliderFloat(const char* label, float* v1, float* v2, float v_min, float v_max, const char* display_format, float power)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const float w = CalcItemWidth();

        const ImVec2 label_size = CalcTextSize(label, NULL, true);
        const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
        const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

        // NB- we don't call ItemSize() yet because we may turn into a text edit box below
        if (!ItemAdd(total_bb, id))
        {
            ItemSize(total_bb, style.FramePadding.y);
            return false;
        }

        const bool hovered = ItemHoverable(frame_bb, id, 0);
        if (hovered)
            SetHoveredID(id);

        if (!display_format)
            display_format = "(%.3f, %.3f)";
        int decimal_precision = ImParseFormatPrecision(display_format, 3);

        // Tabbing or CTRL-clicking on Slider turns it into an input box
        bool start_text_input = false;
        const bool tab_focus_requested = (GetItemStatusFlags() /*& ImGuiItemStatusFlags_FocusedByTabbing*/) != 0;
        if (tab_focus_requested || (hovered && g.IO.MouseClicked[0]))
        {
            SetActiveID(id, window);
            FocusWindow(window);

            if (tab_focus_requested || g.IO.KeyCtrl)
            {
                start_text_input = true;
                g.TempInputId = 0;
            }
        }

        if (start_text_input || (g.ActiveId == id && g.TempInputId == id))
        {
            char fmt[64];
            snprintf(fmt, 64, "%%.%df", decimal_precision);
            return TempInputScalar(frame_bb, id, label, ImGuiDataType_Float, v1, fmt);
        }

        ItemSize(total_bb, style.FramePadding.y);

        // Actual slider behavior + render grab
        const bool value_changed = RangeSliderBehavior(frame_bb, id, v1, v2, v_min, v_max, power, decimal_precision, 0);

        // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
        char value_buf[64];
        const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v1, *v2);
        RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

        if (label_size.x > 0.0f)
            RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

        return value_changed;
    }
}

#endif // __EDITOR__