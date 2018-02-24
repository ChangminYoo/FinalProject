#pragma once
#include"d3dUtil.h"
using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

using namespace std;
class CMesh;
class CGameObject;


struct Joint {
	std::wstring name;//����Ʈ�̸�
	int parentID;//����Ʈ �θ��ε���
	XMFLOAT3 pos;//����Ʈ�� ��ġ
	XMFLOAT4 orientation;//����
};

//���̴��� �����ϱ� ���ѿ�. Joint�� name��Ʈ�� ����.
struct ShaderJoint
{
	XMFLOAT4 orientation;//����
	int parentID;//����Ʈ �θ��ε���
	XMFLOAT3 pos;//����Ʈ�� ��ġ

};


struct Weight {
	int jointID;//����Ʈ �ε��� , ���̵�
	float bias;//����ġ ��
	XMFLOAT3 pos;//����Ʈ�� ��ġ
	XMFLOAT3 normal;//���
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


bool LoadMD5Model(wstring filename, CMesh* MD5Model,  int numberofMesh=0, float scale=1,float Spec=1);//�޽ô� 1���� �����Ǵ°� �Ϲ��������� �ټ��ϰ�� �̳༮�� ���� �ذ����� 0���޽� 1���޽� �� numberofMesh ������ ����������

bool LoadMD5Anim(ID3D12Device* m_Device,std::wstring filename, CMesh* MD5Model, CGameObject* obj,vector<ModelAnimation>& av,int numberofMesh=0);
void UpdateMD5Model(ID3D12GraphicsCommandList* m_DC,CMesh* MD5Model, CGameObject* obj, float deltaTime, int animation,vector<ModelAnimation>& av);
