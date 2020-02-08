#include "Objects.h"

#include <Core/Engine.h>

// Creates the mesh for the ground using the given length.
Mesh* Objects::CreateGround(float length, std::unordered_map<std::string, Mesh*>& meshes)
{
	glm::vec3 corner = glm::vec3(0.f, 0.f, 0.f);
	std::vector<glm::vec3> vertices;
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;

	int vertexCount = 256;
	float chunkLength = length / (vertexCount - 1);
	float texChunkLength = 1.f / (vertexCount - 1);

	for (int i = 0; i < vertexCount; ++i)
	{
		for (int j = 0; j < vertexCount; ++j)
		{
			vertices.push_back(corner + glm::vec3(chunkLength * i, 0, chunkLength * j));
			textureCoords.push_back(glm::vec2(texChunkLength * i, texChunkLength * j));

			if (i && j)
			{
				indices.push_back((i - 1) * vertexCount + (j - 1));
				indices.push_back(i * vertexCount + (j - 1));
				indices.push_back(i * vertexCount + j);

				indices.push_back((i - 1) * vertexCount + (j - 1));
				indices.push_back(i * vertexCount + j);
				indices.push_back((i - 1) * vertexCount + j);

				normals.push_back(glm::vec3(0, 1, 1));
				normals.push_back(glm::vec3(1, 0, 1));
				normals.push_back(glm::vec3(1, 0, 0));
				normals.push_back(glm::vec3(0, 1, 0));
			}
		}
	}

	Mesh* ground = new Mesh("ground");
	ground->InitFromData(vertices, normals, textureCoords, indices);
	meshes["ground"] = ground;

	return ground;
}
