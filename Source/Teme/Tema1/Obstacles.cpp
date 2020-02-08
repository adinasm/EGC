#include "Obstacles.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Objects2D.h"

Obstacles::Obstacles()
{
	squareSide			= 100.f;
	obstacleCount		= 8;
	obstacleDistance	= 330.f;
	stalactiteY			= 0.f;
	scaleTotal			= 4.f;
	initialX			= 300.f;
	position			= Transform2D::Translate(initialX, 0.f);
	first				= 0;
	textureCount		= 5;
}

Obstacles::~Obstacles()
{
}

int Obstacles::getObstacleCount()
{
	return obstacleCount;
}

float Obstacles::getSquareSide()
{
	return squareSide;
}

int Obstacles::getFirst()
{
	return first;
}

glm::mat3 Obstacles::getPosition()
{
	return position;
}

std::string Obstacles::getObstacleTexture(int index)
{
	return obstacleTextures[index];
}

float Obstacles::getScale(int index)
{
	return scales[index];
}

// Creates a square mesh  and assigns random textures and scales to the obstacles.
void Obstacles::initializeObstacles(std::unordered_map<std::string, Mesh*>& meshes,
	float resolutionX, float resolutionY, int textureNum)
{
	glm::vec3 corner = glm::vec3(0.f, 0.f, 0.f);
	stalactiteY = resolutionY;
	obstacleCount = ceil(resolutionX / obstacleDistance) + 1;
	textureCount = textureNum;

	for (int i = 0; i < obstacleCount; ++i)
	{
		Mesh* square = Objects2D::CreateSquare("square", corner, squareSide);
		obstacleTextures[i] = "crystal" + std::to_string(rand() % textureCount);
		meshes[square->GetMeshID()] = square;
	}

	// Generate initial scale factors.
	for (int i = 0; i < obstacleCount; ++i)
	{
		float downScale = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.5f - 1.f)));
		scales.push_back(downScale);
	}
}

// Updates the position of the leftmost obstacle.
bool Obstacles::updateFirst(float translateX)
{
	position *= Transform2D::Translate(translateX, 0.f);

	if (position[2][0] + obstacleDistance + squareSide < 0.f)
	{
		position *= Transform2D::Translate(obstacleDistance, 0.f);
		float downScale = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.5f - 1.f)));
		scales[first] = downScale;
		int newMesh = rand() % textureCount;
		obstacleTextures[first] = "crystal" + std::to_string(newMesh);
		first = (first + 1) % obstacleCount;

		return true;
	}

	return false;
}

// The position of the current stalagmite and stalactite is computed based on the
// current index and the position of the leftmost obstacle.
void Obstacles::createObstacles(int index, glm::mat3& stalagmitePosition, glm::mat3& stalactitePosition,
	float obstacleScale)
{
	stalagmitePosition *= Transform2D::Translate(obstacleDistance, 0.f);

	stalactitePosition = stalagmitePosition;
	stalactitePosition *= Transform2D::Translate(squareSide, stalactiteY);
	stalactitePosition *= Transform2D::Scale(1.f, scaleTotal - scales[index] - obstacleScale);
	stalactitePosition *= Transform2D::Rotate(M_PI);
}

bool Obstacles::checkCollisions(glm::vec3 bird, float radius, int index,
	glm::vec3& stalactite, glm::vec3& stalagmite, float obstacleScale)
{
	float downScale = scales[index] + obstacleScale;

	bool stalagmiteCollision = checkSingleCollision(bird, radius,
		glm::vec3(stalagmite.x, stalagmite.y, 1.f),
		glm::vec3(stalagmite.x + squareSide, stalagmite.y + squareSide * downScale, 1.f));

	bool stalactiteCollision = checkSingleCollision(bird, radius,
		glm::vec3(stalactite.x - squareSide, stalactite.y - squareSide * (scaleTotal - downScale), 1.f),
		glm::vec3(stalactite.x, stalactite.y, 1.f));

	return stalactiteCollision || stalagmiteCollision;
}

// Checks if a bird component collided with a given obstacle.
bool Obstacles::checkSingleCollision(glm::vec3 bird, float radius, glm::vec3& minObstacle, glm::vec3& maxObstacle)
{
	float x = std::max(minObstacle.x, std::min(bird.x, maxObstacle.x));
	float y = std::max(minObstacle.y, std::min(bird.y, maxObstacle.y));

	float distance = sqrt((x - bird.x) * (x - bird.x) + (y - bird.y) * (y - bird.y));
	return distance < radius;
}