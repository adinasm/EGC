#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include <Core/Engine.h>

class Projectile
{
public:
	Projectile();
	~Projectile();

	unsigned int getMaterialShininess();
	float getMaterialKd();
	float getMaterialKs();

	float getAngleOX();
	void setAngleOX(float angleOX);
	void updateAngleOX(float offset);
	float getAngleOY();
	void setAngleOY(float angleOY);
	void updateAngleOY(float offset);
	float getScale();

	glm::vec3 getPosition();
	void setPosition(glm::vec3 position);
	void updatePosition(float deltaTimeSeconds);

	void launchProjectile(float angleOX, float angleOY, float gunScale, glm::vec3& gunPosition,
		glm::vec3& wormPosition);
	bool CollidedWithWorm(glm::vec3& boundingBox, float boundingBoxRadius);

private:
	unsigned int materialShininess;
	float materialKd;
	float materialKs;

	float angleOX;
	float angleOY;

	float velocityOX, velocityOY, velocityOZ;
	float initialVelocity;
	float gravitationalAcc;

	float scale;

	glm::vec3 position;
};
