#include "Camera.h"
#include"MMPE\MyMiniPysicsEngine.h"
using namespace DirectX;

CCamera::CCamera(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist,float asp, XMFLOAT3& e, XMFLOAT3& a, XMFLOAT3& u)
{
	
	aspect = asp;

	SetLens(0.25f*3.141592, aspect, 1.0f, 1000.0f);

	CamData.EyePos = e;
	CamData.At = a;
	CamData.Up = u;
	
	offEye = e;
	offAt = a;

	XMVECTOR Eye = XMLoadFloat3(&e);
	XMVECTOR At = XMLoadFloat3(&a);
	XMVECTOR Up = XMLoadFloat3(&u);

	CamData.AtToEye=FloatLength(At - Eye);
		

	auto tempView = XMMatrixLookAtLH(Eye, At, Up);
	XMStoreFloat4x4(&CamData.View, tempView);

	CamData.ConstBuffer = new UploadBuffer<CameraData>(Device, 1, true);
}

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
	if(CamData.ConstBuffer!=NULL)
		delete CamData.ConstBuffer;
}

void CCamera::UpdateConstantBuffer(ID3D12GraphicsCommandList* commandlist)
{
	CameraData tempCamData;

	XMVECTOR Eye = XMLoadFloat3(&CamData.EyePos);
	XMVECTOR At = XMLoadFloat3(&CamData.At);
	XMVECTOR Up = XMLoadFloat3(&CamData.Up);

	auto tempView = XMMatrixLookAtLH(Eye, At, Up);
	//매번 새로운 카메라 행렬 계산. 이는 카메라가 움직였을때 바뀌어야할 행렬을 갱신해줌
	XMStoreFloat4x4(&CamData.View, tempView);
	//마찬가지로 투영행렬도 갱신해줌. Resizing할때 해도되는데 카메라 행렬은 어처피 하나이므로 이거해줘도 성능무리XX
	SetLens(0.25f*3.141592, aspect, 1.0f, 1000.0f);

	//전치행렬화 해서 연결하기 위해 tempCamData를 이용한다.
	tempCamData = CamData;

	XMMATRIX view = XMLoadFloat4x4(&CamData.View);
	XMMATRIX proj = XMLoadFloat4x4(&CamData.Proj);

	XMMATRIX viewProj = view * proj;
	
	XMStoreFloat4x4(&tempCamData.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&tempCamData.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&tempCamData.ViewProj, XMMatrixTranspose(viewProj));
	tempCamData.EyePos = CamData.EyePos;

	CamData.ConstBuffer->CopyData(0, tempCamData);

	commandlist->SetGraphicsRootConstantBufferView(3, CamData.ConstBuffer->Resource()->GetGPUVirtualAddress());//월드행렬연결

}



void CCamera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf( 0.5f*mFovY );
	mFarWindowHeight  = 2.0f * mFarZ * tanf( 0.5f*mFovY );

	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&CamData.Proj, P);
}




