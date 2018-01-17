
#pragma once

#include "Graphics.h"
#include "Material.h"
#include "BoundingVolume.h"
#include <vector>

using namespace graphics;

struct Vertex 
{
	XMFLOAT3 position;
	XMFLOAT3 aabbmin;
	XMFLOAT3 aabbmax;
	UINT skirt;
	XMFLOAT3 normal;
};

struct TerrainShaderConstants 
{
	float scale;
	float width;
	float depth;
	float base;

	TerrainShaderConstants(float s, float w, float d, float b) : scale(s), width(w), depth(d), base(b) {}
};

class Terrain 
{
public:
	Terrain(ResourceManager* rm, TerrainMaterial* mat, const char* fnHeightmap, const char* fnDisplacementMap);
	~Terrain();

	void Draw(ID3D12GraphicsCommandList* cmdList, bool Draw3D = true);
	void AttachTerrainResources(ID3D12GraphicsCommandList* cmdList, unsigned int srvDescTableIndexHeightMap,
		unsigned int srvDescTableIndexDisplacementMap, unsigned int cbvDescTableIndex);
	void AttachMaterialResources(ID3D12GraphicsCommandList* cmdList, unsigned int srvDescTableIndex);

	BoundingSphere GetBoundingSphere() { return m_BoundingSphere; }
	float GetHeightAtPoint(float x, float y);
	
private:
	void CreateMesh3D();
	void CreateVertexBuffer();	
	void CreateIndexBuffer();
	void CreateConstantBuffer();
	void LoadHeightMap(const char* fnHeightMap);
	void LoadDisplacementMap(const char* fnMap);
	XMFLOAT2 CalcZBounds(Vertex topLeft, Vertex bottomRight);
	void DeleteVertexAndIndexArrays();

	float GetHeightMapValueAtPoint(float x, float y);
	float GetDisplacementMapValueAtPoint(float x, float y);
	XMFLOAT3 CalculateNormalAtPoint(float x, float y);

	TerrainMaterial*			m_pMat;
	ResourceManager*			m_pResMgr;
	D3D12_VERTEX_BUFFER_VIEW	m_viewVertexBuffer;
	D3D12_INDEX_BUFFER_VIEW		m_viewIndexBuffer;
	D3D12_CPU_DESCRIPTOR_HANDLE m_hdlHeightMapSRV_CPU;
	D3D12_GPU_DESCRIPTOR_HANDLE m_hdlHeightMapSRV_GPU;
	D3D12_CPU_DESCRIPTOR_HANDLE m_hdlDisplacementMapSRV_CPU;
	D3D12_GPU_DESCRIPTOR_HANDLE m_hdlDisplacementMapSRV_GPU;
	D3D12_CPU_DESCRIPTOR_HANDLE m_hdlConstantsCBV_CPU;
	D3D12_GPU_DESCRIPTOR_HANDLE m_hdlConstantsCBV_GPU;
	unsigned char*				m_dataHeightMap;
	unsigned char*				m_dataDisplacementMap;
	unsigned int				m_wHeightMap;
	unsigned int				m_hHeightMap;
	unsigned int				m_wDisplacementMap;
	unsigned int				m_hDisplacementMap;
	float						m_hBase;
	unsigned long				m_numVertices;
	unsigned long				m_numIndices;
	float						m_scaleHeightMap;
	Vertex*						m_dataVertices;		// buffer to contain vertex array prior to upload.
	UINT*						m_dataIndices;		// buffer to contain index array prior to upload.
	TerrainShaderConstants*		m_pConstants;
	BoundingSphere				m_BoundingSphere;
};

