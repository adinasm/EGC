#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"
#include "Transform3D.h"
#include <Teme\Tema2\Plane.h>
#include <Teme\Tema2\Obstacle.h>
#include <Teme\Tema2\Fuel.h>
#include <Teme\Tema2\Cloud.h>
#include <Teme\Tema2\Cloud.h>

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color,
			Texture2D* texture);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		void CreateTextures();
		void CreateShaders();

		void CheckEnd(float deltaTimeSeconds);
		bool CheckCollision(glm::vec3& position, float scale, glm::vec3& boundingBoxPosition,
			glm::vec3& boundingBoxScale, float planeAngle, float type);

		void RenderPlane(float deltaTimeSeconds);
		void RenderBackground(float deltaTimeSeconds);
		void RenderObstacles();
		void RenderFuel();
		void RenderClouds();
		void RenderPlaneComponent(std::string textureName, std::string name, glm::vec3& coordinates, glm::vec3& scales);
		void RenderWings();
		void RenderPilot(std::string meshName, std::string textureName, glm::vec3& coordinates, glm::vec3& scales);
		void RenderProp(float deltaTimeSeconds, glm::vec3& coordinates, glm::vec3& scales,
			std::string meshName);

	protected:
		Tema::Camera* camera;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;

		float zNear, zFar;
		float fov;

	private:
		Plane* plane;
		Obstacle* obstacles;
		Fuel* fuel;
		Cloud* cloud;

		glm::mat4 thirdPersCameraPosition;
		glm::mat4 firstPersCameraPosition;
		glm::mat4 targetPosition;
		bool thirdPersCamera;

		glm::vec3 lightPosition;
		glm::vec3 headlightDirection;
		glm::vec3 headlightPosition;
		unsigned int materialShininess;
		float materialKd;
		float materialKs;

		float cutOff;

		float translateX, translateY;

		bool playing;
		int lives;
		float maxFuel, currentFuel, crashAcceleration;
		bool first;
		bool renderBoundingBox;

		float planeAngle;
		float propAngle;
		float sunAngle;
		
		glm::vec3 color;

		float angleOffset;

		float radius, height;

		float lightAngle;

		std::unordered_map<std::string, Texture2D*> mapTextures;
};
