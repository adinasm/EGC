#pragma once

#include <string>
#include <unordered_map>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Objects2D
{

	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length);
	Mesh* CreateScoreboard(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices,
		const std::vector<unsigned short>& indices, std::unordered_map<std::string, Mesh*>& meshes);

}

