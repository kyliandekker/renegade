#include "gameplay/systems/TransformComponent.h"

#include <rapidjson/utils.h>

#define JSON_TRANSFORM_COMPONENT_VAR "transformComponent"
#define JSON_TRANSFORM_COMPONENT_POSITION_VAR "position"
#define JSON_TRANSFORM_COMPONENT_ROTATION_VAR "rotation"
#define JSON_TRANSFORM_COMPONENT_SCALE_VAR "scale"
#define JSON_TRANSFORM_COMPONENT_X_VAR "x"
#define JSON_TRANSFORM_COMPONENT_Y_VAR "y"
#define JSON_TRANSFORM_COMPONENT_Z_VAR "z"

namespace renegade
{
	namespace gameplay
	{
		const glm::vec3& TransformComponent::GetPosition() const
		{
			return m_Position;
		}

		void TransformComponent::SetPosition(const glm::vec3& a_Position)
		{
			m_Position = a_Position;
		}

		const glm::vec3& TransformComponent::GetRotation() const
		{
			return m_Rotation;
		}

		void TransformComponent::SetRotation(const glm::vec3& a_Rotation)
		{
			m_Rotation = a_Rotation;
		}

        const glm::vec3& TransformComponent::GetScale() const
        {
			return m_Scale;
        }

		void TransformComponent::SetScale(const glm::vec3& a_Scale)
		{
			m_Scale = a_Scale;
		}

		glm::vec3 TransformComponent::Forward() const
		{
			glm::vec3 direction{ 1.0f };
			const glm::vec3& angles = m_Rotation;
			direction.x = cos(glm::radians(angles.y)) * cos(glm::radians(angles.x));
			direction.y = sin(glm::radians(angles.x));
			direction.z = sin(glm::radians(angles.y)) * cos(glm::radians(angles.x));
			return direction;
		}

		glm::vec3 TransformComponent::Up() const 
		{ 
			return glm::cross(Right(), Forward());
		}

		glm::vec3 TransformComponent::Right() const 
		{ 
			return glm::normalize(glm::cross(Forward(), WorldUp)); 
		}

		glm::mat4 TransformComponent::Model() const
		{
			glm::mat4 model = glm::identity<glm::mat4>();
			model = glm::translate(model, m_Position);

			model = glm::rotate(model, glm::radians(m_Rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(m_Rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(m_Rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::scale(model, m_Scale);

			return model;
		}

		void TransformComponent::Translate(const glm::vec3& a_Translation)
		{
			m_Position += a_Translation;
		}

		void TransformComponent::Rotate(const glm::vec3& a_Degrees)
		{
			m_Rotation += a_Degrees;
		}

        std::string TransformComponent::GetPropertyName() const
        {
            return JSON_TRANSFORM_COMPONENT_VAR;
        }

		void TransformComponent::Serialize(rapidjson::Document& a_Document) const
		{
			if (!a_Document.IsObject())
			{
				return;
			}

			rapidjson::Document::AllocatorType& allocator = a_Document.GetAllocator();

			a_Document.AddMember(JSON_TRANSFORM_COMPONENT_POSITION_VAR, rapidjson::Value().SetObject(), allocator);

			a_Document[JSON_TRANSFORM_COMPONENT_POSITION_VAR].AddMember(JSON_TRANSFORM_COMPONENT_X_VAR, m_Position.x, allocator);
			a_Document[JSON_TRANSFORM_COMPONENT_POSITION_VAR].AddMember(JSON_TRANSFORM_COMPONENT_Y_VAR, m_Position.y, allocator);
			a_Document[JSON_TRANSFORM_COMPONENT_POSITION_VAR].AddMember(JSON_TRANSFORM_COMPONENT_Z_VAR, m_Position.z, allocator);

			a_Document.AddMember(JSON_TRANSFORM_COMPONENT_ROTATION_VAR, rapidjson::Value().SetObject(), allocator);

			a_Document[JSON_TRANSFORM_COMPONENT_ROTATION_VAR].AddMember(JSON_TRANSFORM_COMPONENT_X_VAR, m_Rotation.x, allocator);
			a_Document[JSON_TRANSFORM_COMPONENT_ROTATION_VAR].AddMember(JSON_TRANSFORM_COMPONENT_Y_VAR, m_Rotation.y, allocator);
			a_Document[JSON_TRANSFORM_COMPONENT_ROTATION_VAR].AddMember(JSON_TRANSFORM_COMPONENT_Z_VAR, m_Rotation.z, allocator);

			a_Document.AddMember(JSON_TRANSFORM_COMPONENT_SCALE_VAR, rapidjson::Value().SetObject(), allocator);

			a_Document[JSON_TRANSFORM_COMPONENT_SCALE_VAR].AddMember(JSON_TRANSFORM_COMPONENT_X_VAR, m_Scale.x, allocator);
			a_Document[JSON_TRANSFORM_COMPONENT_SCALE_VAR].AddMember(JSON_TRANSFORM_COMPONENT_Y_VAR, m_Scale.y, allocator);
			a_Document[JSON_TRANSFORM_COMPONENT_SCALE_VAR].AddMember(JSON_TRANSFORM_COMPONENT_Z_VAR, m_Scale.z, allocator);
		}

		void TransformComponent::Deserialize(const rapidjson::Document& a_Document)
		{
			if (!a_Document.IsObject())
			{
				return;
			}

			if (a_Document.HasMember(JSON_TRANSFORM_COMPONENT_POSITION_VAR) && a_Document[JSON_TRANSFORM_COMPONENT_POSITION_VAR].IsObject())
			{
				rapidjson::GetFloat(a_Document[JSON_TRANSFORM_COMPONENT_POSITION_VAR], JSON_TRANSFORM_COMPONENT_X_VAR, m_Position.x);
				rapidjson::GetFloat(a_Document[JSON_TRANSFORM_COMPONENT_POSITION_VAR], JSON_TRANSFORM_COMPONENT_Y_VAR, m_Position.y);
				rapidjson::GetFloat(a_Document[JSON_TRANSFORM_COMPONENT_POSITION_VAR], JSON_TRANSFORM_COMPONENT_Z_VAR, m_Position.z);
			}

			if (a_Document.HasMember(JSON_TRANSFORM_COMPONENT_ROTATION_VAR) && a_Document[JSON_TRANSFORM_COMPONENT_ROTATION_VAR].IsObject())
			{
				rapidjson::GetFloat(a_Document[JSON_TRANSFORM_COMPONENT_ROTATION_VAR], JSON_TRANSFORM_COMPONENT_X_VAR, m_Rotation.x);
				rapidjson::GetFloat(a_Document[JSON_TRANSFORM_COMPONENT_ROTATION_VAR], JSON_TRANSFORM_COMPONENT_Y_VAR, m_Rotation.y);
				rapidjson::GetFloat(a_Document[JSON_TRANSFORM_COMPONENT_ROTATION_VAR], JSON_TRANSFORM_COMPONENT_Z_VAR, m_Rotation.z);
			}

			if (a_Document.HasMember(JSON_TRANSFORM_COMPONENT_SCALE_VAR) && a_Document[JSON_TRANSFORM_COMPONENT_SCALE_VAR].IsObject())
			{
				rapidjson::GetFloat(a_Document[JSON_TRANSFORM_COMPONENT_SCALE_VAR], JSON_TRANSFORM_COMPONENT_X_VAR, m_Scale.x);
				rapidjson::GetFloat(a_Document[JSON_TRANSFORM_COMPONENT_SCALE_VAR], JSON_TRANSFORM_COMPONENT_Y_VAR, m_Scale.y);
				rapidjson::GetFloat(a_Document[JSON_TRANSFORM_COMPONENT_SCALE_VAR], JSON_TRANSFORM_COMPONENT_Z_VAR, m_Scale.z);
			}
		}
	}
}