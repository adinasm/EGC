#include "Digits.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Objects2D.h"

Digits::Digits()
{

}

Digits::~Digits()
{
}

// Creates a mesh for every digit.
void Digits::initializeDigits(std::unordered_map<std::string, Mesh*>& meshes)
{
	glm::vec3 color = glm::vec3(0.607843f, 0.607843f, 0.607843f);

	VertexFormat v0 = VertexFormat(glm::vec3(0.f, 0.f, 1.f), color); // 0
	VertexFormat v1 = VertexFormat(glm::vec3(1.f, 0.f, 1.f), color); // 1
	VertexFormat v2 = VertexFormat(glm::vec3(3.f, 0.f, 1.f), color); // 2
	VertexFormat v3 = VertexFormat(glm::vec3(4.f, 0.f, 1.f), color); // 3
	VertexFormat v4 = VertexFormat(glm::vec3(0.f, 1.f, 1.f), color); // 4
	VertexFormat v5 = VertexFormat(glm::vec3(4.f, 1.f, 1.f), color); // 5
	VertexFormat v6 = VertexFormat(glm::vec3(0.f, 5.f, 1.f), color); // 6
	VertexFormat v7 = VertexFormat(glm::vec3(1.f, 5.f, 1.f), color); // 7
	VertexFormat v8 = VertexFormat(glm::vec3(3.f, 5.f, 1.f), color); // 8
	VertexFormat v9 = VertexFormat(glm::vec3(4.f, 5.f, 1.f), color); // 9
	VertexFormat v10 = VertexFormat(glm::vec3(0.f, 6.f, 1.f), color); // 10
	VertexFormat v11 = VertexFormat(glm::vec3(1.f, 6.f, 1.f), color); // 11
	VertexFormat v12 = VertexFormat(glm::vec3(3.f, 6.f, 1.f), color); // 12
	VertexFormat v13 = VertexFormat(glm::vec3(4.f, 6.f, 1.f), color); // 13
	VertexFormat v14 = VertexFormat(glm::vec3(0.f, 10.f, 1.f), color); // 14
	VertexFormat v15 = VertexFormat(glm::vec3(4.f, 10.f, 1.f), color); // 15
	VertexFormat v16 = VertexFormat(glm::vec3(0.f, 11.f, 1.f), color); // 16
	VertexFormat v17 = VertexFormat(glm::vec3(1.f, 11.f, 1.f), color); // 17
	VertexFormat v18 = VertexFormat(glm::vec3(3.f, 11.f, 1.f), color); // 18
	VertexFormat v19 = VertexFormat(glm::vec3(4.f, 11.f, 1.f), color); // 19

	std::vector<unsigned short> A = { 14, 19, 16, 14, 15, 19 };
	std::vector<unsigned short> B = { 8, 9, 18, 9, 19, 18 };
	std::vector<unsigned short> C = { 2, 3, 12, 3, 13, 12 };
	std::vector<unsigned short> D = { 0, 5, 4, 0, 3, 5 };
	std::vector<unsigned short> E = { 0, 1, 10, 1, 11, 10 };
	std::vector<unsigned short> F = { 6, 7 ,16, 7, 17, 16 };
	std::vector<unsigned short> G = { 6, 13, 10, 6, 9, 13 };

	std::vector<VertexFormat> vertices
	{
		v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19
	};


	{
		std::vector<unsigned short> indices;
		indices.insert(indices.end(), A.begin(), A.end());
		indices.insert(indices.end(), B.begin(), B.end());
		indices.insert(indices.end(), C.begin(), C.end());
		indices.insert(indices.end(), D.begin(), D.end());
		indices.insert(indices.end(), E.begin(), E.end());
		indices.insert(indices.end(), F.begin(), F.end());

		Objects2D::CreateMesh("0", vertices, indices, meshes);
	}

	{
		std::vector<unsigned short> indices;
		indices.insert(indices.end(), B.begin(), B.end());
		indices.insert(indices.end(), C.begin(), C.end());

		Objects2D::CreateMesh("1", vertices, indices, meshes);
	}

	{
		std::vector<unsigned short> indices;
		indices.insert(indices.end(), A.begin(), A.end());
		indices.insert(indices.end(), B.begin(), B.end());
		indices.insert(indices.end(), D.begin(), D.end());
		indices.insert(indices.end(), E.begin(), E.end());
		indices.insert(indices.end(), G.begin(), G.end());

		Objects2D::CreateMesh("2", vertices, indices, meshes);
	}

	{
		std::vector<unsigned short> indices;
		indices.insert(indices.end(), A.begin(), A.end());
		indices.insert(indices.end(), B.begin(), B.end());
		indices.insert(indices.end(), C.begin(), C.end());
		indices.insert(indices.end(), D.begin(), D.end());
		indices.insert(indices.end(), G.begin(), G.end());

		Objects2D::CreateMesh("3", vertices, indices, meshes);
	}

	{
		std::vector<unsigned short> indices;
		indices.insert(indices.end(), B.begin(), B.end());
		indices.insert(indices.end(), C.begin(), C.end());
		indices.insert(indices.end(), F.begin(), F.end());
		indices.insert(indices.end(), G.begin(), G.end());

		Objects2D::CreateMesh("4", vertices, indices, meshes);
	}

	{
		std::vector<unsigned short> indices;
		indices.insert(indices.end(), A.begin(), A.end());
		indices.insert(indices.end(), C.begin(), C.end());
		indices.insert(indices.end(), D.begin(), D.end());
		indices.insert(indices.end(), F.begin(), F.end());
		indices.insert(indices.end(), G.begin(), G.end());

		Objects2D::CreateMesh("5", vertices, indices, meshes);
	}

	{
		std::vector<unsigned short> indices;
		indices.insert(indices.end(), A.begin(), A.end());
		indices.insert(indices.end(), C.begin(), C.end());
		indices.insert(indices.end(), D.begin(), D.end());
		indices.insert(indices.end(), E.begin(), E.end());
		indices.insert(indices.end(), F.begin(), F.end());
		indices.insert(indices.end(), G.begin(), G.end());

		Objects2D::CreateMesh("6", vertices, indices, meshes);
	}

	{
		std::vector<unsigned short> indices;
		indices.insert(indices.end(), A.begin(), A.end());
		indices.insert(indices.end(), B.begin(), B.end());
		indices.insert(indices.end(), C.begin(), C.end());

		Objects2D::CreateMesh("7", vertices, indices, meshes);
	}

	{
		std::vector<unsigned short> indices;
		indices.insert(indices.end(), A.begin(), A.end());
		indices.insert(indices.end(), B.begin(), B.end());
		indices.insert(indices.end(), C.begin(), C.end());
		indices.insert(indices.end(), D.begin(), D.end());
		indices.insert(indices.end(), E.begin(), E.end());
		indices.insert(indices.end(), F.begin(), F.end());
		indices.insert(indices.end(), G.begin(), G.end());

		Objects2D::CreateMesh("8", vertices, indices, meshes);
	}
	
	{
		std::vector<unsigned short> indices;
		indices.insert(indices.end(), A.begin(), A.end());
		indices.insert(indices.end(), B.begin(), B.end());
		indices.insert(indices.end(), C.begin(), C.end());
		indices.insert(indices.end(), D.begin(), D.end());
		indices.insert(indices.end(), F.begin(), F.end());
		indices.insert(indices.end(), G.begin(), G.end());

		Objects2D::CreateMesh("9", vertices, indices, meshes);
	}
}
