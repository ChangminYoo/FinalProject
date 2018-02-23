#ifndef CAMERA_H
#define CAMERA_H

#include"UploadBuffer.h"
#include "d3dUtil.h"

using namespace DirectX;

struct CameraData // camera CB
{
	XMFLOAT4X4 View = MathHelper::Identity4x4();
	XMFLOAT4X4 Proj = MathHelper::Identity4x4();
	XMFLOAT4X4 ViewProj = MathHelper::Identity4x4();

	XMFLOAT3 EyePos = { 0.0f, 10.0f, -150.0f };

	float AtToEye; //물체에서 부터 카메라까지의 거리

	XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };

	XMFLOAT3 Look = { 0,0,0 };
	XMFLOAT3 At  = { 0,0,0 };
	XMFLOAT3 Up  = { 0,1,0 };

	UploadBuffer<CameraData>* ConstBuffer = nullptr;	//월드행렬과 커스텀데이터를 저장하기위한 버퍼
};



class CCamera
{
public:
	CCamera();
	CCamera(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist, float clientWidth, float clientHeight);
	~CCamera();


	CameraData CamData;
	void UpdateConstantBuffer(ID3D12GraphicsCommandList* commandlist);

	// 월드에서 카메라 위치 세팅/겟
	XMVECTOR GetPosition()const;
	XMFLOAT3 GetPosition3f()const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const DirectX::XMFLOAT3& v);
	
	// 기본 카메라 벡터 겟
	XMVECTOR GetRight()const;
	XMFLOAT3 GetRight3f()const;
	XMVECTOR GetUp()const;
	XMFLOAT3 GetUp3f()const;
	XMVECTOR GetLook()const;
	XMFLOAT3 GetLook3f()const;

	// 프러스텀 정보
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;
	
	// Set frustum.
	void SetLens(float fovY, float aspect, float zn, float zf);

	// Define camera space via LookAt parameters.
	void LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
	void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

	// Get View/Proj matrices.
	XMMATRIX GetView()const;
	XMMATRIX GetProj()const;

	XMFLOAT4X4 GetView4x4f()const;
	XMFLOAT4X4 GetProj4x4f()const;

	// Strafe/Walk the camera a distance d.
	void Strafe(float d);
	void Walk(float d);

	// Rotate the camera.
	void Pitch(float angle);
	void RotateY(float angle);

	// After modifying camera position/orientation, call to rebuild the view matrix.
	void UpdateViewMatrix();

private:

	// Camera coordinate system with coordinates relative to world space.
	XMFLOAT3 mPosition = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 mRight = { 1.0f, 0.0f, 0.0f };
	XMFLOAT3 mUp = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 mLook = { 0.0f, 0.0f, 1.0f };

	// Cache frustum properties.
	float mNearZ = 0.0f;
	float mFarZ = 0.0f;
	float mAspect = 0.0f;
	float mFovY = 0.0f;
	float mNearWindowHeight = 0.0f;
	float mFarWindowHeight = 0.0f;

	bool mViewDirty = true;

	// Cache View/Proj matrices.
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();
};

#endif // CAMERA_H