#include "Tema3.h"

#include <vector>
#include <string>

#include <Core/Engine.h>
#include <Teme\Tema3\Objects.h>
#include <Teme\Tema3\Transform.h>
#include <stb/stb_image.h>
#include <iostream>

Tema3::Tema3()
{
	// Ground information.
	{
		ground = new Ground();
	}

	// Background information.
	{
		skyRotationAngle = 0.f;
		sunRotationAngle = 0.f;
	}

	// Light information.
	{
		lightPosition = glm::vec3(-15.f, 7.5f, 0.f);
		spotlightOffset = glm::vec3(0.f, 1.f, 0.f);
		spotlightDirection = glm::vec3(0.f, -1.f, 0.f);

		cutOff = (float)M_PI / 12;
	}

	// Camera information.
	{
		renderCameraTarget = false;
		thirdPersCamera = false;

		zNear = 0.01f;
		zFar = 200.0f;
		fov = 60.0f;

		camera = new Tema3Camera::Camera();

		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
	}

	// Worm information.
	{
		wormCount = 2;
		currentWorm = 0;
		renderBoundingBox = false;
	}

	// Projectile information;
	{
		launchedProjectile = false;
		projectile = new Projectile();
	}

	{
		playing = true;
		collided = false;
		timeAfterCollision = 0.f;
	}
}

Tema3::~Tema3()
{
	delete ground;
}

void Tema3::Init()
{
	LoadMeshes();

	LoadTextures();
	AddShaders();

	ground->CreateGround(20.f, meshes);
	mapTextures["heightmap"] = ground->CreateHeightMap();

	CreateWorms();

	camera->Set(cameraPositions[currentWorm], worms[currentWorm]->getWormPosition() + glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(0, 1, 0));
}

// Load meshes.
void Tema3::LoadMeshes()
{
	const std::string meshesLoc = "Source/Teme/Tema3/Meshes/";

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("gun");
		mesh->LoadMesh(meshesLoc + "Gun", "Portal Gun.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("dragon");
		mesh->LoadMesh(meshesLoc + "Dragon", "10002_Armadillo_v1_L3.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
}

// Load textures.
void Tema3::LoadTextures()
{
	const std::string textureLoc = "Source/Teme/Tema3/Textures/";

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "sky.jpg").c_str(), GL_REPEAT);
		mapTextures["sky"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "sun.jpg").c_str(), GL_REPEAT);
		mapTextures["sun"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "glass.jpg").c_str(), GL_REPEAT);
		mapTextures["glass"] = texture;
	}
}

// Add shaders.
void Tema3::AddShaders()
{
	{
		Shader* shader = new Shader("GroundShader");
		shader->AddShader("Source/Teme/Tema3/Shaders/GroundVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema3/Shaders/GroundFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("WormShader");
		shader->AddShader("Source/Teme/Tema3/Shaders/WormVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema3/Shaders/WormFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("BasicShader");
		shader->AddShader("Source/Teme/Tema3/Shaders/BasicVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema3/Shaders/BasicFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

// Create wormCount worms (maximum 2) and compute their initial position.
void Tema3::CreateWorms()
{
	float sideLength = ground->getSideLength();
	float heightMapHeight = (float)ground->getHeightMapHeight();
	float heightMapWidth = (float)ground->getHeightMapWidth();
	float wormDistanceX = heightMapWidth / 3;
	float wormDistanceZ = heightMapHeight / 3;
	float x = heightMapWidth / 4;
	float z = heightMapHeight / 4;
	float currentPosition = -sideLength / 4;
	float positionDistance = sideLength / 3;
	float offsetX = -1.f;
	float offsetZ = -1.4f;
	float heightFactor = ground->getHeightFactor();

	wormCount = wormCount > 2 ? 2 : wormCount;

	for (int i = 0; i < wormCount;
		++i, x += wormDistanceX, z += wormDistanceZ, currentPosition += positionDistance,
		offsetX -= 2 * offsetX, offsetZ -= 2 * offsetZ)
	{
		worms.push_back(new Worm());

		float height = heightFactor * ground->getHeight((int)z * ground->getHeightMapWidth() + (int)x) / 255
			+ worms[i]->getScale() / 2;

		worms[i]->createWorm(currentPosition, height, currentPosition);

		cameraPositions.push_back(worms[i]->getWormPosition() + glm::vec3(offsetX, 0.9f, offsetZ));
	}
}

void Tema3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::Update(float deltaTimeSeconds)
{
	if (collided)
	{
		timeAfterCollision += deltaTimeSeconds;

		if (timeAfterCollision > 0.5f)
		{
			currentWorm = (currentWorm + 1) % wormCount;
			camera->Set(cameraPositions[currentWorm], worms[currentWorm]->getWormPosition() + glm::vec3(0.f, 1.f, 0.f),
				glm::vec3(0, 1, 0));
			timeAfterCollision = 0.f;
			collided = false;
		}
	}

	RenderBackground(deltaTimeSeconds);
	RenderProjectile(deltaTimeSeconds);
	RenderWorms(deltaTimeSeconds);

	// Render the camera target. Useful for understanding where is the rotation point in Third-person camera movement
	if (renderCameraTarget)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
	}
}

// Render the ground, the sun and the sky.
void Tema3::RenderBackground(float deltaTimeSeconds)
{
	skyRotationAngle += deltaTimeSeconds / 100;
	sunRotationAngle += deltaTimeSeconds / 50;

	{
		float halfSideLength = ground->getSideLength() / 2;
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(-halfSideLength, 0.f, -halfSideLength);
		RenderSimpleMesh(meshes["ground"], shaders["GroundShader"], modelMatrix, glm::vec3(),
			ground->getCurrentTexture(), ground->getMaterialShininess(), ground->getMaterialKd(),
			ground->getMaterialKs(), -1);
	}

	{
		lightPosition = glm::vec3(-15.f * cos(sunRotationAngle), 7.5f, 15.f * sin(sunRotationAngle));
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Scale(0.5f, 0.5f, 0.5f);
		modelMatrix *= Transform::Translate(lightPosition.x, lightPosition.y, lightPosition.z);
		modelMatrix *= Transform::RotateOY(sunRotationAngle);
		RenderSimpleMesh(meshes["sphere"], shaders["Simple"], modelMatrix, glm::vec3(), mapTextures["sun"], 0, 0.f,
			0.f, -1);
	}

	{
		float sideLength = ground->getSideLength();
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(0.f, 0.f, 0.f);
		modelMatrix *= Transform::Scale(sideLength, sideLength, sideLength);
		modelMatrix *= Transform::RotateOY(skyRotationAngle);
		RenderSimpleMesh(meshes["sphere"], shaders["Simple"], modelMatrix, glm::vec3(), mapTextures["sky"], 0, 0.f,
			0.f, -1);
	}
}

// Render the worms.
void Tema3::RenderWorms(float deltaTimeSeconds)
{
	for (int i = 0; i < wormCount; ++i)
	{
		// Render the worm.
		float wormAngle = worms[i]->getWormAngle();
		float scale = worms[i]->getScale();
		float gunScale = worms[i]->getGunScale();
		float boundingBoxScale = worms[i]->getBoundingBoxScale();
		glm::vec3 wormPosition = worms[i]->getWormPosition();
		glm::vec3 gunPosition = worms[i]->getGunPosition();
		glm::vec3 boundingBoxPosition = worms[i]->getBoundingBoxPosition();
		glm::mat4 modelMatrix = glm::mat4(1);

		modelMatrix *= Transform::Translate(wormPosition.x, wormPosition.y, wormPosition.z);
		modelMatrix *= Transform::Scale(scale, scale, scale);
		modelMatrix *= Transform::RotateOY(wormAngle);
		modelMatrix *= Transform::RotateOX(-4.f * (float)M_PI / 5.f);
		RenderImportedMesh(meshes["dragon"], shaders["Simple"], modelMatrix);

		// Render its gun.
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(gunPosition.x, gunPosition.y, gunPosition.z);
		modelMatrix *= Transform::Rotate(wormPosition, gunPosition, 0.f, wormAngle, 0.f);
		modelMatrix *= Transform::RotateOY((float)M_PI);
		modelMatrix *= Transform::RotateOY((float)-M_PI / 4);
		modelMatrix *= Transform::RotateOX(worms[i]->getGunAngle());
		modelMatrix *= Transform::Scale(gunScale, gunScale, gunScale);
		RenderImportedMesh(meshes["gun"], shaders["Simple"], modelMatrix);

		// Render the bounding box.
		if (renderBoundingBox)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform::Translate(boundingBoxPosition.x, boundingBoxPosition.y, boundingBoxPosition.z);
			modelMatrix *= Transform::Rotate(wormPosition, boundingBoxPosition, 0.f, wormAngle, 0.f);
			modelMatrix *= Transform::Scale(boundingBoxScale, boundingBoxScale, boundingBoxScale);
			RenderSimpleMesh(meshes["sphere"], shaders["WormShader"], modelMatrix, glm::vec3(), mapTextures["glass"],
				worms[i]->getMaterialShininess(), worms[i]->getMaterialKd(), worms[i]->getMaterialKs(), i);
			glDisable(GL_BLEND);
		}
	}
}

// Render the projectile.
void Tema3::RenderProjectile(float deltaTimeSeconds)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	glm::vec3 position;
	float scale = projectile->getScale();

	if (!launchedProjectile)
	{
		glm::vec3 wormPosition = worms[currentWorm]->getWormPosition();
		glm::vec3 gunPosition = worms[currentWorm]->getGunPosition();
		position = gunPosition + glm::vec3(0.f, 0.f, worms[currentWorm]->getGunScale());

		modelMatrix *= Transform::Translate(position.x, position.y, position.z);
		modelMatrix *= Transform::Rotate(wormPosition, position, 0.f, worms[currentWorm]->getWormAngle(), 0.f);
		modelMatrix *= Transform::Rotate(gunPosition, position, -worms[currentWorm]->getGunAngle(), 0.f, 0.f);
		modelMatrix *= Transform::Scale(scale, scale, scale);
	}
	else
	{
		projectile->updatePosition(deltaTimeSeconds);
		position = projectile->getPosition();

		modelMatrix *= Transform::Translate(position.x, position.y, position.z);
		modelMatrix *= Transform::Scale(scale, scale, scale);

		if (currentWorm)
		{
			camera->Set(position + glm::vec3(2.f, 2.f, 2.f), position, glm::vec3(0, 1, 0));
		}
		else
		{
			camera->Set(position + glm::vec3(-2.f, 2.f, -2.f), position, glm::vec3(0, 1, 0));
		}

		CheckCollisions(position);
	}

	std::string textureID = currentWorm == 0 ? "glass" : "sun";

	RenderSimpleMesh(meshes["sphere"], shaders["BasicShader"], modelMatrix, glm::vec3(), mapTextures[textureID],
		projectile->getMaterialShininess(), projectile->getMaterialKd(), projectile->getMaterialKs(), -1);
}

// Checks collisions between the projectile and the players + the ground. You CANNNOT kill yourself.
void Tema3::CheckCollisions(glm::vec3& projectilePosition)
{
	for (int i = 0; i < wormCount; ++i)
	{
		if (i != currentWorm && CheckLoser(i, projectilePosition))
		{
			return;
		}
	}

	if (ground->Collide(projectilePosition, projectile->getScale() / 2))
	{
		mapTextures.erase("heightmap");
		mapTextures["heightmap"] = ground->CreateHeightMap();

		launchedProjectile = false;
		collided = true;

		return;
	}

	float halfSideLength = ground->getSideLength() / 2;
	if (projectilePosition.x * projectilePosition.x + projectilePosition.y * projectilePosition.y
			+ projectilePosition.z * projectilePosition.z > halfSideLength* halfSideLength)
	{
		launchedProjectile = false;
		collided = true;
	}
}

// Checks if the current player collided with the projectile and stops the game if a collision is detected.
bool Tema3::CheckLoser(int i, glm::vec3& projectilePosition)
{
	if (projectile->CollidedWithWorm(worms[i]->getBoundingBoxPosition(), worms[i]->getBoundingBoxScale()))
	{
		launchedProjectile = false;
		playing = false;
		std::cout << "==================== Game over! ====================" << std::endl;
		std::cout << "Player " << currentWorm << " won!" << std::endl;
		std::cout << "Player " << i << " lost!" << std::endl;
		std::cout << "====================================================" << std::endl;

		camera->Set(cameraPositions[currentWorm], glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

		return true;
	}

	return false;
}

void Tema3::FrameEnd()
{
	// DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema3::RenderImportedMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color,
	Texture2D* texture, unsigned int materialShininess, float materialKd, float materialKs, int wormId)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position.
	glUseProgram(shader->program);

	if (!strcmp(shader->GetName(), "GroundShader") || !strcmp(shader->GetName(), "WormShader")
		|| !strcmp(shader->GetName(), "BasicShader"))
	{
		int light_position = glGetUniformLocation(shader->program, "light_position");
		glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

		// Set eye position (camera position) uniform.
		glm::vec3 eyePosition = camera->position;
		int eye_position = glGetUniformLocation(shader->program, "eye_position");
		glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

		// Set material property uniforms (shininess, kd, ks, object color).
		int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
		glUniform1i(material_shininess, materialShininess);

		int material_kd = glGetUniformLocation(shader->program, "material_kd");
		glUniform1f(material_kd, materialKd);

		int material_ks = glGetUniformLocation(shader->program, "material_ks");
		glUniform1f(material_ks, materialKs);
	}


	if (!strcmp(shader->GetName(), "GroundShader") || !strcmp(shader->GetName(), "WormShader"))
	{
		// Set spotlight direction uniform.
		int spotlight_direction = glGetUniformLocation(shader->program, "spotlight_direction");
		glUniform3f(spotlight_direction, spotlightDirection.x, spotlightDirection.y, spotlightDirection.z);

		int cut_off = glGetUniformLocation(shader->program, "cut_off");
		glUniform1f(cut_off, cutOff);
	}

	if (!strcmp(shader->GetName(), "WormShader"))
	{
		// Set spotlight position uniform.
		glm::vec3 spotlightPosition = worms[wormId]->getWormPosition() + spotlightOffset;
		int spotlight_position = glGetUniformLocation(shader->program, "spotlight_position");
		glUniform3f(spotlight_position, spotlightPosition.x, spotlightPosition.y, spotlightPosition.z);
	}

	if (!strcmp(shader->GetName(), "GroundShader"))
	{
		// Set spotlight position uniform.
		glm::vec3 spotlight0Position = worms[0]->getWormPosition() + spotlightOffset;
		int spotlight0_position = glGetUniformLocation(shader->program, "spotlight0_position");
		glUniform3f(spotlight0_position, spotlight0Position.x, spotlight0Position.y, spotlight0Position.z);

		glm::vec3 spotlight1Position = worms[1]->getWormPosition() + spotlightOffset;
		int spotlight1_position = glGetUniformLocation(shader->program, "spotlight1_position");
		glUniform3f(spotlight1_position, spotlight1Position.x, spotlight1Position.y, spotlight1Position.z);

		int height_factor = glGetUniformLocation(shader->program, "height_factor");
		glUniform1f(height_factor, ground->getHeightFactor());
	}

	// Bind model matrix.
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix.
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix.
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture != NULL)
	{
		// Activate texture location 0.
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture1 ID.
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

		// Send texture uniform value.
		glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
	}

	if (!strcmp(shader->GetName(), "GroundShader"))
	{
		// activate texture location 2.
		glActiveTexture(GL_TEXTURE1);

		// Bind the texture2 ID.
		glBindTexture(GL_TEXTURE_2D, mapTextures["heightmap"]->GetTextureID());

		// Send texture uniform value.
		glUniform1i(glGetUniformLocation(shader->program, "heightMap"), 1);

		int loc_heightmap_size = glGetUniformLocation(shader->program, "heightMapSize");
		glUniform2f(loc_heightmap_size, (float)ground->getHeightMapWidth(), (float)ground->getHeightMapHeight());
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
	if (!launchedProjectile && playing)
	{
		// Move the camera only if MOUSE_RIGHT button is pressed.
		if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
		{
			float cameraSpeed = 2.0f;

			if (window->KeyHold(GLFW_KEY_W))
			{
				// Translate the camera forward.
				camera->TranslateForward(cameraSpeed * deltaTime);
			}

			if (window->KeyHold(GLFW_KEY_A))
			{
				// Translate the camera to the left.
				camera->TranslateRight(-cameraSpeed * deltaTime);
			}

			if (window->KeyHold(GLFW_KEY_S))
			{
				// Translate the camera backwards.
				camera->TranslateForward(-cameraSpeed * deltaTime);
			}

			if (window->KeyHold(GLFW_KEY_D))
			{
				// Translate the camera to the right.
				camera->TranslateRight(cameraSpeed * deltaTime);
			}

			if (window->KeyHold(GLFW_KEY_Q))
			{
				// Translate the camera down.
				camera->TranslateUpword(-cameraSpeed * deltaTime);
			}

			if (window->KeyHold(GLFW_KEY_E))
			{
				// Translate the camera up.
				camera->TranslateUpword(cameraSpeed * deltaTime);
			}

			if (window->KeyHold(GLFW_KEY_1))
			{
				// Increase the fov.
				fov += 3 * deltaTime * cameraSpeed;
				projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
			}

			if (window->KeyHold(GLFW_KEY_2))
			{
				// Decrease the fov.
				fov -= 3 * deltaTime * cameraSpeed;
				projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
			}
		}

		if (window->KeyHold(GLFW_KEY_LEFT))
		{
			worms[currentWorm]->updateWormAngle(deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_RIGHT))
		{
			worms[currentWorm]->updateWormAngle(-deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_UP) && worms[currentWorm]->getGunAngle() < M_PI / 2)
		{
			worms[currentWorm]->updateGunAngle(deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_DOWN) && worms[currentWorm]->getGunAngle() > -M_PI / 2)
		{
			worms[currentWorm]->updateGunAngle(-deltaTime);
		}
	}
}

void Tema3::OnKeyPress(int key, int mods)
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

	// Update the current ground texture.
	if (key == GLFW_KEY_G)
	{
		ground->updateCurrentTextureID();
	}
}

void Tema3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && !launchedProjectile && playing)
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			// Rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
			camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// Rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
			camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
		}

	}
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && playing)
	{
		launchedProjectile = true;

		float angleOX = worms[currentWorm]->getGunAngle();
		float angleOY = worms[currentWorm]->getWormAngle();
		glm::vec3 gunPosition = worms[currentWorm]->getGunPosition();
		glm::vec3 wormPosition = worms[currentWorm]->getWormPosition();

		projectile->launchProjectile(angleOX, angleOY, worms[currentWorm]->getGunScale(), gunPosition, wormPosition);
	}
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}
