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
// 정의
//==============================================================================
// 정점데이터


//머터리얼 데이터
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
// 정점 데이터 배열
typedef std::vector<Vertex>	VertexDataArray;

//머터리얼 데이터 배열
typedef std::vector<MaterialData> MatDataArray;
//==================================================

//==============================================================================
//함수정의
//==============================================================================

// FBX데이터에서 정점데이터 변환
// 
//const char* filename = "../datas/humanoid.fbx";


bool LoadFBXConvertToVertexData(const char* filename, VertexDataArray& outVertexData, MatDataArray& outMatData);

//==============================================================================
// End Of File
//==============================================================================
