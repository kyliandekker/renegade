#pragma once

#ifdef __EDITOR__

#include "editor/imgui/views/Components/ComponentUIView.h"
#include <glm/vec3.hpp>

#include "gameplay/systems/TransformComponent.h"
#include "gameplay/systems/TransformSystem.h"
#include "editor/imgui/views/DataTypes/VectorView.h"
#include "editor/imgui/views/DataTypes/ColorView.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class TransformComponentUIView : public ComponentUIView<gameplay::TransformComponent, gameplay::TransformSystem>
			{
			public:
				TransformComponentUIView(ImGuiWindow& a_Window, gameplay::EntityID& a_EntityID, gameplay::TransformComponent& a_TransformComponent, gameplay::TransformSystem& a_System) : ComponentUIView(a_Window, a_EntityID, a_TransformComponent, a_System),
					m_PositionView(a_Window),
					m_RotationView(a_Window),
					m_ScaleView(a_Window),
					m_ColorTest(a_Window)
				{
					m_ColorTest.SetValue(glm::vec3(0, 0, 0));
				}
			private:
				std::string GetName() const override;
				void RenderInner() override;

				Vector3View<glm::vec3>
					m_PositionView,
					m_RotationView,
					m_ScaleView;
				ColorRGBAView<glm::vec3> m_ColorTest;
			};
		}
	}
}

#endif // __EDITOR__