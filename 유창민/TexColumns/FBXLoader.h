#pragma once

//==============================================================================
//
//==============================================================================
#include <d3d12.h>
#include <cstdio>
#include <fbxsdk.h>
#include <vector>
#include <map>
#include "FrameResource.h"
#include "MathHelper.h"


//==============================================================================
// ����
//==============================================================================
// ����������


//���͸��� ������
struct MaterialData
{
	DirectX::XMFLOAT3 Emissive;
	DirectX::XMFLOAT3 Ambient;
	DirectX::XMFLOAT3 Diffuse;
	DirectX::XMFLOAT3 Specular;
	float Shininess;
};

struct Keyframe 
{
	FbxLongLong mFrameNum;
	FbxAMatrix mGlobalTransform;
	Keyframe* mNext;

	Keyframe() : mNext(nullptr)
	{}
};

struct Joint
{
	int mParentIndex;
	const char* mName;
	FbxAMatrix mGlobalBindposeInverse;
	Keyframe* mAnimation;
	FbxNode *mNode;

	Joint() :
		mNode(nullptr),
		mAnimation(nullptr)
	{
		mGlobalBindposeInverse.SetIdentity();
		mParentIndex = -1;
	}

	~Joint()
	{
		while (mAnimation)
		{
			Keyframe* temp = mAnimation->mNext;
			delete mAnimation;
			mAnimation = temp;
		}
	}

};

struct Skeleton
{
	std::vector<Joint> mJoints;
};

//=================================================


//=================================================
// ���� ������ �迭
typedef std::vector<Vertex>	VertexDataArray;

//���͸��� ������ �迭
typedef std::vector<MaterialData> MatDataArray;
//==================================================

//==============================================================================
//�Լ�����
//==============================================================================

// FBX�����Ϳ��� ���������� ��ȯ
// 
//const char* filename = "../datas/humanoid.fbx";


bool LoadFBXConvertToVertexData(const char* filename, VertexDataArray& outVertexData, MatDataArray& outMatData);

//==============================================================================
// End Of File
//==============================================================================
