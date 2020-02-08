#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Bird
{
public:

	Bird();
	~Bird();

	void createBird(std::unordered_map<std::string, Mesh*>& meshes);
	glm::mat3 rotateAroundBody(glm::vec3 currentCoordinates, float angularStep);

	glm::vec3 getBodyCoordinates();
	glm::vec3 getWingCoordinates();
	glm::vec3 getTailCoordinates();
	glm::vec3 getCircleCoordinates();
	glm::vec3 getHeadCoordinates();
	glm::vec3 getBeakCoordinates();
	glm::vec3 getEyeCoordinates();
	glm::vec3 getHeadDecorationCoordinates();

	float getHeadRadius();
	float getBodyRadius();

private:
	void createEllipse(float radiusX, float radiusY, const char* name, glm::vec3 color,
		std::unordered_map<std::string, Mesh*>& meshes);
	void createFeather(const char* name, glm::vec3 color, std::unordered_map<std::string, Mesh*>& meshes);
	void createWing(const char* name, glm::vec3 color, std::unordered_map<std::string, Mesh*>& meshes);
	void createBeak(const char* name, glm::vec3 color, std::unordered_map<std::string, Mesh*>& meshes);
	void createHeadDecoration(glm::vec3 color, std::unordered_map<std::string, Mesh*>& meshes);

	glm::vec3 bodyCoordinates, wingCoordinates, tailCoordinates, circleCoordinates;
	glm::vec3 headCoordinates, beakCoordinates, eyeCoordinates, headDecorationCoordinates;

	float bodyRadius, headRadius, circleRadiusX, circleRadiusY, eyeRadius, irisRadius, decorationRadius;
	float wingX, wingY, beakX, beakY, featherX, featherY;
};
