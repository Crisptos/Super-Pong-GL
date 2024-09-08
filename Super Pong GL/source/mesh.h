#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cassert>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"

typedef struct
{
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 uv;
} Vertex;

class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadMesh(std::string filepath);
	void DrawMesh();

private:
	// Mesh drawing data
	std::vector<Vertex> m_vertices;
	unsigned int m_VBO, m_VAO, m_EBO = 0;
};

std::vector<Vertex> loadOBJFile(std::string filepath);