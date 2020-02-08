#include "Bird.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Objects2D.h"

Bird::Bird()
{
	irisRadius			= 5.f;
	eyeRadius			= 7.f;
	headRadius			= 25.f;
	bodyRadius			= 40.f;
	circleRadiusX		= 8.f;
	circleRadiusY		= 5.f;
	decorationRadius	= 20.f;

	wingX		= 10.f;
	wingY		= 15.f;
	beakX		= 10.f;
	beakY		= 7.f;
	featherX	= -70.f;
	featherY	= 10.f;

	bodyCoordinates.x	= 150.f;
	bodyCoordinates.y	= 300.f;
	wingCoordinates.x	= bodyCoordinates.x;
	wingCoordinates.y	= bodyCoordinates.y;
	tailCoordinates.x	= bodyCoordinates.x - bodyRadius - featherX / 10.f;
	tailCoordinates.y	= bodyCoordinates.y;
	circleCoordinates.x = bodyCoordinates.x - bodyRadius + 4.f * featherX / 5.f;
	circleCoordinates.y = bodyCoordinates.y;

	headCoordinates.x	= bodyCoordinates.x + bodyRadius + 2.f * headRadius / 3.f;
	headCoordinates.y	= bodyCoordinates.y;
	beakCoordinates.x	= bodyCoordinates.x + bodyRadius + 4.5f * headRadius / 3.f;
	beakCoordinates.y	= bodyCoordinates.y - 2.5f;
	eyeCoordinates.x	= bodyCoordinates.x + bodyRadius + headRadius + irisRadius;
	eyeCoordinates.y	= bodyCoordinates.y + irisRadius;

	headDecorationCoordinates.x = bodyCoordinates.x + decorationRadius;
	headDecorationCoordinates.y = bodyCoordinates.y + headRadius;
}

Bird::~Bird()
{
}

glm::vec3 Bird::getBodyCoordinates()
{
	return bodyCoordinates;
}

glm::vec3 Bird::getWingCoordinates()
{
	return wingCoordinates;
}

glm::vec3 Bird::getTailCoordinates()
{
	return tailCoordinates;
}

glm::vec3 Bird::getCircleCoordinates()
{
	return circleCoordinates;
}

glm::vec3 Bird::getHeadCoordinates()
{
	return headCoordinates;
}

glm::vec3 Bird::getBeakCoordinates()
{
	return beakCoordinates;
}

glm::vec3 Bird::getEyeCoordinates()
{
	return eyeCoordinates;
}

glm::vec3 Bird::getHeadDecorationCoordinates()
{
	return headDecorationCoordinates;
}

float Bird::getHeadRadius()
{
	return headRadius;
}

float Bird::getBodyRadius()
{
	return bodyRadius;
}

// Creates meshes for the bird components.
void Bird::createBird(std::unordered_map<std::string, Mesh*>& meshes)
{
	glm::vec3 green = glm::vec3(0.078431f, 0.26274f, 0.137254f);
	glm::vec3 blue = glm::vec3(0.078431f, 0.149019, 0.223529);
	glm::vec3 brown = glm::vec3(0.078431f, 0.011274f, 0.013725f);
	glm::vec3 black = glm::vec3(1.f, 1.f, 1.f);

	createEllipse(irisRadius, irisRadius, "iris", brown, meshes);
	createEllipse(eyeRadius, eyeRadius, "eyeBall", black, meshes);
	createEllipse(headRadius, headRadius, "head", blue, meshes);
	createEllipse(bodyRadius, bodyRadius, "body", blue, meshes);
	createEllipse(circleRadiusX, circleRadiusY, "featherCircle", blue, meshes);
	createFeather("feather", green, meshes);
	createWing("wing", green, meshes);
	createBeak("beak", brown, meshes);
	createHeadDecoration(green, meshes);
}

// Creates an ellipse mesh. The vertex situated in the center is black, whereas the others have
//  the color received as a parameter.
void Bird::createEllipse(float radiusX, float radiusY, const char* name, glm::vec3 color,
	std::unordered_map<std::string, Mesh*>& meshes)
{
	int triangleCount = 100;
	float deg = 2.f * M_PI;
	float angle;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices.push_back(VertexFormat(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f)));
	indices.push_back(0.f);

	for (int i = 0; i <= triangleCount; ++i)
	{
		angle = i * deg / triangleCount;
		vertices.push_back(VertexFormat(glm::vec3(radiusX * cos(angle), radiusY * sin(angle), 1.f), color));
		indices.push_back(i + 1);
	}

	Mesh* circle = Objects2D::CreateMesh(name, vertices, indices, meshes);
	meshes[name]->SetDrawMode(GL_TRIANGLE_FAN);
}

// Creates a feather mesh.
void Bird::createFeather(const char* name, glm::vec3 color, std::unordered_map<std::string, Mesh*>& meshes)
{
	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f)),
		VertexFormat(glm::vec3(featherX, featherY, 1.f), color),
		VertexFormat(glm::vec3(featherX, -featherY, 1.f), color),
	};

	std::vector<unsigned short> indices =
	{
		0, 2, 1,
	};

	Mesh* circle = Objects2D::CreateMesh(name, vertices, indices, meshes);
}

// Creates a wing mesh.
void Bird::createWing(const char* name, glm::vec3 color, std::unordered_map<std::string, Mesh*>& meshes)
{
	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-wingX, 0.f, 1.f), color),
		VertexFormat(glm::vec3(wingX, 0.f, 1.f), color),
		VertexFormat(glm::vec3(0.f, wingY, 1.f), glm::vec3(0.f, 0.f, 0.f)),
	};

	std::vector<unsigned short> indices =
	{
		0, 2, 1,
	};

	Mesh* circle = Objects2D::CreateMesh(name, vertices, indices, meshes);
}

// Creates a beak mesh.
void Bird::createBeak(const char* name, glm::vec3 color, std::unordered_map<std::string, Mesh*>& meshes)
{
	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0.f, -beakY, 1.f), color),
		VertexFormat(glm::vec3(0.f, beakY, 1.f), color),
		VertexFormat(glm::vec3(beakX, 0.f, 1.f), color),
	};

	std::vector<unsigned short> indices =
	{
		0, 2, 1,
	};

	Mesh* circle = Objects2D::CreateMesh(name, vertices, indices, meshes);
}

// Creates a mesh for the head decoration.
void Bird::createHeadDecoration(glm::vec3 color, std::unordered_map<std::string, Mesh*>& meshes)
{
	int triangleCount = 3;
	float deg = M_PI / 3.f;
	float angle;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices.push_back(VertexFormat(glm::vec3(2 * decorationRadius, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f)));
	indices.push_back(0.f);

	for (int i = 0; i <= triangleCount; ++i)
	{
		angle = M_PI / 3.f + i * deg / triangleCount;
		vertices.push_back(VertexFormat(glm::vec3(decorationRadius * cos(angle), decorationRadius * sin(angle), 1.f),
			color));
		indices.push_back(i + 1);
	}

	Mesh* circle = Objects2D::CreateMesh("headDecoration", vertices, indices, meshes);
	meshes["headDecoration"]->SetDrawMode(GL_TRIANGLE_FAN);
}

// Rotates a bird component around its body.
glm::mat3 Bird::rotateAroundBody(glm::vec3 currentCoordinates, float angularStep) 
{
	glm::mat3 matrix;
	float offsetX = bodyCoordinates.x - currentCoordinates.x;
	float offsetY = bodyCoordinates.y - currentCoordinates.y;

	matrix = Transform2D::Translate(offsetX, offsetY);
	matrix *= Transform2D::Rotate(angularStep);
	matrix *= Transform2D::Translate(-offsetX, -offsetY);

	return matrix;
}