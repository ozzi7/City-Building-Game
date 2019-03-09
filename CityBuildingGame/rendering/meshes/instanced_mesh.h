#pragma once
#include "mesh.h"

class InstancedMesh: public Mesh
{
public:
	InstancedMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	// Bind instanced
	void Bind(Shader& shader, std::vector<glm::mat4>& models);

	// Render instanced
	void Draw(int instances);

private:
	unsigned int instanceVBO;
	void setupInstancedMesh();
};
