#include "mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void Mesh::LoadMesh(std::string filepath)
{
	m_vertices = loadOBJFile(filepath);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Mesh::DrawMesh()
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() * 3);
}

// ------- OBJ Parsing --------

std::vector<Vertex> loadOBJFile(std::string filepath)
{
	std::ifstream file;
	file.open(filepath);
	if (!file.is_open()) {
		std::cout << "Unable to open file: " << filepath << "..." << std::endl;
		exit(1);
	}

	std::string line;
	int face_count = 0;
	std::vector<unsigned int> pos_indices, norm_indices, uv_indices;
	std::vector<glm::vec3> v_pos;
	std::vector<glm::vec3> v_norm;
	std::vector<glm::vec2> v_uv;

	while (!file.eof()) {
		std::getline(file, line);
		std::string id;
		if (line.size() != 0)
		{
			id.push_back(line[0]);
			id.push_back(line[1]);
		}
		else {
			id.push_back('$');
		}

		// Position vertex
		if (id == "v ")
		{
			glm::vec3 in_v_pos = glm::vec3(0);
			int obj_parse_result = sscanf_s(line.c_str(), "v %f %f %f\n", &in_v_pos.x, &in_v_pos.y, &in_v_pos.z);
			assert(obj_parse_result != 0);
			v_pos.push_back(in_v_pos);
		}

		else if (id == "vn")
		{
			glm::vec3 in_v_norm = glm::vec3(0);
			int obj_parse_result = sscanf_s(line.c_str(), "vn %f %f %f\n", &in_v_norm.x, &in_v_norm.y, &in_v_norm.z);
			assert(obj_parse_result != 0);
			v_norm.push_back(in_v_norm);
		}

		else if (id == "vt")
		{
			glm::vec2 in_v_uv = glm::vec3(0);
			int obj_parse_result = sscanf_s(line.c_str(), "vt %f %f\n", &in_v_uv.x, &in_v_uv.y);
			assert(obj_parse_result != 0);
			v_uv.push_back(in_v_uv);

		}

		else if (id == "f ")
		{
			face_count++;
			unsigned int face_pos[3], face_norm[3], face_uv[3];
			int obj_parse_result = sscanf_s(
				line.c_str(),
				"f %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&face_pos[0], &face_uv[0], &face_norm[0],
				&face_pos[1], &face_uv[1], &face_norm[1],
				&face_pos[2], &face_uv[2], &face_norm[2]
			);

			pos_indices.push_back(face_pos[0]);
			pos_indices.push_back(face_pos[1]);
			pos_indices.push_back(face_pos[2]);

			norm_indices.push_back(face_norm[0]);
			norm_indices.push_back(face_norm[1]);
			norm_indices.push_back(face_norm[2]);

			uv_indices.push_back(face_uv[0]);
			uv_indices.push_back(face_uv[1]);
			uv_indices.push_back(face_uv[2]);
		}
	}

	file.close();

	std::vector<Vertex> new_vertex_array;

	for (int i = 0; i < face_count * 3; i++)
	{
		unsigned int pos_idx = pos_indices[i];
		unsigned int norm_idx = norm_indices[i];
		unsigned int uv_idx = uv_indices[i];

		Vertex new_vertex;

		new_vertex.pos = v_pos[pos_idx - 1];
		new_vertex.norm = v_norm[norm_idx - 1];
		new_vertex.uv = v_uv[uv_idx - 1];

		new_vertex_array.push_back(new_vertex);
	}

	return new_vertex_array;
}