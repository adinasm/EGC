#include "Worm.h"

Worm::Worm()
{
	materialShininess = 70;
	materialKd = 0.5;
	materialKs = 0.5;

	wormAngle = 0.f;
	gunAngle = 0.f;

	scale = 0.01f;
	gunScale = scale * 30;
	boundingBoxScale = 0.9f;

	wormPosition = glm::vec3(0.f, 0.f, 0.f);
	gunPosition = glm::vec3(0.f, 0.f, 0.f);
	boundingBoxPosition = glm::vec3(0.f, 0.f, 0.f);
}

Worm::~Worm()
{

}

unsigned int Worm::getMaterialShininess()
{
	return materialShininess;
}

float Worm::getMaterialKd()
{
	return materialKd;
}

float Worm::getMaterialKs()
{
	return materialKs;
}

float Worm::getWormAngle()
{
	return wormAngle;
}

void Worm::updateWormAngle(float offset)
{
	this->wormAngle += offset;
}

float Worm::getGunAngle()
{
	return gunAngle;
}

void Worm::updateGunAngle(float offset)
{
	this->gunAngle += offset;
}

float Worm::getScale()
{
	return scale;
}

float Worm::getGunScale()
{
	return gunScale;
}

float Worm::getBoundingBoxScale()
{
	return boundingBoxScale;
}

glm::vec3 Worm::getWormPosition()
{
	return wormPosition;
}

glm::vec3 Worm::getGunPosition()
{
	return gunPosition;
}

glm::vec3 Worm::getBoundingBoxPosition()
{
	return boundingBoxPosition;
}

// Sets the position of the newly created worm, its gun and its bounding box.
void Worm::createWorm(float x, float y, float z)
{
	wormPosition = glm::vec3(x, y - 0.1f, z);
	gunPosition = glm::vec3(x + 0.25f, y + 0.25f, z + 0.25f);
	boundingBoxPosition = glm::vec3(x + 0.05f, y + 0.075f, z);
}