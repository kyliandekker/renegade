#pragma once

#ifdef __EDITOR__

#include "editor/imgui/views/ImGuiUIView.h"

#include <imgui/imgui.h>
#include <string>

#include "editor/imgui/ImGuiDefines.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			// Accepts glm and imcol
			template <class T>
			class ColorRGBView : public ImGuiUIView
			{
			public:
				ColorRGBView(ImGuiWindow& a_Window) : ImGuiUIView(a_Window)
				{ }

				void SetValue(const T& a_Value)
				{
					if (m_Value != a_Value)
					{
						m_Value = a_Value;
					}
				}

				const T& GetValue() const
				{
					return m_Value;
				}

				void Render() override { }
				virtual bool Render(const char* a_Label)
				{
					return ImGui::ColorEdit3(IMGUI_FORMAT_ID("", COLOR_WHEEL_ID, std::string(a_Label) + "COLOR_WHEEL").c_str(), (float*)&this->m_Value);
				}
			protected:
				T m_Value;
			};

			template <class T>
			class ColorRGBAView : public Vector2View<T>
			{
			public:
				ColorRGBAView(ImGuiWindow& a_Window) : Vector2View<T>(a_Window)
				{ }

				void Render() override { }
				bool Render(const char* a_Label) override
				{
					return ImGui::ColorEdit4(IMGUI_FORMAT_ID("", COLOR_WHEEL_ID, std::string(a_Label) + "COLOR_WHEEL").c_str(), (float*)&this->m_Value);
				}
			};
		}
	}
}

#endif // __EDITOR__