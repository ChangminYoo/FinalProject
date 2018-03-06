#pragma once
#include"d3dUtil.h"
using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

using namespace std;
class CMesh;
class CGameObject;


struct Joint {
	std::wstring name;//조인트이름
	int parentID;//조인트 부모인덱스
	XMFLOAT3 pos;//조인트의 위치
	XMFLOAT4 orientation;//방향
};

//쉐이더에 연결하기 위한용. Joint의 name파트를 뺐다.
struct ShaderJoint
{
	XMFLOAT4 orientation;//방향
	int parentID;//조인트 부모인덱스
	XMFLOAT3 pos;//조인트의 위치

};


struct Weight {
	int jointID;//조인트 인덱스 , 아이디
	float bias;//가중치 값
	XMFLOAT3 pos;//웨이트의 위치
	XMFLOAT3 normal;//노멀
};

typedef struct BoundingBox
{
	XMFLOAT3 min;
	XMFLOAT3 max;
}Boundingbox;


struct FrameData 
{ 
	int frameID; std::vector<float> frameData; 
}; 
struct AnimJointInfo { 
	std::wstring name; 
	int parentID; 
	int flags; 
	int startIndex; 
}; 
struct ModelAnimation 
{ 
	int numFrames; 
	int numJoints; 
	int frameRate; 
	int numAnimatedComponents; 
	float frameTime; 
	float totalAnimTime; 
	float currAnimTime; 
	std::vector<AnimJointInfo> jointInfo; 
	std::vector<Joint> baseFrameJoints; 
	std::vector<FrameData> frameData; 
	std::vector<std::vector<Joint>> frameSkeleton; 
};


bool LoadMD5Model(wstring filename, CMesh* MD5Model,  int numberofMesh=0, float scale=1,float Spec=1);//메시는 1개만 생성되는게 일반적이지만 다수일경우 이녀석을 통해 해결하자 0번메쉬 1번메쉬 등 numberofMesh 정보를 얻을수잇음

bool LoadMD5Anim(ID3D12Device* m_Device,std::wstring filename, CMesh* MD5Model, CGameObject* obj,vector<ModelAnimation>& av,int numberofMesh=0);
void UpdateMD5Model(ID3D12GraphicsCommandList* m_DC,CMesh* MD5Model, CGameObject* obj, float deltaTime, int animation,vector<ModelAnimation>& av);
