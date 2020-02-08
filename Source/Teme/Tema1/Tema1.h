#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <Teme/Tema1/Bird.h>
#include <Teme/Tema1/Digits.h>
#include <Teme/Tema1/Obstacles.h>

class Tema1 : public SimpleScene
{
public:

	Tema1();
	~Tema1();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	void printScore();
	void updateBird(float deltaTimeSeconds);
	void updateBody();
	void updateHead();
	void updateTailComponent(std::string name, glm::vec3 currentCoordinates, int index);
	void updateBirdComponent(std::string name, glm::vec3 currentCoordinates);
	void updateJump(float deltaTimeSeconds);
	void moveTail(float deltaTimeSeconds);
	void moveWing(float deltaTimeSeconds);
	void scaleObstacle(float deltaTimeSeconds);

	void updateStalactitesAndStalagmites(float deltaTimeSeconds);
	void RenderObstacle(glm::mat3& mm, int index);
	void checkCollision(glm::vec3& stalagmite, glm::vec3& stalactite, int currentIndex);

	void createTextures();
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture);

protected:
	glm::mat3 modelMatrix;
	float translateX, translateY;
	float acceleration;
	float jumpAcceleration, currentJumpY, maxJumpY, totalJumpY;
	bool jumping;
	bool playing;
	bool first;

	int textureCount;

	int score;
	float scoreboardX;
	float scoreboardY;

	float angularStep;

	float wingScaleY;
	float maxWingScaleY;
	float minWingScaleY;
	bool wingUp;

	float maxFloatAngle;
	float minFloatAngle;
	float floatStep;
	bool floatUp;

	float obstacleScale;
	float maxObstacleScale;
	float minObstacleScale;
	bool growObstacle;

	Bird bird;
	Obstacles obstacles;
	Digits digits;

	glm::vec3 headCoordinates;
	glm::vec3 bodyCoordinates;

	std::unordered_map<std::string, Texture2D*> mapTextures;

};
