#include "Camera.h"
#include"MMPE\MyMiniPysicsEngine.h"
using namespace DirectX;

CCamera::CCamera(HWND hwnd,ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist,float asp, XMFLOAT3& e, XMFLOAT3& a, XMFLOAT3& u)
{
	
	hWnd = hwnd;
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

	ConstBuffer = new UploadBuffer<CameraData>(Device, 1, true);
	ConstBufferOrtho = new UploadBuffer<CameraData>(Device, 1, true);
}

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
	if (ConstBuffer != NULL)
		delete ConstBuffer;
	if (ConstBufferOrtho != NULL)
		delete ConstBufferOrtho;
}

void CCamera::UpdateConstantBuffer(ID3D12GraphicsCommandList* commandlist)
{
	CameraData tempCamData;

	XMVECTOR Eye = XMLoadFloat3(&CamData.EyePos);
	XMVECTOR At = XMLoadFloat3(&CamData.At);
	XMVECTOR Up = XMLoadFloat3(&CamData.Up);

	auto tempView = XMMatrixLookAtLH(Eye, At, Up);
	//�Ź� ���ο� ī�޶� ��� ���. �̴� ī�޶� ���������� �ٲ����� ����� ��������
	XMStoreFloat4x4(&CamData.View, tempView);
	//���������� ������ĵ� ��������. Resizing�Ҷ� �ص��Ǵµ� ī�޶� ����� ��ó�� �ϳ��̹Ƿ� �̰����൵ ���ɹ���XX
	SetLens(0.25f*3.141592, aspect,mNearZ,mFarZ);

	//��ġ���ȭ �ؼ� �����ϱ� ���� tempCamData�� �̿��Ѵ�.
	tempCamData = CamData;

	XMMATRIX view = XMLoadFloat4x4(&CamData.View);
	XMMATRIX proj = XMLoadFloat4x4(&CamData.Proj);

	XMMATRIX viewProj = view * proj;
	
	XMStoreFloat4x4(&tempCamData.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&tempCamData.Proj, XMMatrixTranspose(proj));

	XMStoreFloat4x4(&tempCamData.ViewProj, XMMatrixTranspose(viewProj));
	

	ConstBuffer->CopyData(0, tempCamData);

	commandlist->SetGraphicsRootConstantBufferView(4, ConstBuffer->Resource()->GetGPUVirtualAddress());//������Ŀ���

}

void CCamera::UpdateConstantBufferOrtho(ID3D12GraphicsCommandList * commandlist)
{

	CameraData tempCamData;

	
	RECT rc;
	GetClientRect(hWnd, &rc);
	

	//ī�޶� ����� �������. ������ĵ� ������ķ� �����.
	XMStoreFloat4x4(&CamData.View, XMMatrixIdentity());

	//��������� ������������
	auto tempProj = XMMatrixOrthographicLH(rc.right - rc.left, rc.bottom - rc.top, 0, 1);

	XMStoreFloat4x4(&CamData.Proj, tempProj);

	//��ġ���ȭ �ؼ� �����ϱ� ���� tempCamData�� �̿��Ѵ�.
	tempCamData = CamData;

	XMMATRIX view = XMLoadFloat4x4(&CamData.View);
	XMMATRIX proj = XMLoadFloat4x4(&CamData.Proj);

	XMMATRIX viewProj = view * proj;

	XMStoreFloat4x4(&tempCamData.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&tempCamData.Proj, XMMatrixTranspose(proj));

	XMStoreFloat4x4(&tempCamData.ViewProj, XMMatrixTranspose(viewProj));


	ConstBufferOrtho->CopyData(0, tempCamData);

	commandlist->SetGraphicsRootConstantBufferView(4, ConstBufferOrtho->Resource()->GetGPUVirtualAddress());//ī�޶� ��ĵ� ����

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




