#pragma once

#ifdef __EDITOR__

#include "editor/imgui/views/ImGuiUIView.h"

#include <string>
#include <imgui/imgui.h>

#include "core/datatypes/Data.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class StringTextInput : public ImGuiUIView
			{
			public:
				StringTextInput(ImGuiWindow& a_Window);
                void Initialize(const std::string& a_InitialValue, size_t a_BufferSize = 256);

				void SetString(const std::string& a_String);

				std::string GetString();

				void Render() override { }
				bool Render(const char* a_Label, ImGuiInputTextFlags flags = 0);
			protected:
				core::Data m_Data;
			};

			class SearchBarInput : public StringTextInput
			{
			public:
				SearchBarInput(ImGuiWindow& a_Window);
				bool Render(const char* a_Label, const ImVec2& a_Size, float a_Padding);

				int m_PreviousResult = 0;
			};
		}
	}
}

#endif // __EDITOR__