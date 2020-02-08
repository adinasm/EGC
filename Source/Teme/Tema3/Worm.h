#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include <Core/Engine.h>

class Worm
{
public:
	Worm();
	~Worm();

	unsigned int getMaterialShininess();
	float getMaterialKd();
	float getMaterialKs();

	float getWormAngle();
	void updateWormAngle(float offset);
	float getGunAngle();
	void updateGunAngle(float offset);
	float getScale();
	float getGunScale();
	float getBoundingBoxScale();

	glm::vec3 getWormPosition();
	glm::vec3 getGunPosition();
	glm::vec3 getBoundingBoxPosition();

	void createWorm(float x, float y, float z);

private:
	unsigned int materialShininess;
	float materialKd;
	float materialKs;

	float wormAngle;
	float gunAngle;

	float scale;
	float gunScale;
	float boundingBoxScale;

	glm::vec3 wormPosition;
	glm::vec3 gunPosition;
	glm::vec3 boundingBoxPosition;
};
