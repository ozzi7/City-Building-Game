#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "model.h"

class SkinnedMesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();

	bool LoadMesh(const std::string& fileName);
	void Render(Shader shader);

	void PrecalculateBoneTransforms();
	void BindBoneTransform(float timeInSeconds, Shader* shader);

private:
#define NUM_BONES_PER_VERTEX 4

	void BoneTransform(float timeInSeconds, std::vector<glm::mat4>& Transforms);
	void PrecalculateStringLookupTable();

	struct BoneInfo
	{
		glm::mat4 BoneOffset;
		glm::mat4 FinalTransformation;

		BoneInfo()
		{
			BoneOffset = glm::mat4(0.0f);
			FinalTransformation = glm::mat4(0.0f);
		}
	};

	struct VertexBoneData
	{
		unsigned int IDs[NUM_BONES_PER_VERTEX]{};
		float Weights[NUM_BONES_PER_VERTEX]{};

		VertexBoneData()
		{
			Reset();
		}

		void Reset()
		{
			for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; ++i)
			{
				IDs[i] = 0;
				Weights[i] = 0;
			}
		}

		void AddBoneData(unsigned int BoneID, float Weight);
	};

	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, std::string NodeName);
	void ReadNodeHierarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int MeshIndex,
	              const aiMesh* paiMesh,
	              std::vector<glm::vec3>& Positions,
	              std::vector<glm::vec3>& Normals,
	              std::vector<glm::vec2>& TexCoords,
	              std::vector<VertexBoneData>& Bones,
	              std::vector<unsigned int>& Indices);
	void LoadBones(unsigned int MeshIndex, const aiMesh* paiMesh, std::vector<VertexBoneData>& Bones);
	bool InitMaterials(unsigned int meshIndex, const aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> SkinnedMesh::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
	void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

	enum VB_TYPES
	{
		INDEX_BUFFER,
		POS_VB,
		NORMAL_VB,
		TEXCOORD_VB,
		BONE_VB,
		NUM_VBs
	};

	GLuint m_VAO;
	GLuint m_Buffers[NUM_VBs]{};
	GLuint m_ColorBuffer{};
	std::vector<glm::vec3> Colors;

	struct MeshEntry
	{
		MeshEntry()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = INVALID_MATERIAL;
		}

		unsigned int NumIndices;
		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int MaterialIndex;
	};

	std::string directory;

	std::vector<MeshEntry> m_Entries;
	std::vector<std::vector<Texture>> m_Textures;

	std::vector<std::vector<glm::mat4>> Transforms;
	std::vector<std::string> boneNames;

	const int TRANSFORMS_PER_SECOND = 120;

	std::map<std::string, unsigned int> m_BoneMapping; // maps a bone name to its index
	unsigned int m_NumBones;
	std::vector<BoneInfo> m_BoneInfo;
	glm::mat4 m_GlobalInverseTransform;

	/* duration of the animation, can be changed if frames are not present in all interval */
	double animDuration{};

	const aiScene* m_pScene;
	Assimp::Importer m_Importer;
};
