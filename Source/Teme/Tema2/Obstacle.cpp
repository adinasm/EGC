#include "Obstacle.h"

#include <vector>

#include <Core/Engine.h>

Obstacle::Obstacle()
{
	obstacleDistanceAngle = M_PI / 5;

	minRadiusX = 18.f;
	maxRadiusX = 23.f;
	minRadiusY = 3.5f;
	maxRadiusY = 8.f;
}

Obstacle::~Obstacle()
{
}

int Obstacle::getObstacleCount()
{
	return angles.size();
}

float Obstacle::getObstacleDistanceAngle() {
	return obstacleDistanceAngle;
}

float Obstacle::getRadiusX(int index)
{
	return radiusesX[index];
}

float Obstacle::getRadiusY(int index)
{
	return radiusesY[index];
}


float Obstacle::getAngle(int index)
{
	return angles[index];
}

void Obstacle::setAngle(int index, float value)
{
	angles[index] += value;
}

float Obstacle::getScale(int index)
{
	return scales[index];
}

// Creates an obstacle with a random scale and a random movement.
void Obstacle::createObstacle()
{
	float scale = 0.7f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.4f - 0.7f)));
	scales.push_back(scale);

	float radius = minRadiusX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxRadiusX - minRadiusX)));
	radiusesX.push_back(radius);

	radius = minRadiusY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxRadiusY - minRadiusY)));
	radiusesY.push_back(radius);

	angles.push_back(0.f);
}

// Deletes an obstacle that should no longer be rendered.
void Obstacle::deleteObstacle(int i)
{
	scales.erase(scales.begin() + i);
	radiusesX.erase(radiusesX.begin() + i);
	radiusesY.erase(radiusesY.begin() + i);
	angles.erase(angles.begin() + i);
}
