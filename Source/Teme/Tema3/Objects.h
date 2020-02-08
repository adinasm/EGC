#pragma once

#include <string>
#include <unordered_map>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Objects
{
	Mesh* CreateGround(float length, std::unordered_map<std::string, Mesh*>& meshes);
}

