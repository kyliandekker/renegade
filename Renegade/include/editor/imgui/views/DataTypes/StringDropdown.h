#pragma once

#ifdef __EDITOR__

#include "editor/imgui/views/ImGuiUIView.h"

#include <vector>
#include <string>

#include <imgui/imgui.h>

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			template <class T>
			class StringDropdown : public ImGuiUIView
			{
			public:
				StringDropdown() = default;
				StringDropdown(ImGuiWindow& a_Window) : ImGuiUIView(a_Window)
				{ }

				void Initialize(T a_InitialValue, std::vector<T> a_Options, std::vector<std::string> a_TextValues)
				{
					m_Value = a_InitialValue;
					m_Options = a_Options;
					m_TextValues = a_TextValues;

					for (size_t i = 0; i < a_Options.size(); i++)
					{
						if (a_InitialValue == a_Options[i])
						{
							m_Selected = i;
						}
					}
				}

				void SetValue(T a_Value)
				{
					m_Value = a_Value;
				}

				T GetValue() const
				{
					return m_Value;
				}

				T m_Value;
				std::vector<T> m_Options;
				std::vector<std::string> m_TextValues;
				int m_Selected = 0;

				void Render() override { }
				bool Render(const char* a_Label)
				{
					bool b = ImGui::BeginCombo(a_Label, m_TextValues[m_Selected].c_str());
					if (b)
					{
						for (int n = 0; n < m_Options.size(); n++)
						{
							bool is_selected = (m_Selected == n);
							if (ImGui::Selectable(m_TextValues[n].c_str(), is_selected))
							{
								m_Selected = n;
								m_Value = m_Options[n];
							}

							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
					return b;
				}
			};
		}
	}
}

#endif // __EDITOR__