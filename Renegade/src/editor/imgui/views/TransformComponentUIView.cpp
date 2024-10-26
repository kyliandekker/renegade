#ifdef __EDITOR__

#include "editor/imgui/views/TransformComponentUIView.h"

#include <imgui/imgui_helpers.h>

#include "editor/imgui/ImGuiDefines.h"
#include "editor/imgui/ImGuiWindow.h"
#include "gameplay/systems/TransformComponent.h"
#include "gameplay/systems/TransformSystem.h"
#include "core/Engine.h"
#include "editor/explorer_resources/SceneExplorerResource.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
            std::string TransformComponentUIView::GetName() const
            {
				return "Transform";
            }

            void TransformComponentUIView::RenderInner()
			{
				glm::vec3 position = m_TransformComponent.GetPosition();
				glm::vec3 rotation = m_TransformComponent.GetRotation();
				glm::vec3 scale = m_TransformComponent.GetScale();
				float fontSize = m_Window.FontSize();

				ImGui::DisplayHeader(m_Window.Bold(), "Position");
				ImGui::Indent();
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(fontSize / 2, fontSize / 2));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
				ImGui::PushItemWidth(75);

				ImGui::AlignTextToFramePadding();
				ImGui::Text("X");
				ImGui::SameLine();
				m_PosX.SetString(std::to_string(position.x));
				bool changedPos = ImGui::DragFloat(IMGUI_FORMAT_ID("", INPUT_ID, "POS_X_TRANSFORM_INSPECTOR").c_str(), &position.x);
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				changedPos |= ImGui::DragFloat(IMGUI_FORMAT_ID("", INPUT_ID, "POS_Y_TRANSFORM_INSPECTOR").c_str(), &position.y);
				ImGui::SameLine();
				ImGui::Text("Z");
				ImGui::SameLine();
				changedPos |= ImGui::DragFloat(IMGUI_FORMAT_ID("", INPUT_ID, "POS_Z_TRANSFORM_INSPECTOR").c_str(), &position.z);
				if (changedPos)
				{
					m_TransformComponent.SetPosition(position);
					core::ENGINE.GetEditor().SetDirty();
				}

				ImGui::Unindent();
				ImGui::DisplayHeader(m_Window.Bold(), "Rotation");
				ImGui::Indent();

				ImGui::AlignTextToFramePadding();
				ImGui::Text("X");
				ImGui::SameLine();
				bool changedRotation = ImGui::DragFloat(IMGUI_FORMAT_ID("", INPUT_ID, "ROT_X_TRANSFORM_INSPECTOR").c_str(), &rotation.x);
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				changedRotation |= ImGui::DragFloat(IMGUI_FORMAT_ID("", INPUT_ID, "ROT_Y_TRANSFORM_INSPECTOR").c_str(), &rotation.y);
				ImGui::SameLine();
				ImGui::Text("Z");
				ImGui::SameLine();
				changedRotation |= ImGui::DragFloat(IMGUI_FORMAT_ID("", INPUT_ID, "ROT_Z_TRANSFORM_INSPECTOR").c_str(), &rotation.z);
				if (changedRotation)
				{
					m_TransformComponent.SetRotation(rotation);
					core::ENGINE.GetEditor().SetDirty();
				}

				ImGui::Unindent();
				ImGui::DisplayHeader(m_Window.Bold(), "Scale");
				ImGui::Indent();

				ImGui::AlignTextToFramePadding();
				ImGui::Text("X");
				ImGui::SameLine();
				bool changedScale = ImGui::DragFloat(IMGUI_FORMAT_ID("", INPUT_ID, "SCALE_X_TRANSFORM_INSPECTOR").c_str(), &scale.x);
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				changedScale |= ImGui::DragFloat(IMGUI_FORMAT_ID("", INPUT_ID, "SCALE_Y_TRANSFORM_INSPECTOR").c_str(), &scale.y);
				ImGui::SameLine();
				ImGui::Text("Z");
				ImGui::SameLine();
				changedScale |= ImGui::DragFloat(IMGUI_FORMAT_ID("", INPUT_ID, "SCALE_Z_TRANSFORM_INSPECTOR").c_str(), &scale.z);
				if (changedScale)
				{
					m_TransformComponent.SetScale(scale);
					core::ENGINE.GetEditor().SetDirty();
				}

				ImGui::PopItemWidth();
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
				ImGui::Unindent();
			}

			void TransformComponentUIView::DeleteComponent()
			{
				core::ENGINE.GetECS().GetSystem<gameplay::TransformSystem>().DeleteComponent(m_EntityID);
				ComponentUIView::DeleteComponent();
			}
		}
	}
}

#endif // __EDITOR__