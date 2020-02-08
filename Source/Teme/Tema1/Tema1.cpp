#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Objects2D.h"

Tema1::Tema1()
{
	angularStep			= 0.f;
	translateX			= 0.f;
	translateY			= 0.f;
	acceleration		= -3500.f;
	jumpAcceleration	= 10000.f;

	maxJumpY			= 30.f;
	currentJumpY		= 0.f;
	totalJumpY			= 0.f;
	jumping				= false;

	textureCount		= 5;

	playing				= true;
	score				= 0;
	scoreboardX			= 0.f;
	scoreboardY			 = 0.f;
	first				= true;

	maxFloatAngle		= (float)M_PI / 8;
	minFloatAngle		= -(float)M_PI / 8;
	floatStep			= 0.0f;
	floatUp				= true;

	wingScaleY			= 1.f;
	maxWingScaleY		= 2.f;
	minWingScaleY		= -2.f;
	wingUp				= true;

	obstacleScale		= 0.f;
	maxObstacleScale    = 1.f;
	minObstacleScale    = -1.f;
	growObstacle		= true;
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	acceleration		= -(float)resolution.y * 5;
	jumpAcceleration	= (float)resolution.y * 14;
	maxJumpY			= (float)resolution.y / 21;

	scoreboardX			= (float)resolution.x / 4;
	scoreboardY			= (float)resolution.y / 4;

	// Create meshes for obstacles, digits and the bird.
	obstacles.initializeObstacles(meshes, (float)resolution.x, (float)resolution.y, textureCount);
	digits.initializeDigits(meshes);
	bird.createBird(meshes);

	createTextures();

	// Create mesh for the scoreboard.
	Mesh* scoreboard = Objects2D::CreateScoreboard("scoreboard", glm::vec3(0.f, 0.f, 0.f), 100.f,
		glm::vec3(0.078431f, 0.149019, 0.223529));
	meshes[scoreboard->GetMeshID()] = scoreboard;
}

// Loads the textures and creates the shader for the obstacles.
void Tema1::createTextures()
{
	const std::string textureLoc = "Source/Teme/Tema1/Textures/";

	// Load textures.

	for (int i = 0; i < textureCount; ++i)
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "crystal" + std::to_string(i) + ".jpg").c_str(), GL_REPEAT);
		mapTextures["crystal" + std::to_string(i)] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "background.jpg").c_str(), GL_REPEAT);
		mapTextures["background"] = texture;
	}

	// Create a shader program.
	{
		Shader* shader = new Shader("ShaderT1");
		shader->AddShader("Source/Teme/Tema1/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema1/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.607843f, 0.607843f, 0.607843f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	// Does not apply any transformations on the objects from the first frame.
	if (first)
	{
		first = false;
		deltaTimeSeconds = 0.f;
	}

	// Prints the score if the player lost the game.
	if (!playing)
	{
		printScore();
	}

	// Renders the obstacles and the bird.
	updateStalactitesAndStalagmites(deltaTimeSeconds);
	updateBird(deltaTimeSeconds);

	// Renders the background.
	glm::mat3 mm = Transform2D::Scale((float)window->GetResolution().x / 100.f, (float)window->GetResolution().y / 100.f);

	glm::mat4 model = glm::mat4(
		mm[0][0], mm[0][1], mm[0][2], 0.f,
		mm[1][0], mm[1][1], mm[1][2], 0.f,
		0.f, 0.f, mm[2][2], 0.f,
		mm[2][0], mm[2][1], 0.f, 1.f);

	RenderSimpleMesh(meshes["square"], shaders["ShaderT1"], model, mapTextures["background"]);
}

// Prints the score on the scoreboard.
void Tema1::printScore() {
	glm::ivec2 resolution = window->GetResolution();

	int c = score;
	modelMatrix = Transform2D::Translate(scoreboardX * 3, scoreboardY * 2);
	modelMatrix *= Transform2D::Scale(10.f, 10.f);

	if (!c)
	{
		modelMatrix *= Transform2D::Translate(-10.f, 0.f);
		RenderMesh2D(meshes[std::to_string(c)], shaders["VertexColor"], modelMatrix);
	}

	while (c)
	{
		modelMatrix *= Transform2D::Translate(-10.f, 0.f);
		RenderMesh2D(meshes[std::to_string(c % 10)], shaders["VertexColor"], modelMatrix);
		c /= 10;
	}

	float side = obstacles.getSquareSide();

	modelMatrix = Transform2D::Translate(scoreboardX, scoreboardY);
	modelMatrix *= Transform2D::Scale(2.f * scoreboardX / side, 2.f * scoreboardY / side);
	
	RenderMesh2D(meshes["scoreboard"], shaders["VertexColor"], modelMatrix);
}

// Updates the position of the bird, flaps its wing and moves its tail.
void Tema1::updateBird(float deltaTimeSeconds)
{
	updateBody();
	updateHead();

	if (playing)
	{
		translateY += acceleration * deltaTimeSeconds * deltaTimeSeconds / 2.f + currentJumpY;
		angularStep += acceleration * deltaTimeSeconds / 10000.f;

		updateJump(deltaTimeSeconds);
		moveTail(deltaTimeSeconds);
		moveWing(deltaTimeSeconds);
	}
}

// Updates the position of the body, rotates the tail around the body and flaps the wing.
void Tema1::updateBody()
{
	for (int i = -1; i < 2; ++i)
	{
		updateTailComponent("featherCircle", bird.getCircleCoordinates(), i);
		updateTailComponent("feather", bird.getTailCoordinates(), i);
	}

	updateBirdComponent("wing", bird.getWingCoordinates());
	updateBirdComponent("body", bird.getBodyCoordinates());
}

// Updates the positions of the head and its components.
void Tema1::updateHead()
{
	updateBirdComponent("headDecoration", bird.getHeadDecorationCoordinates());
	updateBirdComponent("iris", bird.getEyeCoordinates());
	updateBirdComponent("eyeBall", bird.getEyeCoordinates());
	updateBirdComponent("beak", bird.getBeakCoordinates());
	updateBirdComponent("head", bird.getHeadCoordinates());
}

// Updates the position of a tail component and rotates it around the body of the bird.
void Tema1::updateTailComponent(std::string name, glm::vec3 currentCoordinates, int index)
{
	modelMatrix = Transform2D::Translate(currentCoordinates.x, currentCoordinates.y + translateY);
	modelMatrix *= Transform2D::Translate(bird.getBodyCoordinates().x - currentCoordinates.x, 0.f);
	modelMatrix *= Transform2D::Rotate(index * (float)M_PI / 12 + floatStep);
	modelMatrix *= Transform2D::Translate(currentCoordinates.x - bird.getBodyCoordinates().x, 0.f);
	modelMatrix *= bird.rotateAroundBody(currentCoordinates, angularStep);
	RenderMesh(meshes[name], shaders["VertexColor"], modelMatrix);
}

// Updates the position of the given bird component. In case the component is the wing, it is scaled
// in order to mimic the flapping of a wing.
void Tema1::updateBirdComponent(std::string name, glm::vec3 currentCoordinates)
{
	modelMatrix = Transform2D::Translate(currentCoordinates.x, currentCoordinates.y + translateY);

	if (name == "wing")
	{
		float offsetX = bird.getBodyCoordinates().x - currentCoordinates.x;
		float offsetY = bird.getBodyCoordinates().y - currentCoordinates.y;
		modelMatrix *= Transform2D::Translate(offsetX, offsetY);
		modelMatrix *= Transform2D::Rotate(angularStep);
		modelMatrix *= Transform2D::Scale(1.f, wingScaleY);
		modelMatrix *= Transform2D::Translate(-offsetX, -offsetY);
	}
	else
	{
		modelMatrix *= bird.rotateAroundBody(currentCoordinates, angularStep);
	}

	if (name == "body")
	{
		bodyCoordinates = modelMatrix[2];
	}
	else if (name == "head")
	{
		headCoordinates = modelMatrix[2];
	}

	RenderMesh(meshes[name], shaders["VertexColor"], modelMatrix);

	// Loses the game if the bird is no longer on the screen.
	if (modelMatrix[2].y < 0.f || modelMatrix[2].y >(float)window->GetResolution().y)
	{
		playing = false;
	}
}

// Computes the position on the OY axis if the bird is jumping, as well as the rotation of the bird
// around its center, based on the jump acceleration.
void Tema1::updateJump(float deltaTimeSeconds)
{
	if (jumping)
	{
		currentJumpY = jumpAcceleration * deltaTimeSeconds * deltaTimeSeconds / 2;
		angularStep += jumpAcceleration * deltaTimeSeconds / 10000;
		totalJumpY += currentJumpY;

		if (totalJumpY >= maxJumpY)
		{
			jumping = false;
			currentJumpY = 0.f;
			totalJumpY = 0.f;
		}
	}
	
	if (angularStep > M_PI / 4)
	{
		angularStep = (float)M_PI / 4;
	}
	
	if (angularStep < -M_PI / 4)
	{
		angularStep = -(float)M_PI / 4;
	}
}

// Rotates the tail around the body.
void Tema1::moveTail(float deltaTimeSeconds)
{
	if (floatUp)
	{
		floatStep -= deltaTimeSeconds / 2;

		if (floatStep < minFloatAngle)
		{
			floatUp = false;
		}
	}
	else
	{
		floatStep += deltaTimeSeconds / 2;

		if (floatStep > maxFloatAngle)
		{
			floatUp = true;
		}
	}
}

// Flaps the wing by scaling it on the OY axis.
void Tema1::moveWing(float deltaTimeSeconds) {
	if (wingUp)
	{
		wingScaleY += 2 * deltaTimeSeconds;

		if (wingScaleY >= maxWingScaleY)
		{
			wingUp = false;
		}
	}
	else
	{
		wingScaleY -= 2 * deltaTimeSeconds;

		if (wingScaleY <= minWingScaleY)
		{
			wingUp = true;
		}
	}
}


// Modifies the scales of all the obstacles.
void Tema1::scaleObstacle(float deltaTimeSeconds)
{
	if (growObstacle)
	{
		obstacleScale += deltaTimeSeconds / 2;

		if (obstacleScale >= maxObstacleScale)
		{
			growObstacle = false;
		}
	}
	else
	{
		obstacleScale -= deltaTimeSeconds / 2;

		if (obstacleScale <= minObstacleScale)
		{
			growObstacle = true;
		}
	}
}

// Renders the obstacles and updates the score.
void Tema1::updateStalactitesAndStalagmites(float deltaTimeSeconds)
{
	if (playing)
	{
		translateX -= deltaTimeSeconds / 2;

		if (score > 30)
		{
			scaleObstacle(deltaTimeSeconds);
		}

		// The score is incremented when an obstacle's OX position is less than 0.
		if (obstacles.updateFirst(translateX))
		{
			++score;
			std::cout << "Current score: " << score << "!\n";
		}
	}

	glm::mat3 stalagmitePosition = obstacles.getPosition();
	glm::mat3 stalactitePosition;

	for (int i = 0; i < obstacles.getObstacleCount(); ++i)
	{
		int currentIndex = (i + obstacles.getFirst()) % obstacles.getObstacleCount();

		// The position of the current stalagmite and stalactite is computed based on the
		// current index and the position of the leftmost obstacle.
		obstacles.createObstacles(currentIndex, stalagmitePosition, stalactitePosition, obstacleScale);

		// Renders only the obstacles that can be seen.
		if (stalagmitePosition[2][0] < (float)window->GetResolution().x)
		{
			glm::mat3 currentStalagmite = stalagmitePosition;
			currentStalagmite *= Transform2D::Scale(1.f, obstacles.getScale(currentIndex) + obstacleScale);

			RenderObstacle(currentStalagmite, currentIndex);
			RenderObstacle(stalactitePosition, currentIndex);

			// Checks for existing collisions.
			if (playing)
			{
				checkCollision(currentStalagmite[2], stalactitePosition[2], currentIndex);
			}
		}
	}
}

// Renders an obstacle using the randomly assigned texture.
void Tema1::RenderObstacle(glm::mat3& mm, int index)
{
	glm::mat4 model = glm::mat4(
		mm[0][0], mm[0][1], mm[0][2], 0.f,
		mm[1][0], mm[1][1], mm[1][2], 0.f,
		0.f, 0.f, mm[2][2], 0.f,
		mm[2][0], mm[2][1], 0.f, 1.f);

	RenderSimpleMesh(meshes["square"], shaders["ShaderT1"], model,
		mapTextures[obstacles.getObstacleTexture(index)]);
}

// If the bird is situated near the current obstacles, then collision checking is performed.
void Tema1::checkCollision(glm::vec3& stalagmite, glm::vec3& stalactite, int currentIndex)
{
	float headRadius = bird.getHeadRadius();
	float bodyRadius = bird.getBodyRadius();

	bool headCollision = false;
	bool bodyCollision = false;

	if (headCoordinates.x + headRadius >= stalagmite.x
		&& headCoordinates.x - headRadius <= stalagmite.x + obstacles.getSquareSide())
	{
		headCollision = obstacles.checkCollisions(headCoordinates, headRadius,
			currentIndex, stalactite, stalagmite, obstacleScale);
	}

	if (bodyCoordinates.x + bodyRadius >= stalagmite.x
		&& bodyCoordinates.x - bodyRadius <= stalagmite.x + obstacles.getSquareSide())
	{
		bodyCollision = obstacles.checkCollisions(bodyCoordinates, bodyRadius,
			currentIndex, stalactite, stalagmite, obstacleScale);
	}

	if (headCollision || bodyCollision)
	{
		playing = false;
	}
}

void Tema1::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture)
	{
		// Activate texture location 0
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture1 ID
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

		// Send texture uniform value
		glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	// The bird starts jumping when the spacebar is pressed.
	if (window->KeyHold(GLFW_KEY_SPACE)) {
		jumping = true;
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
