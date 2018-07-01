#define MAXJOINT 65
#define LIGHT_MAX 10

TextureCube gCubeMap : register(t0);

Texture2D gDiffuseMap : register(t1);

// ���÷�
SamplerState gsamPointWrap        : register(s0);
SamplerState gsamPointClamp       : register(s1);
SamplerState gsamLinearWrap       : register(s2);
SamplerState gsamLinearClamp      : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);


float4 QuaternionMultiply(float4 Q1, float4 Q2)
{
	float4 q;
	q.x = (Q2.w * Q1.x) + (Q2.x * Q1.w) + (Q2.y * Q1.z) - (Q2.z * Q1.y);
	q.y = (Q2.w * Q1.y) - (Q2.x * Q1.z) + (Q2.y * Q1.w) + (Q2.z * Q1.x);
	q.z = (Q2.w * Q1.z) + (Q2.x * Q1.y) - (Q2.y * Q1.x) + (Q2.z * Q1.w);
	q.w = (Q2.w * Q1.w) - (Q2.x * Q1.x) - (Q2.y * Q1.y) - (Q2.z * Q1.z);


	return q;
}

//-----------------------------------------------
// ����ü ����
//-----------------------------------------------

//�ִϸ��̼� ó���� ����Ʈ. �̰ɷ� ����Ʈ������ ���� ����ؼ� ���� ���� ��ġ�� ����
struct Joint
{
	float4 orientation:Orient;//����
	int parentID : pID;//����Ʈ �θ��ε���
	float3 pos:Posj;//����Ʈ�� ��ġ
};

//����Ʈ ����ü - ������, ����, ��ġ
struct Light
{
	float4 DiffuseColor;
	float4 Direction;
	float3 Position;
	float SpecularPower;
};


//------------------------------------
// �������
//-------------------------------------
cbuffer ObjectData : register(b0)
{
	float4x4 gWorld;
	float Scale;
	float SpecularParamater;
	int isAnimation;
	float BlendValue;
	float4 CustomData1;
	float PTime;
	float3 Velocity;
	float4 TexClamp;
};

cbuffer JointArr : register(b1)//����Ʈ���� �迭. ����� 65���� �ִ� ����Ʈ
{
	Joint interpolatedSkeleton[MAXJOINT];

};

cbuffer CameraData :register(b2)
{
	float4x4 gView;
	float4x4 gProj;
	float4x4 gViewProj;
	float3 gEyePos;

}

cbuffer LightData : register(b3)
{
	Light gLights[LIGHT_MAX];
	float4 gAmbientLight;
	int nLights;//����Ʈ����

}

cbuffer MaterialData : register(b4)
{
	//float4   gDiffuseAlbedo;//MaterialData(���������)
	float    gRoughness;//MaterialData(���������)
	float3 CustomMaterialData;//Ŀ���ҿ뵥���� ������ ������۴� 16����Ʈ������.
	float4 Emissive;
}