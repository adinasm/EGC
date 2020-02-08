#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <unordered_map>
#include <Core/Engine.h>
#include <Teme/Tema1/Bird.h>


class Obstacles
{
public:
	Obstacles();
	~Obstacles();

	int getObstacleCount();
	float getSquareSide();
	int getFirst();
	glm::mat3 getPosition();
	std::string getObstacleTexture(int index);
	float getScale(int index);

	void initializeObstacles(std::unordered_map<std::string, Mesh*>& meshes,
		float resolutionX, float resolutionY, int textureNum);
	bool updateFirst(float translateX);
	void createObstacles(int index, glm::mat3& stalagmitePosition, glm::mat3& stalactitePosition,
		float obstacleScale);
	bool checkCollisions(glm::vec3 bird, float radius, int index,
		glm::vec3& stalactite, glm::vec3& stalagmite, float obstacleScale);
	bool checkSingleCollision(glm::vec3 bird, float radius, glm::vec3& minObstacle, glm::vec3& maxObstacle);

private:
	float squareSide;
	float obstacleDistance;
	float stalactiteY;
	float initialX;
	float scaleTotal;
	int obstacleCount;
	int first;
	int textureCount;

	glm::mat3 position;
	std::unordered_map<int, std::string> obstacleTextures;
	std::vector<float> scales;
};
