#ifdef __EDITOR__

#include "editor/imgui/views/DataTypes/StringTextInput.h"

#include <imgui/imgui.h>
#include <imgui/imgui_helpers.h>
#include "core/Engine.h"
#include <editor/imgui/ImGuiDefines.h>

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			StringTextInput::StringTextInput(ImGuiWindow& a_Window) : ImGuiUIView(a_Window)
			{ }

			void StringTextInput::Initialize(const std::string& a_InitialValue, size_t a_BufferSize)
			{
				m_Data = core::Data(a_BufferSize);
				SetString(a_InitialValue);
			}

			void StringTextInput::SetString(const std::string& a_String)
			{
				if (a_String.size() == 0)
				{
					return;
				}

				m_Data.Clear();
				strncpy_s(reinterpret_cast<char*>(m_Data.data()), m_Data.size(), a_String.c_str(), a_String.size());
			}

			std::string StringTextInput::GetString()
			{
				return std::string(reinterpret_cast<char*>(m_Data.data()));
			}

			bool StringTextInput::Render(const char* a_Label, ImGuiInputTextFlags flags)
			{
				bool success = ImGui::InputText(a_Label, reinterpret_cast<char*>(m_Data.data()), m_Data.size(), flags);

				if (ImGui::IsItemHovered())
				{
					// Set the cursor to a text input
					ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
				}
				return success;
			}

			SearchBarInput::SearchBarInput(ImGuiWindow& a_Window) : StringTextInput(a_Window)
			{ }

			bool SearchBarInput::Render(const char* a_Label, const ImVec2& a_Size, float a_Padding)
			{
				return ImGui::SearchBar(core::ENGINE.GetEditor().GetImGuiWindow().FontSize(), a_Label, reinterpret_cast<char*>(m_Data.data()), m_Data.size(), a_Size, a_Padding);
			}
		}
	}
}

#endif // __EDITOR__