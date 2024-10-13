#pragma once

#ifdef __EDITOR__

#include <string>
#include "core/datatypes/Data.h"
#include "imgui/imgui.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class StringTextInput
			{
			public:
				StringTextInput(const std::string& a_InitialValue = "", size_t a_BufferSize = 256);

				void SetString(const std::string& a_String);

				std::string GetString();

				virtual bool Render(const char* a_Label);
			protected:
				core::Data m_Data;
			};

			class SearchBarInput : public StringTextInput
			{
			public:
				SearchBarInput(const std::string& a_InitialValue = "", size_t a_BufferSize = 256);

				bool Render(const char* a_Label, const ImVec2& a_Size, float a_Padding);
				bool Render(const char* a_Label) override;

				int m_PreviousResult = 0;
			};
		}
	}
}

#endif // __EDITOR__