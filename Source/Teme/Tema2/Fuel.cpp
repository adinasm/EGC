#include "Fuel.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

Fuel::Fuel()
{
	fuelDistanceAngle = M_PI / 3;
	fuelCapacity = 10.f;
	sphereCount = 5;

	minRadiusX = 18.f;
	maxRadiusX = 21.f;
	minRadiusY = 3.5f;
	maxRadiusY = 8.f;

	scale = 0.3f;
}

Fuel::~Fuel()
{
}

int Fuel::getSphereCount()
{
	return sphereCount;
}

float Fuel::getScale()
{
	return scale;
}

int Fuel::getFuelCount()
{
	return angles.size();
}

float Fuel::getFuelDistanceAngle()
{
	return fuelDistanceAngle;
}

float Fuel::getFuelCapacity()
{
	return fuelCapacity;
}

float Fuel::getRadiusX(int index)
{
	return radiusesX[index];
}

float Fuel::getRadiusY(int index)
{
	return radiusesY[index];
}


float Fuel::getAngle(int index)
{
	return angles[index];
}

void Fuel::setAngle(int index, float value)
{
	angles[index] += value;
}

int Fuel::getDeletedSpheres(int index)
{
	return deletedSpheres[index];
}

bool Fuel::getSphereToRender(int index, int sphereIndex)
{
	return spheresToRender[index][sphereIndex];
}

void Fuel::setSphereToRender(int index, int sphereIndex)
{
	spheresToRender[index][sphereIndex] = false;
	++deletedSpheres[index];
}

// Creates a fuel with a random movement.
void Fuel::createFuel()
{
	float radius = minRadiusX + static_cast <float> (rand()) /
		(static_cast <float> (RAND_MAX / (maxRadiusX - minRadiusX)));
	radiusesX.push_back(radius);

	radius = minRadiusY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxRadiusY - minRadiusY)));
	radiusesY.push_back(radius);

	angles.push_back(0);

	deletedSpheres.push_back(0);
	spheresToRender.push_back(std::vector<bool>(sphereCount, true));
}

// Deletes a fuel item that should no longer be rendered.
void Fuel::deleteFuel(int i)
{
	radiusesX.erase(radiusesX.begin() + i);
	radiusesY.erase(radiusesY.begin() + i);
	angles.erase(angles.begin() + i);
	deletedSpheres.erase(deletedSpheres.begin() + i);
	spheresToRender.erase(spheresToRender.begin() + i);
}
