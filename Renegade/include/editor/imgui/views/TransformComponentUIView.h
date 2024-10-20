#pragma once

#ifdef __EDITOR__

#include "editor/imgui/views/ComponentUIView.h"

#include "editor/imgui/StringTextInput.h"

namespace renegade
{
	namespace gameplay
	{
		class TransformComponent;
	}
	namespace editor
	{
		namespace imgui
		{
			class TransformComponentUIView : public ComponentUIView
			{
			public:
				TransformComponentUIView(ImGuiWindow& a_Window, gameplay::TransformComponent& a_TransformComponent) : ComponentUIView(a_Window), m_TransformComponent(a_TransformComponent) {};
			private:
				std::string GetName() const override;
				void RenderInner() override;

				gameplay::TransformComponent& m_TransformComponent;

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