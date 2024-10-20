#ifdef __EDITOR__

#include "editor/imgui/views/TransformComponentUIView.h"

#include <imgui/imgui_helpers.h>

#include "editor/imgui/ImGuiDefines.h"
#include "editor/imgui/ImGuiWindow.h"
#include "gameplay/systems/TransformComponent.h"

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
				const glm::vec3& position = m_TransformComponent.GetPosition();
				const glm::vec3& rotation = m_TransformComponent.GetRotation();
				const glm::vec3& scale = m_TransformComponent.GetScale();
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
				if (m_PosX.Render(IMGUI_FORMAT_ID("", INPUT_ID, "POS_X_TRANSFORM_INSPECTOR").c_str(), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll))
				{
					m_TransformComponent.SetPosition(glm::vec3(std::stof(m_PosX.GetString()), position.y, position.z));
				}
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				m_PosY.SetString(std::to_string(position.y));
				if (m_PosY.Render(IMGUI_FORMAT_ID("", INPUT_ID, "POS_Y_TRANSFORM_INSPECTOR").c_str(), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll))
				{
					m_TransformComponent.SetPosition(glm::vec3(position.x, std::stof(m_PosY.GetString()), position.z));
				}
				ImGui::SameLine();
				ImGui::Text("Z");
				ImGui::SameLine();
				m_PosZ.SetString(std::to_string(position.z));
				if (m_PosZ.Render(IMGUI_FORMAT_ID("", INPUT_ID, "POS_Z_TRANSFORM_INSPECTOR").c_str(), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll))
				{
					m_TransformComponent.SetPosition(glm::vec3(position.x, position.y, std::stof(m_PosZ.GetString())));
				}

				ImGui::Unindent();
				ImGui::DisplayHeader(m_Window.Bold(), "Rotation");
				ImGui::Indent();

				ImGui::AlignTextToFramePadding();
				ImGui::Text("X");
				ImGui::SameLine();

				m_RotX.SetString(std::to_string(rotation.x));
				if (m_RotX.Render(IMGUI_FORMAT_ID("", INPUT_ID, "ROT_X_TRANSFORM_INSPECTOR").c_str(), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll))
				{
					m_TransformComponent.SetRotation(glm::vec3(std::stof(m_RotX.GetString()), rotation.y, rotation.z));
				}
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				m_RotY.SetString(std::to_string(rotation.y));
				if (m_RotY.Render(IMGUI_FORMAT_ID("", INPUT_ID, "ROT_Y_TRANSFORM_INSPECTOR").c_str(), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll))
				{
					m_TransformComponent.SetRotation(glm::vec3(rotation.x, std::stof(m_RotY.GetString()), rotation.z));
				}
				ImGui::SameLine();
				ImGui::Text("Z");
				ImGui::SameLine();
				m_RotZ.SetString(std::to_string(rotation.z));
				if (m_RotZ.Render(IMGUI_FORMAT_ID("", INPUT_ID, "ROT_Z_TRANSFORM_INSPECTOR").c_str(), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll))
				{
					m_TransformComponent.SetRotation(glm::vec3(rotation.x, rotation.y, std::stof(m_RotZ.GetString())));
				}

				ImGui::Unindent();
				ImGui::DisplayHeader(m_Window.Bold(), "Scale");
				ImGui::Indent();

				ImGui::AlignTextToFramePadding();
				ImGui::Text("X");
				ImGui::SameLine();

				m_ScaleX.SetString(std::to_string(scale.x));
				if (m_ScaleX.Render(IMGUI_FORMAT_ID("", INPUT_ID, "SCALE_X_TRANSFORM_INSPECTOR").c_str(), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll))
				{
					m_TransformComponent.SetScale(glm::vec3(std::stof(m_ScaleX.GetString()), scale.y, scale.z));
				}
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				m_ScaleY.SetString(std::to_string(scale.y));
				if (m_ScaleY.Render(IMGUI_FORMAT_ID("", INPUT_ID, "SCALE_Y_TRANSFORM_INSPECTOR").c_str(), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll))
				{
					m_TransformComponent.SetScale(glm::vec3(scale.x, std::stof(m_ScaleY.GetString()), scale.z));
				}
				ImGui::SameLine();
				ImGui::Text("Z");
				ImGui::SameLine();
				m_ScaleZ.SetString(std::to_string(scale.z));
				if (m_ScaleZ.Render(IMGUI_FORMAT_ID("", INPUT_ID, "SCALE_Z_TRANSFORM_INSPECTOR").c_str(), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll))
				{
					m_TransformComponent.SetScale(glm::vec3(scale.x, scale.y, std::stof(m_ScaleZ.GetString())));
				}

				ImGui::PopItemWidth();
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
				ImGui::Unindent();
			}
		}
	}
}

#endif // __EDITOR__