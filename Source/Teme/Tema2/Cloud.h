#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>


class Cloud
{
public:
	Cloud();
	~Cloud();

	int getCloudCount();
	float getCloudDistanceAngle();
	float getRadiusX(int index);
	float getRadiusY(int index);
	float getZ(int index);
	float getAngle(int index);
	void setAngle(int index, float value);
	std::vector<float> getScale(int index);
	std::vector<std::string> getShapes(int index);

	void createCloud();
	void deleteCloud(int i);

private:
	float cloudDistanceAngle;
	float minRadiusX;
	float maxRadiusX;
	float minRadiusY;
	float maxRadiusY;
	float minZ;
	float maxZ;

	std::vector<std::vector<float>> scales;
	std::vector<float> radiusesX;
	std::vector<float> radiusesY;
	std::vector<float> angles;
	std::vector<float> z;
	std::vector<std::vector<std::string>> shapes;
};
