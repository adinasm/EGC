#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>


class Fuel
{
public:
	Fuel();
	~Fuel();

	int getSphereCount();
	int getFuelCount();
	float getFuelDistanceAngle();
	float getFuelCapacity();
	float getScale();
	float getRadiusX(int index);
	float getRadiusY(int index);
	float getAngle(int index);
	void setAngle(int index, float value);
	int getDeletedSpheres(int index);
	bool getSphereToRender(int index, int sphereIndex);
	void setSphereToRender(int index, int sphereIndex);

	void createFuel();
	void deleteFuel(int i);

private:
	float fuelDistanceAngle;
	float fuelCapacity;
	int sphereCount;

	float minRadiusX;
	float maxRadiusX;
	float minRadiusY;
	float maxRadiusY;
	
	float scale;

	std::vector<float> radiusesX;
	std::vector<float> radiusesY;
	std::vector<float> angles;
	std::vector<int> deletedSpheres;
	std::vector<std::vector<bool>> spheresToRender;
};
