#ifdef __EDITOR__

#include "editor/imgui/views/ComponentUIView.h"

#include "editor/imgui/ImGuiDefines.h"
#include "core/Engine.h"
#include "utils/string_extensions.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			void ComponentUIView::Render()
			{
				ImGui::SetNextItemOpen(m_FoldedOut);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
				m_FoldedOut = ImGui::CollapsingHeader(IMGUI_FORMAT_ID(ICON_FA_SCENE + std::string(" ") + GetName(), FOLDOUT_ID, string_extensions::StringToUpper(GetName()) + "_INSPECTOR").c_str());
				if (m_FoldedOut)
				{
					ImGui::Indent();
					RenderInner();
					ImGui::Unindent();
				}
				ImGui::PopStyleVar();
			}
		}
	}
}

#endif // __EDITOR__