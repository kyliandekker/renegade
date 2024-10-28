#pragma once

#ifdef __EDITOR__

#include "editor/imgui/views/ComponentUIView.h"

#include "editor/imgui/StringTextInput.h"
#include "gameplay/systems/TransformComponent.h"
#include "gameplay/systems/TransformSystem.h"

namespace renegade
{
	namespace editor
	{
		namespace imgui
		{
			class TransformComponentUIView : public ComponentUIView<gameplay::TransformComponent, gameplay::TransformSystem>
			{
			public:
				TransformComponentUIView(ImGuiWindow& a_Window, gameplay::EntityID& a_EntityID, gameplay::TransformComponent& a_TransformComponent, gameplay::TransformSystem& a_System) : ComponentUIView(a_Window, a_EntityID, a_TransformComponent, a_System) {};
			private:
				std::string GetName() const override;
				void RenderInner() override;

				StringTextInput m_PosX;
				StringTextInput m_PosY;
				StringTextInput m_PosZ;

				StringTextInput m_RotX;
				StringTextInput m_RotY;
				StringTextInput m_RotZ;

				StringTextInput m_ScaleX;
				StringTextInput m_ScaleY;
				StringTextInput m_ScaleZ;
			};
		}
	}
}

#endif // __EDITOR__