#pragma once

//==============================================================================
//
//==============================================================================
#include <DirectXMath.h>
#include <vector>

//==============================================================================
// ����
//==============================================================================
// ����������
typedef struct VertexData
{
	DirectX::XMFLOAT3 pos;		// ��ǥ
	DirectX::XMFLOAT4 normal;	// ���
	DirectX::XMFLOAT2 uv;		// UV��ǥ
	int index;

}VertexData;


//���͸��� ������
struct MaterialData
{
	DirectX::XMFLOAT3 Emissive;
	DirectX::XMFLOAT3 Ambient;
	DirectX::XMFLOAT3 Diffuse;
	DirectX::XMFLOAT3 Specular;
	float Shininess;
};


// ���� ������ �迭
typedef std::vector<VertexData>	VertexDataArray;

//���͸��� ������ �迭
typedef std::vector<MaterialData> MatDataArray;

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
