#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>


class Obstacle
{
public:
	Obstacle();
	~Obstacle();

	int getObstacleCount();
	float getObstacleDistanceAngle();
	float getRadiusX(int index);
	float getRadiusY(int index);
	float getAngle(int index);
	void setAngle(int index, float value);
	float getScale(int index);

	void createObstacle();
	void deleteObstacle(int i);

private:
	float obstacleDistanceAngle;

	float minRadiusX;
	float maxRadiusX;
	float minRadiusY;
	float maxRadiusY;

	std::vector<float> scales;
	std::vector<float> radiusesX;
	std::vector<float> radiusesY;
	std::vector<float> angles;
};
