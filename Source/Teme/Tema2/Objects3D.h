#pragma once

#include <string>
#include <unordered_map>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Objects3D
{
	Mesh* CreateCylinder(float height, float radius, std::unordered_map<std::string, Mesh*>& meshes);
	Mesh* CreateSquare(std::string name, float length, std::unordered_map<std::string, Mesh*>& meshes);
	Mesh* CreateTrunk(std::unordered_map<std::string, Mesh*>& meshes);
	Mesh* CreateWing(std::unordered_map<std::string, Mesh*>& meshes);
	Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices,
		const std::vector<unsigned short>& indices, std::unordered_map<std::string, Mesh*>& meshes);
}

