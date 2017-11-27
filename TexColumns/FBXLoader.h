#pragma once

//==============================================================================
//
//==============================================================================
#include <DirectXMath.h>
#include <vector>

//==============================================================================
// 정의
//==============================================================================
// 정점데이터
typedef struct VertexData
{
	DirectX::XMFLOAT3 pos;		// 좌표
	DirectX::XMFLOAT4 normal;	// 노멀
	DirectX::XMFLOAT2 uv;		// UV좌표
	int index;

}VertexData;


//머터리얼 데이터
struct MaterialData
{
	DirectX::XMFLOAT3 Emissive;
	DirectX::XMFLOAT3 Ambient;
	DirectX::XMFLOAT3 Diffuse;
	DirectX::XMFLOAT3 Specular;
	float Shininess;
};


// 정점 데이터 배열
typedef std::vector<VertexData>	VertexDataArray;

//머터리얼 데이터 배열
typedef std::vector<MaterialData> MatDataArray;

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
