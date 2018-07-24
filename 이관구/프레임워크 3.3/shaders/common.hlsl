#define MAXJOINT 65
#define LIGHT_MAX 10

TextureCube gCubeMap : register(t0);

Texture2D gDiffuseMap : register(t1);

Texture2D gNormalMap : register(t2);


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
cbuffer JointArr : register(b0)//����Ʈ���� �迭. ����� 65���� �ִ� ����Ʈ
{
	Joint interpolatedSkeleton[MAXJOINT];

};
cbuffer ObjectData : register(b1)
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
	float4 gDiffuse;//MaterialData(���������)
	float  gRoughness;//MaterialData(���������)
	float3 CustomMaterialData;//Ŀ���ҿ뵥���� ������ ������۴� 16����Ʈ������.
	float4 Emissive;

}

float4 CalcDiffuseLight(float3 normal, float3 lightDirection, float4 lightColor, float lightIntensity)
{
	return saturate(dot(normal, -lightDirection)) * lightIntensity * lightColor;
}

float4 CalcSpecularLight(float3 normal, float3 lightDirection, float3 cameraDirection, float4 lightColor, float lightIntensity)
{
	float3 halfVector = normalize(-lightDirection + -cameraDirection);
	float specular = saturate(dot(halfVector, normal));

	float specularPower = 20;

	return lightIntensity * lightColor * pow(abs(specular), specularPower);
}
float lengthSquared(float3 v1)
{
	return v1.x * v1.x + v1.y * v1.y + v1.z * v1.z;
}