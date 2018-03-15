#ifndef CAMERA_H
#define CAMERA_H

#include"UploadBuffer.h"
#include "d3dUtil.h"

using namespace DirectX;

struct CameraData // camera CB
{
	
	XMFLOAT4X4 View;
	XMFLOAT4X4 Proj;
	XMFLOAT4X4 ViewProj;

	XMFLOAT3 EyePos = { 0.0f, 0.0f, 0.0f };

	float AtToEye; //��ü���� ���� ī�޶������ �Ÿ�

	XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };

	XMFLOAT3 Look = { 0,0,1 };
	XMFLOAT3 At  = { 0,0,0 };
	XMFLOAT3 Up  = { 0,1,0 };


};



class CCamera
{
public:
	CCamera();
	CCamera(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist,float asp,XMFLOAT3& e,XMFLOAT3& a,XMFLOAT3& u);
	~CCamera();


	CameraData CamData;
	void UpdateConstantBuffer(ID3D12GraphicsCommandList* commandlist);

	
	// Set frustum.
	void SetLens(float fovY, float aspect, float zn, float zf);

	XMFLOAT3 offEye;//�������� Eye��ġ
	XMFLOAT3 offAt;//���������� At��ġ
	
	UploadBuffer<CameraData>* ConstBuffer = nullptr;	//������İ� Ŀ���ҵ����͸� �����ϱ����� ����	
	float mNearZ = 0.0f;
	float mFarZ = 0.0f;
private:

	
	// Cache frustum properties.

	float mAspect = 0.0f;
	float mFovY = 0.0f;
	float aspect = 0;
	bool mViewDirty = true;
	float mNearWindowHeight;
	float	mFarWindowHeight;
};

#endif // CAMERA_H