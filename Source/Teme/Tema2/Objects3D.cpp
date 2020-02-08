#include "Objects3D.h"

#include <Core/Engine.h>

// Creates a cylinder mesh with the given height and radius.
Mesh* Objects3D::CreateCylinder(float height, float radius, std::unordered_map<std::string, Mesh*>& meshes) {
	int triangleCount = 100;
	int pointCount = 101;
	float deg = 2 * M_PI;
	float angle, nextAngle;

	glm::vec3 color = glm::vec3(0.308f, 0.375f, 0.461f);

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	for (int i = 0; i < triangleCount; ++i)
	{
		angle = ((float)i) * 2.0f * M_PI / triangleCount;
		nextAngle = ((float)i + 1) * 2.0f * M_PI / triangleCount;

		vertices.push_back(VertexFormat(glm::vec3(0.0f, height, 0.0f), color));
		indices.push_back(vertices.size() - 1);
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(angle), height, radius * sin(angle)), color));
		indices.push_back(vertices.size() - 1);
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(nextAngle), height, radius * sin(nextAngle)), color));
		indices.push_back(vertices.size() - 1);

		// Build slice.
		for (int j = pointCount - 1; j >= -pointCount + 1; --j)
		{
			float currentY = height * j / pointCount;

			if ((((j & 1) || (j == pointCount - 1)) && (i & 1)) || (!(j & 1) && !(i & 1)))
			{
				vertices.push_back(VertexFormat(glm::vec3(radius * cos(angle), currentY, radius * sin(angle)), color));
				indices.push_back(vertices.size() - 3);
				indices.push_back(vertices.size() - 1);
				indices.push_back(vertices.size() - 2);
			}
			else
			{
				vertices.push_back(VertexFormat(glm::vec3(radius * cos(nextAngle), currentY, radius * sin(nextAngle)),
					color));
				indices.push_back(vertices.size() - 3);
				indices.push_back(vertices.size() - 2);
				indices.push_back(vertices.size() - 1);
			}
		}

		indices.push_back(vertices.size() - 2);
		indices.push_back(vertices.size() - 1);
		indices.push_back(vertices.size());

		indices.push_back(vertices.size() - 1);
		indices.push_back(vertices.size() + 1);
		indices.push_back(vertices.size());

		vertices.push_back(VertexFormat(glm::vec3(radius * cos(nextAngle), -height, radius * sin(nextAngle)), color));
		indices.push_back(vertices.size() - 1);
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(angle), -height, radius * sin(angle)), color));
		indices.push_back(vertices.size() - 1);
		vertices.push_back(VertexFormat(glm::vec3(0.0f, -height, 0.0f), color));
		indices.push_back(vertices.size() - 1);
	}

	Mesh* cylinder = CreateMesh("cylinder", vertices, indices, meshes);

	return cylinder;
}

// Creates a square mesh with given name and length.
Mesh* Objects3D::CreateSquare(std::string name, float length, std::unordered_map<std::string, Mesh*>& meshes)
{
	glm::vec3 corner = glm::vec3(0.f, 0.f, 0.f);

	std::vector<glm::vec3> vertices =
	{
		corner,
		corner + glm::vec3(length, 0, 0),
		corner + glm::vec3(length, length, 0),
		corner + glm::vec3(0, length, 0)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };


	std::vector<glm::vec3> normals
	{
		glm::vec3(0, 1, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0)
	};

	// Texture coordinates for the square.
	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(1.f, 1.f),
		glm::vec2(0.f, 1.f)
	};

	Mesh* square = new Mesh(name);
	square->InitFromData(vertices, normals, textureCoords, indices);
	meshes[name] = square;

	return square;
}

// Creates a pyramid trunk mesh.
Mesh* Objects3D::CreateTrunk(std::unordered_map<std::string, Mesh*>& meshes)
{
	glm::vec3 color = glm::vec3(0.48627f, 0.48627f, 0.48627f);

		std::vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-0.15f, -0.f,  0.15f), color),
			VertexFormat(glm::vec3(0.5f, -0.5f,  0.5f), color),
			VertexFormat(glm::vec3(-0.15f, 0.5f,  0.5f), color),
			VertexFormat(glm::vec3(0.5f, 0.5f, 0.5f), color),
			VertexFormat(glm::vec3(-0.15f, -0.f,  -0.15f), color),
			VertexFormat(glm::vec3(0.5f, -0.5f,  -0.5f), color),
			VertexFormat(glm::vec3(-0.15f, 0.5f,  -0.5f), color),
			VertexFormat(glm::vec3(0.5f, 0.5f,  -0.5f), color),
		};

		std::vector<unsigned short> indices =
		{
			0, 1, 2,
			1, 3, 2,
			2, 3, 7,
			2, 7, 6,
			1, 7, 3,
			1, 5, 7,
			6, 7, 4,
			7, 5, 4,
			0, 4, 1,
			1, 4, 5,
			2, 6, 4,
			0, 2, 4,
		};

		Mesh* trunk = CreateMesh("trunk", vertices, indices, meshes);
	
		return trunk;
}

// Creates a mesh for the plane wing.
Mesh* Objects3D::CreateWing(std::unordered_map<std::string, Mesh*>& meshes)
{
	glm::vec3 color = glm::vec3(0.48627f, 0.48627f, 0.48627f);

	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0.f, -0.5f,  0.5f), color),
		VertexFormat(glm::vec3(0.f, 0.5f,  0.5f), color),
		VertexFormat(glm::vec3(-0.5f, -0.5f,  -0.5f), color),
		VertexFormat(glm::vec3(0.5f, -0.5f,  -0.5f), color),
		VertexFormat(glm::vec3(-0.5f, 0.5f,  -0.5f), color),
		VertexFormat(glm::vec3(0.5f, 0.5f,  -0.5f), color),
	};

	std::vector<unsigned short> indices =
	{
		0, 0, 1,
		0, 1, 1,
		1, 1, 5,
		1, 5, 4,
		0, 5, 1,
		0, 3, 5,
		4, 5, 2,
		5, 3, 2,
		0, 2, 0,
		0, 2, 3,
		1, 4, 2,
		0, 1, 2,
	};

	Mesh* wing = CreateMesh("wing", vertices, indices, meshes);

	return wing;
}

// Creates a mesh.
Mesh* Objects3D::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices,
	const std::vector<unsigned short>& indices, std::unordered_map<std::string, Mesh*>& meshes)
{
	unsigned int VAO = 0;
	// Creates the VAO and binds it.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Creates the VBO and binds it.
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Sends vertices data into the VBO buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// Creates the IBO and binds it.
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// Sends indices data into the IBO buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data.

	// Sets vertex position attribute.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// Sets vertex normal attribute.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// Sets texture coordinate attribute.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// Sets vertex color attribute.
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
		(void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbinds the VAO.
	glBindVertexArray(0);

	// Checks for OpenGL errors.
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object.
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}