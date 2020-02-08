#include "Plane.h"
#include "Objects3D.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include <Teme\Tema2\Transform3D.h>

// Initializes the scales and the coordinates of the plane components.
Plane::Plane()
{
	bodyScale = glm::vec3(0.8f, 0.5f, 0.5f);
	bodyCoordinates = glm::vec3(-3.5f, 5.5f, 0.f);

	windowScale = glm::vec3(1.3f, 0.4f, 0.5f);
	windowCoordinates = glm::vec3(bodyCoordinates.x - (windowScale.x - bodyScale.x) / 2 + 0.1f,
		bodyCoordinates.y + (bodyScale.y + windowScale.y) / 2, 0.f);

	cabinScale = glm::vec3(0.4f, 0.1f, 0.4f);
	cabinCoordinates = glm::vec3(bodyCoordinates.x, bodyCoordinates.y + (bodyScale.y + cabinScale.y) / 2, 0.f);

	frontScale = glm::vec3(0.1f, 0.5f, 0.5f);
	frontCoordinates = glm::vec3(bodyCoordinates.x + (bodyScale.x + frontScale.x) / 2,
		bodyCoordinates.y + (frontScale.y - bodyScale.y) / 2, 0.f);

	trunkScale = glm::vec3(0.7f, 0.5f, 0.5f);
	trunkCoordinates = glm::vec3(bodyCoordinates.x - (bodyScale.x + trunkScale.x) / 2, bodyCoordinates.y, 0.f);

	tailScale = glm::vec3(0.3f, 0.6f, 0.2f);
	tailCoordinates = glm::vec3(trunkCoordinates.x - trunkScale.x / 4, trunkCoordinates.y + trunkScale.y * 2 / 3, 0.f);

	mountScale = glm::vec3(0.15f, 0.1f, 0.1f);
	mountCoordinates = glm::vec3(frontCoordinates.x + (frontScale.x + mountScale.x) / 2, bodyCoordinates.y, 0.f);

	propScale = glm::vec3(0.15f, 0.15f, 0.8f);
	propCoordinates = glm::vec3(mountCoordinates.x + mountScale.x / 2, mountCoordinates.y, 0.f);

	neckScale = glm::vec3(0.05f, 0.1f, 0.05f);
	neckCoordinates = glm::vec3(cabinCoordinates.x - cabinScale.x / 4,
		cabinCoordinates.y + (cabinScale.y + neckScale.y) / 2, 0.f);

	headScale = glm::vec3(0.2f, 0.2f, 0.2f);
	headCoordinates = glm::vec3(neckCoordinates.x, neckCoordinates.y + (neckScale.y + headScale.y) / 3, 0.f);

	leftWingScale = glm::vec3(0.5f, 0.1f, 0.5f);
	leftWingCoordinates = glm::vec3(bodyCoordinates.x, bodyCoordinates.y,
		bodyCoordinates.z + (leftWingScale.z + bodyScale.z) / 2);

	rightWingScale = glm::vec3(0.5f, 0.1f, 0.5f);
	rightWingCoordinates = glm::vec3(bodyCoordinates.x, bodyCoordinates.y,
		bodyCoordinates.z - (leftWingScale.z + bodyScale.z) / 2);

	headlightsScale = glm::vec3(0.2f, 0.2f, 0.2f);
	headlightsCoordinates = glm::vec3(propCoordinates.x + propScale.x / 2, propCoordinates.y, 0.f);

	leftPillarScale = glm::vec3(0.025f, 0.1f, 0.025f);
	leftPillarCoordinates = glm::vec3(trunkCoordinates.x + trunkScale.x / 4, trunkCoordinates.y - 2 * trunkScale.y / 5,
		trunkCoordinates.z - trunkScale.z / 3);

	rightPillarScale = glm::vec3(0.025f, 0.1f, 0.025f);
	rightPillarCoordinates = glm::vec3(trunkCoordinates.x + trunkScale.x / 4, trunkCoordinates.y - 2 * trunkScale.y / 5,
		trunkCoordinates.z + trunkScale.z / 3);

	leftWheelScale = glm::vec3(0.15f, 0.15f, 0.15f);
	leftWheelCoordinates = glm::vec3(leftPillarCoordinates.x,
		leftPillarCoordinates.y - (leftPillarScale.y + leftWheelScale.y) / 2, leftPillarCoordinates.z);

	rightWheelScale = glm::vec3(0.15f, 0.15f, 0.15f);
	rightWheelCoordinates = glm::vec3(rightPillarCoordinates.x,
		rightPillarCoordinates.y - (rightPillarScale.y + rightWheelScale.y) / 2, rightPillarCoordinates.z);

	boundingBoxScale = glm::vec3(bodyScale.x + trunkScale.x + frontScale.x + propScale.x + 0.06f, bodyScale.y + windowScale.y, bodyScale.z);
	boundingBoxCoordinates = glm::vec3(bodyCoordinates.x - (boundingBoxScale.x - bodyScale.x) / 2 + tailScale.x + 0.02f,
		bodyCoordinates.y + (boundingBoxScale.y - bodyScale.y) / 2, 0.f);
}

Plane::~Plane()
{
}

glm::vec3 Plane::getBoundingBoxCoordinates() {
	return boundingBoxCoordinates;
}

void Plane::setBoundingBoxCoordinates(glm::vec3& boundingBoxCoordinates) {
	this->boundingBoxCoordinates = boundingBoxCoordinates;
}

glm::vec3 Plane::getBoundingBoxScale() {
	return boundingBoxScale;
}

void Plane::setBoundingBoxScale(glm::vec3& boundingBoxScale) {
	this->boundingBoxScale = boundingBoxScale;
}

glm::vec3 Plane::getBodyCoordinates() {
	return bodyCoordinates;
}

void Plane::setBodyCoordinates(glm::vec3& bodyCoordinates) {
	this->bodyCoordinates = bodyCoordinates;
}

glm::vec3 Plane::getWindowCoordinates() {
	return windowCoordinates;
}

void Plane::setWindowCoordinates(glm::vec3& windowCoordinates) {
	this->windowCoordinates = windowCoordinates;
}

glm::vec3 Plane::getWindowScale() {
	return windowScale;
}

void Plane::setWindowScale(glm::vec3& windowScale) {
	this->windowScale = windowScale;
}

glm::vec3 Plane::getCabinCoordinates() {
	return cabinCoordinates;
}

void Plane::setCabinCoordinates(glm::vec3& cabinCoordinates) {
	this->cabinCoordinates = cabinCoordinates;
}

glm::vec3 Plane::getCabinScale() {
	return cabinScale;
}

void Plane::setCabinScale(glm::vec3& cabinScale) {
	this->cabinScale = cabinScale;
}

glm::vec3 Plane::getFrontCoordinates() {
	return frontCoordinates;
}

void Plane::setFrontCoordinates(glm::vec3& frontCoordinates) {
	this->frontCoordinates = frontCoordinates;
}

glm::vec3 Plane::getFrontScale() {
	return frontScale;
}

void Plane::setFrontScale(glm::vec3& frontScale) {
	this->frontScale = frontScale;
}

glm::vec3 Plane::getTailCoordinates() {
	return tailCoordinates;
}

void Plane::setTailCoordinates(glm::vec3& tailCoordinates) {
	this->tailCoordinates = tailCoordinates;
}

glm::vec3 Plane::getTrunkCoordinates() {
	return trunkCoordinates;
}

void Plane::setTrunkCoordinates(glm::vec3& trunkCoordinates) {
	this->trunkCoordinates = trunkCoordinates;
}

glm::vec3 Plane::getPropCoordinates() {
	return propCoordinates;
}

void Plane::setPropCoordinates(glm::vec3& propCoordinates) {
	this->propCoordinates = propCoordinates;
}

glm::vec3 Plane::getMountCoordinates() {
	return mountCoordinates;
}

void Plane::setMountCoordinates(glm::vec3& mountCoordinates) {
	this->mountCoordinates = mountCoordinates;
}

glm::vec3 Plane::getLeftWingCoordinates() {
	return leftWingCoordinates;
}

void Plane::setLeftWingCoordinates(glm::vec3& leftWingCoordinates) {
	this->leftWingCoordinates = leftWingCoordinates;
}

glm::vec3 Plane::getRightWingCoordinates() {
	return rightWingCoordinates;
}

void Plane::setRightWingCoordinates(glm::vec3& rightWingCoordinates) {
	this->rightWingCoordinates = rightWingCoordinates;
}

glm::vec3 Plane::getNeckCoordinates() {
	return neckCoordinates;
}

void Plane::setNeckCoordinates(glm::vec3& neckCoordinates) {
	this->neckCoordinates = neckCoordinates;
}

glm::vec3 Plane::getHeadCoordinates() {
	return headCoordinates;
}

void Plane::setHeadCoordinates(glm::vec3& headCoordinates) {
	this->headCoordinates = headCoordinates;
}

glm::vec3 Plane::getBodyScale() {
	return bodyScale;
}

void Plane::setBodyScale(glm::vec3& bodyScale) {
	this->bodyScale = bodyScale;
}

glm::vec3 Plane::getTailScale() {
	return tailScale;
}

void Plane::setTailScale(glm::vec3& tailScale) {
	this->tailScale = tailScale;
}

glm::vec3 Plane::getTrunkScale() {
	return trunkScale;
}

void Plane::setTrunkScale(glm::vec3& trunkScale) {
	this->trunkScale = trunkScale;
}

glm::vec3 Plane::getPropScale() {
	return propScale;
}

void Plane::setPropScale(glm::vec3& propScale) {
	this->propScale = propScale;
}

glm::vec3 Plane::getMountScale() {
	return mountScale;
}

void Plane::setMountScale(glm::vec3& mountScale) {
	this->mountScale = mountScale;
}

glm::vec3 Plane::getLeftWingScale() {
	return leftWingScale;
}

void Plane::setLeftWingScale(glm::vec3& leftWingScale) {
	this->leftWingScale = leftWingScale;
}

glm::vec3 Plane::getRightWingScale() {
	return rightWingScale;
}

void Plane::setRightWingScale(glm::vec3& rightWingScale) {
	this->rightWingScale = rightWingScale;
}

glm::vec3 Plane::getNeckScale() {
	return neckScale;
}

void Plane::setNeckScale(glm::vec3& neckScale) {
	this->neckScale = neckScale;
}

glm::vec3 Plane::getHeadScale() {
	return headScale;
}

void Plane::setHeadScale(glm::vec3& headScale) {
	this->headScale = headScale;
}


glm::vec3 Plane::getHeadlightsCoordinates() {
	return headlightsCoordinates;
}

void Plane::setHeadlightsCoordinates(glm::vec3& headlightsCoordinates) {
	this->headlightsCoordinates = headlightsCoordinates;
}

glm::vec3 Plane::getHeadlightsScale() {
	return headlightsScale;
}

void Plane::setHeadlightsScale(glm::vec3& headlightsScale) {
	this->headlightsScale = headlightsScale;
}

glm::vec3 Plane::getLeftWheelCoordinates() {
	return leftWheelCoordinates;
}

void Plane::setLeftWheelCoordinatess(glm::vec3& leftWheelCoordinates) {
	this->leftWheelCoordinates = leftWheelCoordinates;
}

glm::vec3 Plane::getLeftWheelScale() {
	return leftWheelScale;
}

void Plane::setLeftWheelScale(glm::vec3& leftWheelScale) {
	this->leftWheelScale = leftWheelScale;
}

glm::vec3 Plane::getRightWheelCoordinates() {
	return rightWheelCoordinates;
}

void Plane::setRightWheelCoordinates(glm::vec3& rightWheelCoordinates) {
	this->rightWheelCoordinates = rightWheelCoordinates;
}

glm::vec3 Plane::getRightWheelScale() {
	return rightWheelScale;
}

void Plane::setRightWheelScale(glm::vec3& rightWheelScale) {
	this->rightWheelScale = rightWheelScale;
}

glm::vec3 Plane::getLeftPillarCoordinates() {
	return leftPillarCoordinates;
}

void Plane::setLeftPillarCoordinates(glm::vec3& leftPillarCoordinates) {
	this->leftPillarCoordinates = leftPillarCoordinates;
}

glm::vec3 Plane::getLeftPillarScale() {
	return leftPillarScale;
}

void Plane::setLeftPillarScale(glm::vec3& leftPillarScale) {
	this->leftPillarScale = leftPillarScale;
}

glm::vec3 Plane::getRightPillarCoordinates() {
	return rightPillarCoordinates;
}

void Plane::setRightPillarCoordinates(glm::vec3& rightPillarCoordinates) {
	this->rightPillarCoordinates = rightPillarCoordinates;
}

glm::vec3 Plane::getRightPillarScale() {
	return rightPillarScale;
}

void Plane::setRightPillarScale(glm::vec3& rightPillarScale) {
	this->rightPillarScale = rightPillarScale;
}

// Rotates a plane component around its body.
glm::mat4 Plane::rotateAroundBody(glm::vec3 currentCoordinates, float angularStep)
{
	glm::mat4 matrix;

	float offsetX = bodyCoordinates.x - currentCoordinates.x;
	float offsetY = bodyCoordinates.y - currentCoordinates.y;

	matrix = Transform3D::Translate(offsetX, offsetY, 0.f);
	matrix *= Transform3D::RotateOZ(angularStep);
	matrix *= Transform3D::Translate(-offsetX, -offsetY, 0.f);

	return matrix;
}