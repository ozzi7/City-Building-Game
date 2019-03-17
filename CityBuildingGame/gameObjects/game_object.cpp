#include "stdafx.h"
#pragma once
#include "game_object.h"
#include "event_handler.h"


/*TODO: problem with using pos.x for grid coordinate is that for buildings and larger objects it will be wrong*/
GameObject::GameObject(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
{
	position = aPosition;
	scale = aScale;
	rotation = aRotation;
	recalculateModelMatix();

	gridX = (int)position.x,
	gridY = (int)position.y;
}
GameObject::GameObject(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation, int aGridX, int aGridY)
{
	position = aPosition;
	scale = aScale;
	rotation = aRotation;
	recalculateModelMatix();

	gridX = aGridX;
	gridY = aGridY;
}

GameObject::GameObject() {};

void GameObject::recalculateModelMatix()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // TODO: 
	model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);
}