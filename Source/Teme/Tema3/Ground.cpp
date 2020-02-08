#include "Ground.h"

#include <Core/Engine.h>
#include <Teme\Tema3\Objects.h>
#include <stb/stb_image.h>
#include <iostream>

Ground::Ground()
{
	currentTextureID = 0;

	sideLength = 20.f;
	materialShininess = 50;
	materialKd = 0.5;
	materialKs = 0.5;

	heightMapHeight = 0;
	heightMapWidth = 0;
	heightFactor = 2.f;

	collisionRadius = 10;

	heightPixels = NULL;

	channels = 1;
	std::string textureLoc = "Source/Teme/Tema3/Textures/";

	heightPixels = stbi_load((textureLoc + "heightmap.png").c_str(), &heightMapWidth, &heightMapHeight, &channels,
		STBI_grey);
}

Ground::~Ground()
{
	SAFE_FREE_ARRAY(heightPixels);
}

float Ground::getSideLength()
{
	return sideLength;
}

int Ground::getHeightMapWidth()
{
	return heightMapWidth;
}


int Ground::getHeightMapHeight()
{
	return heightMapHeight;
}

unsigned int Ground::getMaterialShininess()
{
	return materialShininess;
}

float Ground::getMaterialKd()
{
	return materialKd;
}

float Ground::getMaterialKs()
{
	return materialKs;
}

float Ground::getHeightFactor()
{
	return heightFactor;
}

float Ground::getHeight(int index)
{
	return heightPixels[index];
}

Texture2D* Ground::getCurrentTexture()
{
	return mapTextures[textures[currentTextureID]];
}

void Ground::updateCurrentTextureID()
{
	currentTextureID = (++currentTextureID) % textures.size();
}

// Creates a square mesh for the ground, reads the heightmap and creates textures.
void Ground::CreateGround(float newSideLength, std::unordered_map<std::string, Mesh*>& meshes)
{
	const std::string textureLoc = "Source/Teme/Tema3/Textures/";

	{
		textures.push_back("sand");
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "sand.jpg").c_str(), GL_REPEAT);
		mapTextures["sand"] = texture;
	}

	{
		textures.push_back("sand1");
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "sand1.jpg").c_str(), GL_REPEAT);
		mapTextures["sand1"] = texture;
	}

	{
		textures.push_back("snow");
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "snow.jpg").c_str(), GL_REPEAT);
		mapTextures["snow"] = texture;
	}

	sideLength = newSideLength;
	Objects::CreateGround(sideLength, meshes);
}

// Creates a texture for the heightmap.
Texture2D* Ground::CreateHeightMap()
{
	GLuint textureID = 0;

	// Generate and bind the new texture ID.
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	CheckOpenGLError();

	// Use glTextImage2D to set the texture data.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, heightMapWidth, heightMapHeight, 0, GL_RED, GL_UNSIGNED_BYTE,
		(void*)heightPixels);

	// Generate texture mip-maps.
	glGenerateMipmap(GL_TEXTURE_2D);

	CheckOpenGLError();

	// Save the texture into a wrapper Texture2D class for using easier later during rendering phase.
	Texture2D* texture = new Texture2D();
	texture->Init(textureID, heightMapWidth, heightMapHeight, channels);

	return texture;
}

// Checks if the projectile collided with the ground and in case a collision is detected, the ground is deformed.
bool Ground::Collide(glm::vec3& projectilePosition, float projectileRadius)
{
	int x = (int)((projectilePosition.x + sideLength / 2) * heightMapWidth / sideLength);
	int z = (int)((projectilePosition.z + sideLength / 2) * heightMapHeight / sideLength);
	int radiusX = (int)(projectileRadius * heightMapWidth / sideLength);
	int radiusZ = (int)(projectileRadius * heightMapHeight / sideLength);
	float height;
	int index;

	for (int i = -radiusZ; i <= radiusZ; ++i)
	{
		for (int j = -radiusX; j <= radiusX; ++j)
		{
			index = ((z + i) * heightMapWidth + (x + j));

			if ((index >= heightMapHeight * heightMapWidth) || (index < 0))
			{
				continue;
			}

			height = heightFactor * heightPixels[index] / 255;

			if (height >= projectilePosition.y - projectileRadius / 2)
			{
				DeformGround(x, z);

				return true;
			}
		}
	}

	return false;
}

// Creates a crater around the collision point.
void Ground::DeformGround(int collisionX, int collisionZ)
{
	int distance;
	float collisionAttenuation, deltaY;
	int index;

	for (int i = -collisionRadius; i <= collisionRadius; ++i)
	{
		for (int j = -collisionRadius; j <= collisionRadius; ++j)
		{
			index = (collisionZ + i) * heightMapWidth + (collisionX + j);
			distance = sqrt(j * j + i * i);
			
			if ((distance > collisionRadius) || (index >= heightMapHeight * heightMapWidth)
				|| (index < 0))
			{
				continue;
			}

			deltaY = 50.f * (collisionRadius - distance) / distance;
			if (heightPixels[index] - deltaY < 0.f)
			{
				heightPixels[index] = 0;
			}
			else
			{
				heightPixels[index] -= deltaY;
			}
		}
	}
}