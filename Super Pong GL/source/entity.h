#pragma once
#include "mesh.h"

class Entity
{
public:
	Entity() 
	{ 
		m_transform = glm::mat4(1.0f);
		position = glm::vec3(0.0f);
		rotation = glm::vec3(1.0f);
		scale = glm::vec3(1.0f);
	}

	Entity(glm::vec3 init_position) 
	{ 
		m_transform = glm::mat4(1.0f);
		position = init_position;
		rotation = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
	}

	void UpdateEntity() 
	{
		m_transform = glm::mat4(1.0f);
		// Scale -> Rotate -> Transform
		m_transform = glm::translate(m_transform, position);
		// rotate
		//m_transform = glm::scale(m_transform, scale);
	};
	void DrawEntity() { m_mesh.DrawMesh(); }

	glm::mat4 m_transform;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Mesh m_mesh;
};