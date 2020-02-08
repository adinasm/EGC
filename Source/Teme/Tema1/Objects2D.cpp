#include "Objects2D.h"

#include <Core/Engine.h>

// Creates square with given bottom left corner, length and color.
Mesh* Objects2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<glm::vec3> vertices =
	{
		corner,
		corner + glm::vec3(length, 0, 0),
		corner + glm::vec3(length, length, 0),
		corner + glm::vec3(0, length, 0)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };


	std::vector<glm::vec3> normals
	{
		glm::vec3(0, 1, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0)
	};

	// Texture coordinates for the square
	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0.f, 0.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(1.f, 1.f),
		glm::vec2(0.f, 1.f)
	};

	square->InitFromData(vertices, normals, textureCoords, indices);

	return square;
}

Mesh* Objects2D::CreateScoreboard(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;
	glm::vec3 black = glm::vec3(0.f, 0.f, 0.f);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, black),
		VertexFormat(corner + glm::vec3(length, 0, 0),black),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), black)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	square->InitFromData(vertices, indices);

	return square;
}

Mesh* Objects2D::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices,
	const std::vector<unsigned short>& indices, std::unordered_map<std::string, Mesh*>& meshes)
{
	unsigned int VAO = 0;
	// Creates the VAO and binds it.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Creates the VBO and binds it.
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Sends vertices data into the VBO buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// Creates the IBO and binds it.
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// Sends indices data into the IBO buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

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

	// Unbinds the VAO.
	glBindVertexArray(0);

	// Checks for OpenGL errors.
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object.
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}
