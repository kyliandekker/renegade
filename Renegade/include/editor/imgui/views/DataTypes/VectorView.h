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
			// Accepts glm and imvec
			template <class T>
			class Vector2View : public ImGuiUIView
			{
			public:
				Vector2View(ImGuiWindow& a_Window) : ImGuiUIView(a_Window)
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
					ImGui::AlignTextToFramePadding();
					ImGui::Text("X");
					ImGui::SameLine();
					bool changedValue = ImGui::DragFloat(IMGUI_FORMAT_ID("", INPUT_ID, std::string("X_") + a_Label).c_str(), &m_Value.x);
					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					changedValue |= ImGui::DragFloat(IMGUI_FORMAT_ID("", INPUT_ID, std::string("Y_") + a_Label).c_str(), &m_Value.y);

					return changedValue;
				}
			protected:
				T m_Value;
			};

			template <class T>
			class Vector3View : public Vector2View<T>
			{
			public:
				Vector3View(ImGuiWindow& a_Window) : Vector2View<T>(a_Window)
				{ }

				void Render() override { }
				bool Render(const char* a_Label) override
				{
					bool changedValue = Vector2View<T>::Render(a_Label);
					ImGui::SameLine();
					ImGui::Text("Z");
					ImGui::SameLine();
					changedValue |= ImGui::DragFloat(IMGUI_FORMAT_ID("", INPUT_ID, std::string("Z_") + a_Label).c_str(), &this->m_Value.z);

					return changedValue;
				}
			};
		}
	}
}

#endif // __EDITOR__