#include "Cloud.h"

#include <vector>

#include <Core/Engine.h>

Cloud::Cloud()
{
	cloudDistanceAngle = M_PI / 7;

	minRadiusX = 20.f;
	maxRadiusX = 23.f;
	minRadiusY = 3.f;
	maxRadiusY = 8.f;
	minZ = -6.f;
	maxZ = 1.f;

}

Cloud::~Cloud()
{
}

int Cloud::getCloudCount()
{
	return angles.size();
}


float Cloud::getCloudDistanceAngle()
{
	return cloudDistanceAngle;
}

float Cloud::getRadiusX(int index)
{
	return radiusesX[index];
}

float Cloud::getRadiusY(int index)
{
	return radiusesY[index];
}

float Cloud::getZ(int index)
{
	return z[index];
}


float Cloud::getAngle(int index)
{
	return angles[index];
}

void Cloud::setAngle(int index, float value)
{
	angles[index] += value;
}

std::vector<float> Cloud::getScale(int index)
{
	return scales[index];
}

std::vector<std::string> Cloud::getShapes(int index) {
	return shapes[index];
}

// Creates a cloud that consists of 5 to 9 boxes or spheres with random scales and curves.
void Cloud::createCloud()
{
	int numComponents = 5 + (rand()) % 4;
	std::vector<float> currentScales;
	std::vector<std::string> currentShapes;

	for (int i = 0; i < numComponents; ++i)
	{
		float scale = 0.8f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.4f - 0.8f)));
		currentScales.push_back(scale);

		int type = rand() % 2;
		if (type == 0)
		{
			currentShapes.push_back("sphere");
		}
		else
		{
			currentShapes.push_back("box");
		}
	}

	scales.push_back(currentScales);
	shapes.push_back(currentShapes);
	angles.push_back(0);

	float radius = minRadiusX + static_cast <float> (rand()) /
		(static_cast <float> (RAND_MAX / (maxRadiusX - minRadiusX)));
	radiusesX.push_back(radius);

	radius = minRadiusY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxRadiusY - minRadiusY)));
	radiusesY.push_back(radius);

	float newZ = minZ + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxZ - minZ)));
	z.push_back(newZ);
}

// Deletes a cloud that should no longer be rendered.
void Cloud::deleteCloud(int i)
{
	scales.erase(scales.begin() + i);
	radiusesX.erase(radiusesX.begin() + i);
	radiusesY.erase(radiusesY.begin() + i);
	shapes.erase(shapes.begin() + i);
	angles.erase(angles.begin() + i);
	z.erase(z.begin() + i);
}
