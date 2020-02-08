#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include <Teme\Tema3\Ground.h>
#include <Teme\Tema3\Tema3Camera.h>
#include <Teme\Tema3\Worm.h>
#include <Teme\Tema3\Projectile.h>

class Tema3 : public SimpleScene
{
public:
	Tema3();
	~Tema3();

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

	void RenderImportedMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color,
		Texture2D* texture, unsigned int materialShininess, float materialKd, float materialKs, int wormId);

	void LoadMeshes();
	void LoadTextures();
	void AddShaders();
	void CreateWorms();

	void CheckCollisions(glm::vec3& projectilePosition);
	bool CheckLoser(int i, glm::vec3& projectilePosition);

	void RenderBackground(float deltaTimeSeconds);
	void RenderWorms(float deltaTimeSeconds);
	void RenderProjectile(float deltaTimeSeconds);

protected:
	Tema3Camera::Camera* camera;
	glm::mat4 projectionMatrix;
	bool renderCameraTarget;

	float zNear, zFar;
	float fov;

	glm::mat4 thirdPersCameraPosition;
	glm::mat4 firstPersCameraPosition;
	bool thirdPersCamera;

private:
	float skyRotationAngle;
	float sunRotationAngle;

	glm::vec3 lightPosition;
	glm::vec3 spotlightOffset;
	glm::vec3 spotlightDirection;
	float cutOff;

	bool playing;
	bool collided;
	float timeAfterCollision;

	bool renderBoundingBox;

	bool launchedProjectile;
	Projectile* projectile;
	
	int wormCount;
	int currentWorm;
	std::vector<Worm*> worms;
	std::vector<glm::vec3> cameraPositions;

	Ground* ground;

	std::unordered_map<std::string, Texture2D*> mapTextures;
};
