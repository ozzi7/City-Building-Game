#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

#include "globals.h"
#include "camera.h"
#include "terrain.h"
#include "shader.h"
#include "model.h"
#include "skinned_mesh.h"
#include "instanced_model.h"

#include "tree.h"
#include "lumberjack.h"
#include "triple_buffer.h"
#include <grid.h>

class Renderer
{
public:
	Camera& camera;

	SkinnedMesh* mesh_lumby_walk_w_axe;
	SkinnedMesh* mesh_lumby_walk_w_wood;
	SkinnedMesh* mesh_lumby_chopping;
	SkinnedMesh* mesh_worker;
	SkinnedMesh* mesh_wood_worker;
	Shader* terrain_shader;
	Shader* skinned_mesh_shader;
	Shader* instanced_mesh_shader;
	Shader* shadow_shader;
	Shader* shadow_instanced_shader;
	Shader* shadow_skinned_shader;
	InstancedModel* instanced_model_pine;
	InstancedModel* instanced_model_toona;
	InstancedModel* instanced_model_euroBeech;
	InstancedModel* instanced_model_euroBeech2;
	InstancedModel* instanced_model_oak;
	InstancedModel* instanced_model_dwelling;
	InstancedModel* instanced_model_lumberjack_hut;
	InstancedModel* instanced_model_building_outline;
	InstancedModel* instanced_model_wood;
	InstancedModel* instanced_model_wood_plank; 
	InstancedModel* instanced_model_grass;

	unsigned int ShadowDepthMap = 0;

	Renderer(Camera& aCamera) : camera(aCamera)
	{
		terrain_shader = new Shader("shaders/mesh.vert", "shaders/mesh.frag");
		skinned_mesh_shader = new Shader("shaders/mesh_skinned.vert", "shaders/mesh.frag");
		instanced_mesh_shader = new Shader("shaders/mesh_instanced.vert", "shaders/mesh.frag");

		shadow_shader = new Shader("shaders/shadow_depth.vert", "shaders/shadow_depth.frag");
		shadow_instanced_shader = new Shader("shaders/shadow_depth_instanced.vert", "shaders/shadow_depth.frag");
		shadow_skinned_shader = new Shader("shaders/shadow_depth_skinned.vert", "shaders/shadow_depth.frag");

		/* buildings */
		instanced_model_dwelling = new InstancedModel(Path + "/../models/buildings/dwelling/dwelling.dae");
		instanced_model_lumberjack_hut = new InstancedModel(
			Path + "/../models/buildings/lumberjack_hut/lumberjack_hut.dae");
		instanced_model_building_outline = new InstancedModel(
			Path + "/../models/buildings/building_outline/building_outline.dae");

		/* vegetation */
		instanced_model_pine = new InstancedModel(Path + "/../models/vegetation/pine/pine.dae");
		instanced_model_toona = new InstancedModel(Path + "/../models/vegetation/toona/toona.dae");
		instanced_model_euroBeech = new InstancedModel(Path + "/../models/vegetation/euro_beech/euro_beech.dae");
		instanced_model_euroBeech2 = new InstancedModel(Path + "/../models/vegetation/euro_beech/euro_beech2.dae");
		instanced_model_oak = new InstancedModel(Path + "/../models/vegetation/oak/oak.dae");
		instanced_model_grass = new InstancedModel(Path + "/../models/vegetation/grass/grass.dae");

		/* resources */
		instanced_model_wood = new InstancedModel(Path + "/../models/resources/wood/wood.dae");
		instanced_model_wood_plank = new InstancedModel(Path + "/../models/resources/stone_brick/stone_brick.fbx");
		//instanced_model_wood_plank = new InstancedModel(Path + "/../models/resources/wood_plank/wood_plank.dae");

		/* lumberjack init*/
		mesh_lumby_walk_w_axe = new SkinnedMesh();
		mesh_lumby_walk_w_axe->LoadMesh(Path + "/../models/people/lumberjack/walking_w_axe/lumberjack.dae");
		mesh_lumby_walk_w_axe->PrecalculateBoneTransforms();

		mesh_lumby_walk_w_wood = new SkinnedMesh();
		mesh_lumby_walk_w_wood->LoadMesh(Path + "/../models/people/lumberjack/walking_w_wood/lumberjack.dae");
		mesh_lumby_walk_w_wood->PrecalculateBoneTransforms();

		mesh_lumby_chopping = new SkinnedMesh();
		mesh_lumby_chopping->LoadMesh(Path + "/../models/people/lumberjack/chopping_wood/lumberjack.dae");
		mesh_lumby_chopping->PrecalculateBoneTransforms();

		mesh_worker = new SkinnedMesh();
		mesh_worker->LoadMesh(Path + "/../models/people/worker/walking.dae");
		mesh_worker->PrecalculateBoneTransforms();

		mesh_wood_worker = new SkinnedMesh();
		mesh_wood_worker->LoadMesh(Path + "/../models/people/worker/carrying_wood.dae");
		mesh_wood_worker->PrecalculateBoneTransforms();
	}

	void InitShadowMap()
	{
		glGenFramebuffers(1, &shadowDepthMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowDepthMapFBO);

		glGenTextures(1, &ShadowDepthMap);
		glActiveTexture(GL_TEXTURE0 + ShadowDepthMap);
		glBindTexture(GL_TEXTURE_2D, ShadowDepthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
		             nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowDepthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	void BindShadowMap() const
	{
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowDepthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void UnbindShadowMap()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, ScreenWidth, ScreenHeight);
	}
	void SetMatrices(glm::mat4 aProjection, glm::mat4 aView, glm::mat4 aLightSpaceMatrix, unsigned int shadowMapID) const
	{
		terrain_shader->use();
		terrain_shader->setMat4("projection", aProjection);
		terrain_shader->setMat4("view", aView);
		terrain_shader->setMat4("model", glm::mat4(1.0f));
		terrain_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);
		terrain_shader->setInt("shadowMap", shadowMapID);
		terrain_shader->setVec3("light.ambient", camera.AmbientLight);
		terrain_shader->setVec3("light.diffuse", camera.GetDirectionalLight().Color);
		terrain_shader->setVec3("light.direction", normalize(-camera.GetDirectionalLight().Direction));
		terrain_shader->setVec3("cameraPosition", camera.Position);
		terrain_shader->setVec2("texelSizeShadowMap", 1.0f / (float)SHADOW_WIDTH, 1.0f /(float)SHADOW_HEIGHT);

		skinned_mesh_shader->use();
		skinned_mesh_shader->setMat4("projection", aProjection);
		skinned_mesh_shader->setMat4("view", aView);
		skinned_mesh_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);
		skinned_mesh_shader->setInt("shadowMap", shadowMapID);
		skinned_mesh_shader->setVec3("light.ambient", camera.AmbientLight);
		skinned_mesh_shader->setVec3("light.diffuse", camera.GetDirectionalLight().Color);
		skinned_mesh_shader->setVec3("light.direction", normalize(-camera.GetDirectionalLight().Direction));
		skinned_mesh_shader->setVec3("cameraPosition", camera.Position);
		skinned_mesh_shader->setVec2("texelSizeShadowMap", 1.0f / (float)SHADOW_WIDTH, 1.0f /(float)SHADOW_HEIGHT);

		instanced_mesh_shader->use();
		instanced_mesh_shader->setMat4("projection", aProjection);
		instanced_mesh_shader->setMat4("view", aView);
		instanced_mesh_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);
		instanced_mesh_shader->setInt("shadowMap", shadowMapID);
		instanced_mesh_shader->setVec3("light.ambient", camera.AmbientLight);
		instanced_mesh_shader->setVec3("light.diffuse", camera.GetDirectionalLight().Color);
		instanced_mesh_shader->setVec3("light.direction", normalize(-camera.GetDirectionalLight().Direction));
		instanced_mesh_shader->setVec3("cameraPosition", camera.Position);
		instanced_mesh_shader->setVec2("texelSizeShadowMap", 1.0f / (float)SHADOW_WIDTH, 1.0f /(float)SHADOW_HEIGHT);
	}
	void SetShadowMatrices(glm::mat4 aLightSpaceMatrix) const
	{
		shadow_shader->use();
		shadow_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);

		shadow_instanced_shader->use();
		shadow_instanced_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);

		shadow_skinned_shader->use();
		shadow_skinned_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);
	}

	void CalculateShadow(RenderBuffer* renderBuffer)
	{
		// don't calculate terrain shadows
		shadowPass = true;
		renderInstancedObjects(renderBuffer);
		renderBoneAnimated(renderBuffer);
		shadowPass = false;
	}

	void Render(RenderBuffer* renderBuffer)
	{
		renderTerrain(renderBuffer);
		camera.CalculateCursorPositionOnGrid(); // TODO: where does it belong

		renderInstancedObjects(renderBuffer);
		renderBoneAnimated(renderBuffer);
	}

private:
	bool shadowPass = false;
	unsigned int shadowDepthMapFBO = 0;
	long long reloadGPUDataCounter = -1;

	void renderTerrain(RenderBuffer* renderBuffer) const
	{
		if (!renderBuffer->terrainVector.empty()) {
			terrain_shader->use();
			renderBuffer->terrainVector[0]->Draw(*terrain_shader);
		}
	}

	void renderInstancedObjects(RenderBuffer* renderBuffer)
	{
		Shader* shader;
		if (shadowPass)
			shader = shadow_instanced_shader;
		else
			shader = instanced_mesh_shader;
		shader->use();

		if (renderBuffer->reloadGPUDataCounter != reloadGPUDataCounter)
		{
			instanced_model_pine->Draw(*shader, renderBuffer->pineModels);
			instanced_model_oak->Draw(*shader, renderBuffer->oakModels);
			instanced_model_euroBeech->Draw(*shader, renderBuffer->euroBeechModels);
			instanced_model_euroBeech2->Draw(*shader, renderBuffer->euroBeech2Models);
			instanced_model_toona->Draw(*shader, renderBuffer->toonaModels);
			instanced_model_grass->Draw(*shader, renderBuffer->grassModels);
		}
		else
		{
			// this data is only binded but not sent to gpu again, we only resend it when the visible units changed (scrolled)
			instanced_model_pine->DrawOnly(*shader, renderBuffer->pineModels);
			instanced_model_oak->DrawOnly(*shader, renderBuffer->oakModels);
			instanced_model_euroBeech->DrawOnly(*shader, renderBuffer->euroBeechModels);
			instanced_model_euroBeech2->DrawOnly(*shader, renderBuffer->euroBeech2Models);
			instanced_model_toona->DrawOnly(*shader, renderBuffer->toonaModels);
			instanced_model_grass->DrawOnly(*shader, renderBuffer->grassModels);
		}
		instanced_model_lumberjack_hut->Draw(*shader, renderBuffer->lumberjackHutModels);
		instanced_model_dwelling->Draw(*shader, renderBuffer->dwellingModels_growth1);
		instanced_model_building_outline->Draw(*shader, renderBuffer->buildingOutlineModels);
		instanced_model_wood->Draw(*shader, renderBuffer->woodModels);
		instanced_model_wood_plank->Draw(*shader, renderBuffer->woodPlankModels);

		reloadGPUDataCounter = renderBuffer->reloadGPUDataCounter;
	}

	void renderBoneAnimated(RenderBuffer* renderBuffer) const
	{
		Shader* shader;
		if (shadowPass)
			shader = shadow_skinned_shader;
		else
			shader = skinned_mesh_shader;
		shader->use();

		for (int i = 0; i < renderBuffer->lumberjackModels.size(); ++i)
		{
			mesh_lumby_walk_w_axe->BindBoneTransform(renderBuffer->lumberjackAnimationSeconds[i], shader);

			shader->setMat4("model", renderBuffer->lumberjackModels[i]);
			mesh_lumby_walk_w_axe->Render(*shader);
		}
		for (int i = 0; i < renderBuffer->lumberjackWoodModels.size(); ++i)
		{
			mesh_lumby_walk_w_wood->BindBoneTransform(renderBuffer->lumberjackWoodAnimationSeconds[i], shader);

			shader->setMat4("model", renderBuffer->lumberjackWoodModels[i]);
			mesh_lumby_walk_w_wood->Render(*shader);
		}
		for (int i = 0; i < renderBuffer->lumberjackChoppingModels.size(); ++i)
		{
			mesh_lumby_chopping->BindBoneTransform(renderBuffer->lumberjackChoppingAnimationSeconds[i], shader);

			shader->setMat4("model", renderBuffer->lumberjackChoppingModels[i]);
			mesh_lumby_chopping->Render(*shader);
		}
		for (int i = 0; i < renderBuffer->workerModels.size(); ++i)
		{
			mesh_worker->BindBoneTransform(renderBuffer->workerAnimationSeconds[i], shader);

			shader->setMat4("model", renderBuffer->workerModels[i]);
			mesh_worker->Render(*shader);
		}
		for (int i = 0; i < renderBuffer->workerWoodModels.size(); ++i)
		{
			mesh_wood_worker->BindBoneTransform(renderBuffer->workerWoodAnimationSeconds[i], shader);

			shader->setMat4("model", renderBuffer->workerWoodModels[i]);
			mesh_wood_worker->Render(*shader);
		}
	}
};
