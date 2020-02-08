#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include <Core/Engine.h>

class Ground
{
public:
	Ground();
	~Ground();

	float getSideLength();
	int getHeightMapHeight();
	int getHeightMapWidth();
	unsigned int getMaterialShininess();
	float getMaterialKd();
	float getMaterialKs();
	float getHeightFactor();
	float getHeight(int index);
	Texture2D* getCurrentTexture();
	void updateCurrentTextureID();

	void CreateGround(float newSideLength, std::unordered_map<std::string, Mesh*>& meshes);
	Texture2D* CreateHeightMap();

	bool Collide(glm::vec3& projectilePosition, float projectileRadius);
	void DeformGround(int collisionX, int collisionZ);

private:
	float sideLength;
	unsigned char* heightPixels;

	int heightMapHeight;
	int heightMapWidth;
	int channels;

	float heightFactor;

	int collisionRadius;

	float materialKd;
	float materialKs;
	unsigned int materialShininess;

	int currentTextureID;
	std::vector<std::string> textures;
	std::unordered_map<std::string, Texture2D*> mapTextures;
};
