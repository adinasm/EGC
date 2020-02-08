#include "Projectile.h"
#include <Teme\Tema3\Transform.h>

Projectile::Projectile()
{
	materialShininess = 20;
	materialKd = 0.5;
	materialKs = 0.5;

	angleOX = 0.f;
	angleOY = 0.f;

	scale = 0.25f;

	position = glm::vec3(0.f, 0.f, 0.f);

	initialVelocity = 2.5f;
	gravitationalAcc = 0.75f;
	velocityOX = 0.f;
	velocityOY = 0.f;
	velocityOZ = 0.f;
}

Projectile::~Projectile()
{

}

unsigned int Projectile::getMaterialShininess()
{
	return materialShininess;
}

float Projectile::getMaterialKd()
{
	return materialKd;
}

float Projectile::getMaterialKs()
{
	return materialKs;
}

float Projectile::getAngleOX()
{
	return angleOX;
}

void Projectile::setAngleOX(float angleOX)
{
	this->angleOX = angleOX;
}

void Projectile::updateAngleOX(float offset)
{
	this->angleOX += offset;
}

float Projectile::getAngleOY()
{
	return angleOY;
}

void Projectile::setAngleOY(float angleOY)
{
	this->angleOY = angleOY;
}


void Projectile::updateAngleOY(float offset)
{
	this->angleOY += offset;
}

float Projectile::getScale()
{
	return scale;
}

glm::vec3 Projectile::getPosition()
{
	return position;
}

void Projectile::setPosition(glm::vec3 position)
{
	this->position = position;
}

// Updates the position of the projectile (it has a parabolic movement).
void Projectile::updatePosition(float deltaTimeSeconds)
{
	velocityOY -= gravitationalAcc * deltaTimeSeconds;

	float offsetOX = velocityOX * deltaTimeSeconds;
	float offsetOY = velocityOY * deltaTimeSeconds - gravitationalAcc * deltaTimeSeconds * deltaTimeSeconds / 2.f;
	float offsetOZ = velocityOZ * deltaTimeSeconds;

	position += glm::vec3(offsetOX, offsetOY, offsetOZ);
}

// Launches the projectile: sets its angles, its position and its velocities along the axes.
void Projectile::launchProjectile(float angleOX, float angleOY, float gunScale, glm::vec3& gunPosition,
	glm::vec3& wormPosition)
{
	glm::vec3 position = gunPosition + glm::vec3(0.f, 0.f, gunScale);
	glm::mat4 modelMatrix = glm::mat4(1);

	modelMatrix *= Transform::Translate(position.x, position.y, position.z);
	modelMatrix *= Transform::Rotate(wormPosition, position, 0.f, angleOY, 0.f);
	modelMatrix *= Transform::Rotate(gunPosition, position, -angleOX, 0.f, 0.f);
	position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);

	this->angleOX = angleOX;
	this->angleOY = angleOY;
	this->position = position;

	velocityOX = initialVelocity * cos(angleOX) * sin(angleOY);
	velocityOY = initialVelocity * sin(angleOX);
	velocityOZ = initialVelocity * cos(angleOX) * cos(angleOY);
}

// Checks if two spheres collided.
bool Projectile::CollidedWithWorm(glm::vec3& boundingBox, float boundingBoxRadius) {
	float squaredDistance = (position.x - boundingBox.x) * (position.x - boundingBox.x) +
		(position.y - boundingBox.y) * (position.y - boundingBox.y) +
		(position.z - boundingBox.z) * (position.z - boundingBox.z);

	return squaredDistance < (scale / 2 + boundingBoxRadius) * (scale / 2 + boundingBoxRadius);
}