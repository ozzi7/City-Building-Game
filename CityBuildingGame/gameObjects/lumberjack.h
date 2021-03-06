#pragma once
#include "bone_animated.h"
#include <math.h>
#include "visitor.h"
#include <grid.h>

class Lumberjack : public BoneAnimated
{
public:
	Lumberjack(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation);
	void Accept(Visitor& v) override;
	void SetLumberjackHut(LumberjackHut* aLumberjackHut);
	void GameStep() override;

	LumberjackHut* lumberjackHut{};

protected:
	int workTimeLeft = 0;
	bool soundPlayed = false;
};
