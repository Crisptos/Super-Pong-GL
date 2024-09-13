#include "entity.h"

Entity::Entity()
{
	m_transform = glm::mat4(1.0f);
	position = glm::vec3(0.0f);
	rotation = glm::vec3(1.0f);
	scale = glm::vec3(1.0f);
	velocity = glm::vec3(0.0f);
}

Entity::Entity(glm::vec3 init_position)
{
	m_transform = glm::mat4(1.0f);
	position = init_position;
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	velocity = glm::vec3(0.0f);
}

void Entity::UpdateEntity()
{
	m_transform = glm::mat4(1.0f);
	// Scale -> Rotate -> Transform
	m_transform = glm::translate(m_transform, position);

	// Rotation for each axis
	m_transform = glm::rotate(m_transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_transform = glm::rotate(m_transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_transform = glm::rotate(m_transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	m_transform = glm::scale(m_transform, scale);
}

void Entity::DrawEntity() 
{ 
	m_mesh.DrawMesh(); 
}
