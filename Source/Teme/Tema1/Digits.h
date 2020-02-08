#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <unordered_map>
#include <Core/Engine.h>
#include <Teme/Tema1/Bird.h>


class Digits
{
public:
	Digits();
	~Digits();

	void initializeDigits(std::unordered_map<std::string, Mesh*>& meshes);
};
