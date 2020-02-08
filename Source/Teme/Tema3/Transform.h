#pragma once

#include <include/glm.h>

namespace Transform
{
	// Translate matrix
	inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
	{
		// Implement translate matrix
		return glm::mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			translateX, translateY, translateZ, 1);
	}

	// Scale matrix
	inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
	{
		// Implement scale matrix
		return glm::mat4(scaleX, 0, 0, 0,
			0, scaleY, 0, 0,
			0, 0, scaleZ, 0,
			0, 0, 0, 1);
	}

	// Rotate matrix relative to the OZ axis
	inline glm::mat4 RotateOZ(float radians)
	{
		// Implement rotate matrix
		return glm::mat4(cos(radians), sin(radians), 0, 0,
			-sin(radians), cos(radians), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}

	// Rotate matrix relative to the OY axis
	inline glm::mat4 RotateOY(float radians)
	{
		// Implement rotate matrix
		return glm::mat4(cos(radians), 0, -sin(radians), 0,
			0, 1, 0, 0,
			sin(radians), 0, cos(radians), 0,
			0, 0, 0, 1);
	}

	// Rotate matrix relative to the OX axis
	inline glm::mat4 RotateOX(float radians)
	{
		// Implement rotate matrix
		return glm::mat4(1, 0, 0, 0,
			0, cos(radians), sin(radians), 0,
			0, -sin(radians), cos(radians), 0,
			0, 0, 0, 1);
	}

	// Generates the rotation matrix for a rotation around the given rotation center.
	inline glm::mat4 Rotate(glm::vec3& rotationCenter, glm::vec3& position, float angularStepX, float angularStepY,
		float angularStepZ)
	{
		glm::mat4 matrix;

		float offsetX = rotationCenter.x - position.x;
		float offsetY = rotationCenter.y - position.y;
		float offsetZ = rotationCenter.z - position.z;

		matrix = Transform::Translate(offsetX, offsetY, offsetZ);
		matrix *= Transform::RotateOX(angularStepX);
		matrix *= Transform::RotateOY(angularStepY);
		matrix *= Transform::RotateOZ(angularStepZ);
		matrix *= Transform::Translate(-offsetX, -offsetY, -offsetZ);

		return matrix;
	}
}
