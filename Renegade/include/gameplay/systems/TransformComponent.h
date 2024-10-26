#pragma once

#include "gameplay/systems/Component.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };
const glm::vec3 WorldRight = { 1.0f, 0.0f, 0.0f };
const glm::vec3 WorldForward = { 0.0f, 0.0f, 1.0f };

namespace renegade
{
	namespace gameplay
	{
		class TransformComponent : public Component
		{
		public:
			const glm::vec3& GetPosition() const;
			void SetPosition(const glm::vec3& a_Position);

			const glm::vec3& GetRotation() const;
			void SetRotation(const glm::vec3& a_Rotation);

			const glm::vec3& GetScale() const;
			void SetScale(const glm::vec3& a_Scale);

			glm::vec3 Forward() const;
			glm::vec3 Up() const;
			glm::vec3 Right() const;
			glm::mat4 Model() const;

			void Translate(const glm::vec3& a_Translation);
			void Rotate(const glm::vec3& a_Degrees);

			void Serialize(rapidjson::Value& a_Document, rapidjson::Document::AllocatorType& a_Allocator) const override;
			void Deserialize(const rapidjson::Value& a_Document, rapidjson::Document::AllocatorType& a_Allocator) override;
		private:
			glm::vec3 m_Position{ 0.0f };
			glm::vec3 m_Rotation{ 0.0f };
			glm::vec3 m_Scale{ 1.0f };
		};
	}
}