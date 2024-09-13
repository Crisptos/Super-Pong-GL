#pragma once
#include "mesh.h"

class Entity
{
public:
	Entity();
	Entity(glm::vec3 init_position);

	void UpdateEntity();
	void DrawEntity();

	glm::mat4 m_transform;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec3 velocity;

	Mesh m_mesh;
};