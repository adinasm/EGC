#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include <Teme\Tema2\Objects3D.h>

Tema2::Tema2()
{
	plane = new Plane();
	obstacles = new Obstacle();
	fuel = new Fuel();
	cloud = new Cloud();

	{
		glm::vec3 pos = plane->getHeadCoordinates();
		firstPersCameraPosition = glm::mat3(1);
		firstPersCameraPosition *= Transform3D::Translate(pos.x + plane->getHeadScale().x / 2, pos.y, pos.z);

		targetPosition = firstPersCameraPosition;
		targetPosition *= Transform3D::Translate(5.f, 0.f, 0.f);

		thirdPersCameraPosition = glm::mat4(1);
		thirdPersCameraPosition = Transform3D::Translate(0, 5.f, 20.f);
		thirdPersCamera = true;
	}

	//Light & material properties.
	{
		headlightPosition = glm::vec3(0.f, 0.f, 0.f);
		lightPosition = glm::vec3(9.f, 7.5f, -3.f);
		headlightDirection = glm::vec3(1.f, 0.f, 0.f);
		materialShininess = 50;
		materialKd = 0.5;
		materialKs = 0.5;

		cutOff = (float)M_PI / 6.f;
		lightAngle = 0.f;
	}

	// Data relevant to the plane movement.
	{
		planeAngle = 0.f;
		propAngle = (float)M_PI;
		sunAngle = 0.f;

		translateX = 0.f;
		translateY = 0.f;
	}

	// Data relevant to the state of the game.
	{
		lives = 3;
		currentFuel = 1000.f;
		maxFuel = 1000.f;
		crashAcceleration = -200.f;
		playing = true;
		first = true;
	}

	// Sea data.
	{
		height = 400.f;
		radius = 300.f;
	}

	// Color of certain plane components.
	color = glm::vec3(0.35f, 0.35f, 0.35f);

	renderBoundingBox = false;

	renderCameraTarget = false;

	zNear = 0.01f;
	zFar = 200.0f;
	fov = 45.0f;

	angleOffset = 0.f;

	camera = new Tema::Camera();
	camera->Set(thirdPersCameraPosition * glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
}

Tema2::~Tema2()
{
	delete plane;
	delete obstacles;
	delete fuel;
	delete cloud;
	delete camera;
}

void Tema2::Init()
{
	// Load meshes.
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create meshes.
	Objects3D::CreateSquare("square", 1.f, meshes);
	Objects3D::CreateTrunk(meshes);
	Objects3D::CreateWing(meshes);
	Objects3D::CreateCylinder(height, radius, meshes);

	// Create textures.
	CreateTextures();

	// Create shader programs.
	CreateShaders();
}

// Load textures.
void Tema2::CreateTextures()
{
	const std::string textureLoc = "Source/Teme/Tema2/Textures/";

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "airplane.jpg").c_str(), GL_REPEAT);
		mapTextures["airplane"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "wheel.jpg").c_str(), GL_REPEAT);
		mapTextures["wheel"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "prop.jpg").c_str(), GL_REPEAT);
		mapTextures["prop"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "facemap.png").c_str(), GL_REPEAT);
		mapTextures["head"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "neck.jpg").c_str(), GL_REPEAT);
		mapTextures["neck"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "sun.jpg").c_str(), GL_REPEAT);
		mapTextures["sun"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "fuel.jpg").c_str(), GL_REPEAT);
		mapTextures["fuel"] = texture;
	}
}

// Add shaders.
void Tema2::CreateShaders()
{
	{
		Shader* shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Teme/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("CloudShader");
		shader->AddShader("Source/Teme/Tema2/Shaders/CloudVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema2/Shaders/CloudFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("SeaShader");
		shader->AddShader("Source/Teme/Tema2/Shaders/SeaVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema2/Shaders/SeaFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("GlassShader");
		shader->AddShader("Source/Teme/Tema2/Shaders/GlassVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema2/Shaders/GlassFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}
void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.38f, 0.45f, 0.54f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	angleOffset = 0.5f * deltaTimeSeconds;

	// Checks if the game has ended.
	CheckEnd(deltaTimeSeconds);

	// Renders the plane, obstacles, fuel, clouds and background.
	RenderObstacles();
	RenderFuel();
	RenderClouds();
	RenderBackground(deltaTimeSeconds);
	RenderPlane(deltaTimeSeconds);

	// Render the camera target. Useful for understanding where is the rotation point in Third-person camera movement
	if (renderCameraTarget)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix, glm::vec3(0.5f, 0.08f, 0.08f), NULL);
	}
}

// Checks if the player still has fuel and lives. If it doesn't, then the plane crashes.
void Tema2::CheckEnd(float deltaTimeSeconds)
{
	if (first)
	{
		first = false;
		return;
	}

	if (playing)
	{
		currentFuel -= 20 * deltaTimeSeconds;
	}

	if (lives <= 0 || currentFuel <= 0.0f)
	{
		playing = false;

		if (plane->getBodyCoordinates().y + translateY > 0.f)
		{
			float offset = crashAcceleration * deltaTimeSeconds * deltaTimeSeconds / 2;
			translateX -= offset;
			translateY += offset;
		}

		if (planeAngle >= -M_PI / 4)
		{
			planeAngle -= deltaTimeSeconds;
		}
	}
}

// Checks if an obstacle or fuel item collided with the bounding box of the plane.
bool Tema2::CheckCollision(glm::vec3& position, float scale, glm::vec3& boundingBoxPosition,
	glm::vec3& boundingBoxScale, float planeAngle, float type)
{
	bool collision = false;

	float circleCos = cos(-planeAngle);
	float circleSin = sin(-planeAngle);
	float boxOffsetX = boundingBoxScale.x / 2;
	float boxOffsetY = boundingBoxScale.y / 2;

	glm::vec3 rotationCenter = plane->getBodyCoordinates();
	glm::vec3 planePosition = boundingBoxPosition + glm::vec3(translateX, translateY, 0.f);

	float circleX = circleCos * (position.x - rotationCenter.x) - circleSin * (position.y - rotationCenter.y)
		+ rotationCenter.x;
	float circleY = circleSin * (position.x - rotationCenter.x) + circleCos * (position.y - rotationCenter.y)
		+ rotationCenter.y;

	float x = std::max(planePosition.x - boxOffsetX, std::min(circleX, planePosition.x + boxOffsetX));
	float y = std::max(planePosition.y - boxOffsetY, std::min(circleY, planePosition.y + boxOffsetY));
	float z = std::max(planePosition.z - boundingBoxScale.z / 2,
		std::min(position.z, planePosition.z + boundingBoxScale.z / 2));

	float dist = (x - circleX) * (x - circleX) + (y - circleY) * (y - circleY);

	collision = dist < (scale / 2) * (scale / 2);

	if (collision)
	{
		// Decrease the lives if the item is an obstacle and increase the fuel amount if it is a fuel item.
		if (type == 0)
		{
			--lives;
		}
		else
		{
			currentFuel += type;
			currentFuel = currentFuel > maxFuel ? maxFuel : currentFuel;
		}
	}

	return collision;
}

// Renders the background.
void Tema2::RenderBackground(float deltaTimeSeconds)
{
	// Renders the sea.
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(0.f, -radius + 1.f, 0.f);
		modelMatrix *= Transform3D::RotateOX((float)M_PI / 2.f);
		modelMatrix *= Transform3D::RotateOY(sunAngle / 10);
		RenderSimpleMesh(meshes["cylinder"], shaders["SeaShader"], modelMatrix, glm::vec3(0.23f, 0.21f, 0.45f), NULL);
	}

	// Renders the fuel bar.
	glm::vec3 fuelBarPosition = glm::vec3(-17.f, 8.5f, -5.f);

	if (!thirdPersCamera)
	{
		glm::mat4 fuelBarMatrix = glm::mat4(1);
		fuelBarMatrix *= Transform3D::Translate(0.f, camera->position.y + 0.9f, camera->position.z + 1.f);
		fuelBarPosition = fuelBarMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	}

	if (currentFuel >= 0)
	{
		glm::mat4 modelMatrix = glm::mat4(1);

		if (!thirdPersCamera)
		{
			modelMatrix *= Transform3D::Translate(fuelBarPosition.x - 0.1f, fuelBarPosition.y, fuelBarPosition.z);
			modelMatrix *= Transform3D::RotateOY((float)-M_PI_2);
			modelMatrix *= Transform3D::Scale(1.2f * currentFuel / maxFuel, 0.2f, 0.25f);
		}
		else
		{
			modelMatrix *= Transform3D::Translate(fuelBarPosition.x + 0.1f, fuelBarPosition.y + 0.1f, fuelBarPosition.z);
			modelMatrix *= Transform3D::Scale(4.8f * currentFuel / maxFuel, 0.8f, 1.f);
		}

		RenderSimpleMesh(meshes["square"], shaders["Simple"], modelMatrix, glm::vec3(), mapTextures["fuel"]);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(fuelBarPosition.x, fuelBarPosition.y, fuelBarPosition.z);

		if (!thirdPersCamera)
		{
			modelMatrix *= Transform3D::Scale(0.25f, 0.25f, 1.25f);
			modelMatrix *= Transform3D::RotateOY((float)-M_PI_2);
		}
		else
		{
			modelMatrix *= Transform3D::Scale(5.f, 1.f, 1.f);
		}

		RenderSimpleMesh(meshes["square"], shaders["ShaderTema2"], modelMatrix, color, NULL);
	}

	// Renders the lives.
	if (thirdPersCamera)
	{
		for (int i = 0; i < lives; ++i) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(-16.5f + i * 0.7f, 8.f, -5.f);
			modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
			RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, glm::vec3(), mapTextures["fuel"]);
		}
	}
	else
	{
		for (int i = 0; i < lives; ++i) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(0.f, camera->position.y + 0.75f, camera->position.z + 1.1f + i * 0.2f);
			modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
			RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, glm::vec3(), mapTextures["fuel"]);
		}
	}

	// Renders the point light in the scene (the sun).
	{
		sunAngle += 0.5f * deltaTimeSeconds;
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, lightPosition);
		modelMatrix *= Transform3D::RotateOY(sunAngle);
		modelMatrix *= Transform3D::Scale(2.f, 2.f, 2.f);
		RenderSimpleMesh(meshes["sphere"], shaders["Simple"], modelMatrix, glm::vec3(), mapTextures["sun"]);
	}
}

void Tema2::RenderObstacles() {
	int obstacleCount = obstacles->getObstacleCount();
	float obstacleDistance = obstacles->getObstacleDistanceAngle();

	// Updates the positions and deletes the obstacles that should no longer be rendered.
	for (int i = 0; i < obstacleCount; ++i)
	{
		obstacles->setAngle(i, angleOffset);
		float angle = obstacles->getAngle(i);

		if (angle > M_PI) {
			obstacles->deleteObstacle(i);
			--obstacleCount;
		}
	}

	// Adds a new obstacle.
	if (!obstacleCount || obstacles->getAngle(obstacleCount - 1) >= obstacleDistance)
	{
		obstacles->createObstacle();
		++obstacleCount;
	}

	// Renders the obstacles.
	for (int i = 0; i < obstacleCount; ++i)
	{
		float angle = obstacles->getAngle(i);
		float radiusX = obstacles->getRadiusX(i);
		float radiusY = obstacles->getRadiusY(i);
		float scale = obstacles->getScale(i);

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(radiusX * cos(angle), radiusY * sin(angle), 0.f);
		modelMatrix *= Transform3D::Scale(scale, scale, scale);
		modelMatrix *= Transform3D::RotateOY(sunAngle);

		glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);

		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, glm::vec3(), mapTextures["prop"]);

		// Deletes the obstacle in case of a collision.
		if (playing && CheckCollision(position, scale, plane->getBoundingBoxCoordinates(), plane->getBoundingBoxScale(), planeAngle, 0))
		{
			obstacles->deleteObstacle(i);
			--i;
			--obstacleCount;
		}
	}
}

void Tema2::RenderFuel() {
	int fuelCount = fuel->getFuelCount();
	int sphereCount = fuel->getSphereCount();
	float fuelDistance = fuel->getFuelDistanceAngle();
	float scale = fuel->getScale();
	float fuelCapacity = fuel->getFuelCapacity();

	// Updates the positions and deletes the fuel items that should no longer be rendered.
	for (int i = 0; i < fuelCount; ++i)
	{
		fuel->setAngle(i, angleOffset);
		float angle = fuel->getAngle(i);

		if (angle > M_PI) {
			fuel->deleteFuel(i);
			--fuelCount;
		}
	}

	// Adds a new fuel item.
	if (!fuelCount || fuel->getAngle(fuelCount - 1) >= fuelDistance)
	{
		fuel->createFuel();
		++fuelCount;
	}

	// Renders the fuel items. Each item consists of three small spheres.
	for (int i = 0; i < fuelCount; ++i)
	{
		float angle = fuel->getAngle(i);
		float radiusX = fuel->getRadiusX(i);
		float radiusY = fuel->getRadiusY(i);

		for (int j = 0; j < sphereCount; ++j)
		{
			// If the current sphere from the fuel cluster has not already been collected.
			if (fuel->getSphereToRender(i, j))
			{
				glm::mat4 modelMatrix = glm::mat4(1);

				modelMatrix *= Transform3D::Translate(radiusX * (float)cos(angle + j * M_PI / 60),
					radiusY * (float)sin(angle + j * M_PI / 60), 0.f);
				modelMatrix *= Transform3D::Scale(scale, scale, scale);
				modelMatrix *= Transform3D::RotateOY(sunAngle);

				glm::vec3 position = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);

				RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, glm::vec3(),
					mapTextures["fuel"]);

				// Checks if the current sphere from the fuel cluster collided with the plane.
				if (playing && CheckCollision(position, scale, plane->getBoundingBoxCoordinates(),
					plane->getBoundingBoxScale(), planeAngle, fuelCapacity))
				{
					fuel->setSphereToRender(i, j);

					// Deletes the fuel item in case all of the spheres from the cluster have been collected.
					if (fuel->getDeletedSpheres(i) == sphereCount)
					{
						fuel->deleteFuel(i);
						--i;
						--fuelCount;
						break;
					}
				}
			}
		}
	}
}

void Tema2::RenderClouds() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int cloudCount = cloud->getCloudCount();
	float cloudDistance = cloud->getCloudDistanceAngle();

	// Updates the positions and deletes the clouds that should no longer be rendered.
	for (int i = 0; i < cloudCount; ++i)
	{
		cloud->setAngle(i, angleOffset);
		float angle = cloud->getAngle(i);

		if (angle > M_PI) {
			cloud->deleteCloud(i);
			--cloudCount;
		}
	}

	// Adds a new cloud.
	if (!cloudCount || cloud->getAngle(cloudCount - 1) >= cloudDistance)
	{
		cloud->createCloud();
		++cloudCount;
	}

	// Renders the cloud. Each cloud is a cluster of transparent spheres and/or boxes.
	for (int i = 0; i < cloudCount; ++i)
	{
		float angle = cloud->getAngle(i);
		float radiusX = cloud->getRadiusX(i);
		float radiusY = cloud->getRadiusY(i);
		float z = cloud->getZ(i);
		std::vector<float> scales = cloud->getScale(i);
		std::vector<std::string> shapes = cloud->getShapes(i);

		for (int j = 0; j < scales.size(); ++j)
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			if (j & 1)
			{
				modelMatrix *= Transform3D::Translate(radiusX * (float)cos(angle + j * M_PI / 160), radiusY * (float)sin(angle + j * M_PI / 160), z);
			}
			else
			{
				modelMatrix *= Transform3D::Translate(0.1f + radiusX * (float)cos(angle + (j - 1) * (float)M_PI / 160),
					0.3f + radiusY * (float)sin(angle + (j - 1) * (float)M_PI / 160), z);
			}
			modelMatrix *= Transform3D::Scale(scales[j], scales[j], scales[j]);
			modelMatrix *= Transform3D::RotateOX(sunAngle);
			modelMatrix *= Transform3D::RotateOZ(sunAngle);

			RenderSimpleMesh(meshes[shapes[j]], shaders["CloudShader"], modelMatrix, glm::vec3(0.9f, 0.55f, 0.7f), NULL);
		}
	}

	glDisable(GL_BLEND);
}

// Renders the plane and its components, updates the spotlight position and direction.
void Tema2::RenderPlane(float deltaTimeSeconds) {
	RenderPlaneComponent("light", "sphere", plane->getHeadlightsCoordinates(), plane->getHeadlightsScale());
	RenderPilot("sphere", "head", plane->getHeadCoordinates(), plane->getHeadScale());

	RenderPlaneComponent("neck", "box", plane->getNeckCoordinates(), plane->getNeckScale());
	RenderPlaneComponent("airplane", "box", plane->getBodyCoordinates(), plane->getBodyScale());
	RenderPlaneComponent("none", "trunk", plane->getTrunkCoordinates(), plane->getTrunkScale());
	RenderPlaneComponent("airplane", "box", plane->getTailCoordinates(), plane->getTailScale());
	RenderPlaneComponent("airplane", "box", plane->getMountCoordinates(), plane->getMountScale());
	RenderPlaneComponent("none", "box", plane->getCabinCoordinates(), plane->getCabinScale());
	RenderPlaneComponent("none", "box", plane->getFrontCoordinates(), plane->getFrontScale());
	RenderPlaneComponent("transparent", "box", plane->getWindowCoordinates(), plane->getWindowScale());

	RenderPlaneComponent("wheel", "sphere", plane->getLeftWheelCoordinates(), plane->getLeftWheelScale());
	RenderPlaneComponent("wheel", "sphere", plane->getRightWheelCoordinates(), plane->getRightWheelScale());
	RenderPlaneComponent("airplane", "box", plane->getLeftPillarCoordinates(), plane->getLeftPillarScale());
	RenderPlaneComponent("airplane", "box", plane->getRightPillarCoordinates(), plane->getRightPillarScale());

	RenderWings();
	RenderProp(deltaTimeSeconds, plane->getPropCoordinates(), plane->getPropScale(), "box");

	if (renderBoundingBox)
	{
		RenderPlaneComponent("transparent", "box", plane->getBoundingBoxCoordinates(), plane->getBoundingBoxScale());
	}
}

// Renders the wings of the plane.
void Tema2::RenderWings() {
	{
		glm::vec3 coordinates = plane->getLeftWingCoordinates();
		glm::vec3 scales = plane->getLeftWingScale();

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(coordinates.x + translateX, coordinates.y + translateY, coordinates.z);
		modelMatrix *= plane->rotateAroundBody(coordinates, planeAngle);
		modelMatrix *= Transform3D::Scale(scales.x, scales.y, scales.z);

		RenderSimpleMesh(meshes["wing"], shaders["ShaderTema2"], modelMatrix, color, NULL);
	}

	{
		glm::vec3 coordinates = plane->getRightWingCoordinates();
		glm::vec3 scales = plane->getRightWingScale();

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(coordinates.x + translateX, coordinates.y + translateY, coordinates.z);
		modelMatrix *= plane->rotateAroundBody(coordinates, planeAngle);
		modelMatrix *= Transform3D::RotateOY((float)M_PI);
		modelMatrix *= Transform3D::Scale(scales.x, scales.y, scales.z);

		RenderSimpleMesh(meshes["wing"], shaders["ShaderTema2"], modelMatrix, color, NULL);
	}
}

// Renders a plane component using the given mesh, texture or color; updates the spotlight position and direction.
void Tema2::RenderPlaneComponent(std::string textureName, std::string name, glm::vec3& coordinates, glm::vec3& scales) {
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(coordinates.x + translateX, coordinates.y + translateY, coordinates.z);
	modelMatrix *= plane->rotateAroundBody(coordinates, planeAngle);
	modelMatrix *= Transform3D::Scale(scales.x, scales.y, scales.z);

	if (textureName == "transparent")
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		RenderSimpleMesh(meshes[name], shaders["GlassShader"], modelMatrix, glm::vec3(0.9f, 0.55f, 0.7f), NULL);
		glDisable(GL_BLEND);
	}
	else if (textureName == "light")
	{
		// Updates the spotlight position and direction.
		glm::vec4 direction = glm::vec4(1.f, 0.f, 0.f, 0.f);

		glm::mat4 rot = glm::mat4(1);
		rot = glm::rotate(rot, -planeAngle, glm::vec3(0.f, 0.f, 1.f));

		headlightDirection = direction * rot;
		headlightPosition = modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);

		RenderSimpleMesh(meshes[name], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.95f, 0.82f, 0.31f), NULL);
	}
	else if (textureName != "none")
	{
		RenderSimpleMesh(meshes[name], shaders["ShaderTema2"], modelMatrix, glm::vec3(), mapTextures[textureName]);
	}
	else
	{
		RenderSimpleMesh(meshes[name], shaders["ShaderTema2"], modelMatrix, color, NULL);
	}
	
}

// Renders the head of the pilot.
void Tema2::RenderPilot(std::string meshName, std::string textureName, glm::vec3& coordinates, glm::vec3& scales) {
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(coordinates.x + translateX, coordinates.y + translateY, coordinates.z);
	modelMatrix *= plane->rotateAroundBody(coordinates, planeAngle);
	modelMatrix *= Transform3D::RotateOY((float)M_PI_2);
	modelMatrix *= Transform3D::Scale(scales.x, scales.y, scales.z);

	// Place the camera in the head of the pilot.
	firstPersCameraPosition = glm::mat4(1);
	firstPersCameraPosition *= Transform3D::Translate(coordinates.x + translateX + scales.x / 2,
		coordinates.y + translateY, coordinates.z);
	firstPersCameraPosition *= plane->rotateAroundBody(coordinates, planeAngle);

	targetPosition = firstPersCameraPosition;
	targetPosition *= Transform3D::Translate(5.f, 0.f, 0.f);
	
	if (!thirdPersCamera)
	{
		camera->Set(firstPersCameraPosition * glm::vec4(0.f, 0.f, 0.f, 1.f),
			targetPosition * glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(0, 1, 0));
	}

	RenderSimpleMesh(meshes[meshName], shaders["ShaderTema2"], modelMatrix, glm::vec3(), mapTextures[textureName]);
}

// Renders the propeller and the headlight.
void Tema2::RenderProp(float deltaTimeSeconds, glm::vec3& coordinates, glm::vec3& scales, std::string meshName) {
	propAngle += 4 * deltaTimeSeconds;

	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(coordinates.x + translateX, coordinates.y + translateY, coordinates.z);
	modelMatrix *= plane->rotateAroundBody(coordinates, planeAngle);
	modelMatrix *= Transform3D::RotateOX(propAngle);
	modelMatrix *= Transform3D::Scale(scales.x, scales.y, scales.z);

	RenderSimpleMesh(meshes[meshName], shaders["ShaderTema2"], modelMatrix, glm::vec3(), mapTextures["prop"]);
}

void Tema2::FrameEnd()
{
	// Decomment in order to see the coordinate system.
	// DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& objColor, Texture2D* texture)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// Set light position uniform
	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	// Set eye position (camera position) uniform
	glm::vec3 eyePosition = camera->position;
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, objColor.r, objColor.g, objColor.b);

	int headlight_position = glGetUniformLocation(shader->program, "headlight_position");
	glUniform3f(headlight_position, headlightPosition.x, headlightPosition.y, headlightPosition.z);

	int headlight_direction = glGetUniformLocation(shader->program, "headlight_direction");
	glUniform3f(headlight_direction, headlightDirection.x, headlightDirection.y, headlightDirection.z);

	int cut_off = glGetUniformLocation(shader->program, "cut_off");
	glUniform1f(cut_off, cutOff);

	if (!strcmp(shader->GetName(), "CloudShader") || !strcmp(shader->GetName(), "SeaShader"))
	{
		GLint loc_time = glGetUniformLocation(shader->program, "Time");
		float time = (float)Engine::GetElapsedTime();
		glUniform1f(loc_time, time);
	}

	if (!strcmp(shader->GetName(), "CloudShader") || !strcmp(shader->GetName(), "GlassShader"))
	{
		GLint loc_transparency = glGetUniformLocation(shader->program, "transparency");
		float transparency = 0.25f;
		glUniform1f(loc_transparency, transparency);
	}

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int useTexture = 0;

	if (texture)
	{
		// Activate texture location 0
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture1 ID
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

		// Send texture uniform value
		glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);

		useTexture = 1;
	}

	int use_texture_loc = glGetUniformLocation(shader->program, "use_texture");
	glUniform1i(use_texture_loc, useTexture);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// Translate the camera forward
			camera->TranslateForward(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// Translate the camera to the left
			camera->TranslateRight(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// Translate the camera backwards
			camera->TranslateForward(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// Translate the camera to the right
			camera->TranslateRight(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// Translate the camera down
			camera->TranslateUpword(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// Translate the camera up
			camera->TranslateUpword(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_1)) {
			// Translate the camera up
			fov += 3 * deltaTime * cameraSpeed;
			projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
		}

		if (window->KeyHold(GLFW_KEY_2)) {
			// Translate the camera up
			fov -= 3 * deltaTime * cameraSpeed;
			projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
		}
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}

	if (key == GLFW_KEY_B)
	{
		renderBoundingBox = !renderBoundingBox;
	}

	if (key == GLFW_KEY_C)
	{
		if (!thirdPersCamera)
		{
			camera->Set(thirdPersCameraPosition * glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
		}

		thirdPersCamera = !thirdPersCamera;
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.003f;
		float sensivityOY = 0.003f;

		if (window->GetSpecialKeyState() == 0)
		{
			renderCameraTarget = false;
			// Rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			if (!thirdPersCamera)
			{
				camera->RotateFirstPerson_OX(deltaY * sensivityOX);
				camera->RotateFirstPerson_OY(deltaX * sensivityOY);
			}
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL)
		{
			renderCameraTarget = true;
			// Rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			if (thirdPersCamera)
			{
				camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
				camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
			}
		}
	}
	else if (playing)
	{
		// Moves and rotates the plane by following the mouse movement.
		float sensivity = 0.007f;
		float offset = deltaY * sensivity;
		float angularOffset = sensivity * 10;

		translateY -= offset;

		if (offset > 0.f && planeAngle > -M_PI / 4)
		{
			planeAngle -= angularOffset;
		}
		else if (offset < 0.f && planeAngle < M_PI / 4)
		{
			planeAngle += angularOffset;
		}
		
	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
