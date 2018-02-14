#pragma once
#include"CMesh.h"
#include"UploadBuffer.h"
#include"Timer.h"
#include"CTexture.h"

struct ObjectData
{
	XMFLOAT4X4 WorldMatrix;
	float Scale;
	//XMFLOAT4 CustomData1;
	//XMFLOAT4 CustomData2;
};

class CGameObject
{
public:
	CGameObject();
	~CGameObject();
	ID3D12GraphicsCommandList* commandlist;//�׷���Ŀ�ǵ帮��Ʈ
protected:
	CMesh * Mesh=NULL;

	ObjectData ObjData;

	UploadBuffer<ObjectData>* ConstBuffer=NULL;	//������İ� Ŀ���ҵ����͸� �����ϱ����� ����

public:

	
	XMFLOAT4 CenterPos;//����

	XMFLOAT3 Lookvector;//�躤��. ������Ʈ�� �ٶ󺸰��ִ� ����.

	float ThetaY;	//Y��ȸ���� ����

	float currAnimTime = 0;//����ִϸ��̼ǽð�
	std::vector<ModelAnimation> animations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�
	int n_Animation = 0;//���° �ִϸ��̼��� ���ǰ�?
	
	int TickValue =0;//�ִϸ��̼� ȣ�� ������
	
	UploadBuffer<CVertex>* DynamicVertexBuffer=NULL;// (�ִϸ��̼ǿ�)

	CGameObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist,bool animation,XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	virtual void SetWorldMatrix();//�����Ʈ���� ����.
	
	virtual void Render(ID3D12GraphicsCommandList* commandlist);
	virtual void Tick(const GameTimer& gt);
	virtual void CreateConstBuffer(ID3D12Device* m_Device);
	virtual void UpdateConstBuffer(ID3D12GraphicsCommandList* commandlist);
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���

	void SetTexture(ID3D12GraphicsCommandList* commandlist);
	void LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandlist, string texturename, wstring FileName);
	
	unordered_map<string, unique_ptr<CTexture>> Textures; //���߿� ����ƽ����

	ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap = nullptr;
	
};

