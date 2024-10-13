#ifndef IMGUI_HELPERS_H
#define IMGUI_HELPERS_H

#include <imgui/imgui.h>

#include <stdint.h>
#include <string>
#include <map>

namespace ImGui
{
    ImVec2 operator+(ImVec2 const &a, ImVec2 const &b);
    ImVec2 operator-(ImVec2 const &a, ImVec2 const &b);

    void ShowTooltipOnHover(
        char const *tooltip);

    bool DropDown(
        char const *label,
        unsigned char &value,
        char const *const names[],
        unsigned int nameCount,
        char const *tooltip);

    void TextBox(
        char const *label,
        ImVec2 const &size);

    bool CheckboxButton(const char *label, bool *p_value, const ImVec2 &size_arg = ImVec2(0, 0));
	bool CheckboxText(const char* label, bool* p_value, const std::string& checked_text, const std::string& unchecked_text, const ImVec2& size_arg = ImVec2(0, 0));
    bool InvisButton(const char *label, const ImVec2 &size_arg = ImVec2(0, 0), ImU32 button_color_hover = IM_COL32(200, 200, 200, 100));
    bool TransparentButton(const char *label, const ImVec2 &size_arg = ImVec2(0, 0), ImVec4 color = ImVec4(1, 1, 1, 1));
	bool TransparentCheckboxButton(const char* label, bool* p_value, const ImVec2& size_arg = ImVec2(0, 0), ImVec4 color = ImVec4(1, 1, 1, 1));
    bool HierarchyItem(const char *label, const ImVec2 &size_arg = ImVec2(0, 0), bool selected = false, ImU32 button_color_hover = IM_COL32(200, 200, 200, 100));
	bool EngineTreeNodeExS(const char* id, const char* icon, const char* label, bool& clicked, bool& right_clicked, bool selected, ImVec2 size, ImGuiTreeNodeFlags flags);
	bool EngineResourceNode(const char* id, const char* icon, const char* label, const char* label2, bool& clicked, bool& right_clicked, bool& double_clicked, bool selected, ImVec2 size, ImGuiTreeNodeFlags flags);
	int InvisInputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
	bool SearchBar(float fontSize, const char* label, char* buf, size_t buf_size, ImVec2 size = ImVec2(0, 0), float padding = 4, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
	void DisplayHeader(ImFont* boldFont, const char* label);

	bool RangeSliderFloat(const char *label, float *v1, float *v2, float v_min, float v_max, const char *display_format = "(%.3f, %.3f)", float power = 1.0f);

	inline ImU32 ImColorConvertFloat4ToU32(const ImVec4& in)
	{
		ImU32 out;
		out = ((int)(in.x * 255.0f)) << IM_COL32_R_SHIFT;  // Red
		out |= ((int)(in.y * 255.0f)) << IM_COL32_G_SHIFT;  // Green
		out |= ((int)(in.z * 255.0f)) << IM_COL32_B_SHIFT;  // Blue
		out |= ((int)(in.w * 255.0f)) << IM_COL32_A_SHIFT;  // Alpha
		return out;
	}

	inline ImVec4 ConvertColorsRgba(float r, float g, float b, float a)
	{
		return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

#endif // IMGUI_HELPERS_H
}