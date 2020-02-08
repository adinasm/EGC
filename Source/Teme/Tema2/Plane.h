#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Plane
{
public:

	Plane();
	~Plane();

	glm::vec3 getBoundingBoxCoordinates();
	void setBoundingBoxCoordinates(glm::vec3& boundingBoxCoordinates);
	glm::vec3 getBoundingBoxScale();
	void setBoundingBoxScale(glm::vec3& boundingBoxScale);

	glm::vec3 getBodyCoordinates();
	void setBodyCoordinates(glm::vec3& bodyCoordinates);
	glm::vec3 getWindowCoordinates();
	void setWindowCoordinates(glm::vec3& windowCoordinates);
	glm::vec3 getWindowScale();
	void setWindowScale(glm::vec3& windowScale);
	glm::vec3 getCabinCoordinates();
	void setCabinCoordinates(glm::vec3& cabinCoordinates);
	glm::vec3 getCabinScale();
	void setCabinScale(glm::vec3& cabinScale);
	glm::vec3 getFrontCoordinates();
	void setFrontCoordinates(glm::vec3& frontCoordinates);
	glm::vec3 getFrontScale();
	void setFrontScale(glm::vec3& frontScale);
	glm::vec3 getTailCoordinates();
	void setTailCoordinates(glm::vec3& tailCoordinates);
	glm::vec3 getTrunkCoordinates();
	void setTrunkCoordinates(glm::vec3& trunkCoordinates);
	glm::vec3 getPropCoordinates();
	void setPropCoordinates(glm::vec3& propCoordinates);
	glm::vec3 getMountCoordinates();
	void setMountCoordinates(glm::vec3& mountCoordinates);
	glm::vec3 getLeftWingCoordinates();
	void setLeftWingCoordinates(glm::vec3& leftWingCoordinates);
	glm::vec3 getRightWingCoordinates();
	void setRightWingCoordinates(glm::vec3& rightWingCoordinates);
	glm::vec3 getNeckCoordinates();
	void setNeckCoordinates(glm::vec3& neckCoordinates);
	glm::vec3 getHeadCoordinates();
	void setHeadCoordinates(glm::vec3& headCoordinates);
	glm::vec3 getBodyScale();
	void setBodyScale(glm::vec3& bodyScale);
	glm::vec3 getTailScale();
	void setTailScale(glm::vec3& tailScale);
	glm::vec3 getTrunkScale();
	void setTrunkScale(glm::vec3& trunkScale);
	glm::vec3 getPropScale();
	void setPropScale(glm::vec3& propScale);
	glm::vec3 getMountScale();
	void setMountScale(glm::vec3& mountScale);
	glm::vec3 getLeftWingScale();
	void setLeftWingScale(glm::vec3& leftWingScale);
	glm::vec3 getRightWingScale();
	void setRightWingScale(glm::vec3& rightWingScale);
	glm::vec3 getNeckScale();
	void setNeckScale(glm::vec3& neckScale);
	glm::vec3 getHeadScale();
	void setHeadScale(glm::vec3& headScale);
	glm::vec3 getHeadlightsCoordinates();
	void setHeadlightsCoordinates(glm::vec3& headlightsCoordinates);
	glm::vec3 getHeadlightsScale();
	void setHeadlightsScale(glm::vec3& headlightsScale);
	glm::vec3 getLeftWheelCoordinates();
	void setLeftWheelCoordinatess(glm::vec3& leftWheelCoordinates);
	glm::vec3 getLeftWheelScale();
	void setLeftWheelScale(glm::vec3& leftWheelScale);
	glm::vec3 getRightWheelCoordinates();
	void setRightWheelCoordinates(glm::vec3& rightWheelCoordinates);
	glm::vec3 getRightWheelScale();
	void setRightWheelScale(glm::vec3& rightWheelScale);
	glm::vec3 getLeftPillarCoordinates();
	void setLeftPillarCoordinates(glm::vec3& leftPillarCoordinates);
	glm::vec3 getLeftPillarScale();
	void setLeftPillarScale(glm::vec3& leftPillarScale);
	glm::vec3 getRightPillarCoordinates();
	void setRightPillarCoordinates(glm::vec3& rightPillarCoordinates);
	glm::vec3 getRightPillarScale();
	void setRightPillarScale(glm::vec3& rightPillarScale);

	glm::mat4 rotateAroundBody(glm::vec3 currentCoordinates, float angularStep);

private:
	glm::vec3 bodyCoordinates, trunkCoordinates, tailCoordinates, propCoordinates, mountCoordinates;
	glm::vec3 leftWingCoordinates, rightWingCoordinates, windowCoordinates;
	glm::vec3 leftWheelCoordinates, rightWheelCoordinates, leftPillarCoordinates, rightPillarCoordinates;
	glm::vec3 neckCoordinates, headCoordinates, cabinCoordinates, frontCoordinates;

	glm::vec3 bodyScale, trunkScale, tailScale, propScale, mountScale;
	glm::vec3 leftWingScale, rightWingScale, windowScale;
	glm::vec3 leftWheelScale, rightWheelScale, leftPillarScale, rightPillarScale;
	glm::vec3 neckScale, headScale, cabinScale, frontScale;

	glm::vec3 headlightsCoordinates, headlightsScale;

	glm::vec3 boundingBoxCoordinates, boundingBoxScale;
};
