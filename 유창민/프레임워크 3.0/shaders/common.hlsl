#define MAXJOINT 65
#define LIGHT_MAX 10

TextureCube gCubeMap : register(t0);

Texture2D gDiffuseMap : register(t1);

// 샘플러
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
// 구조체 정의
//-----------------------------------------------

//애니메이션 처리용 조인트. 이걸로 웨이트포지션 들을 계산해서 실제 정점 위치가 계산됨
struct Joint
{
	float4 orientation:Orient;//방향
	int parentID : pID;//조인트 부모인덱스
	float3 pos:Posj;//조인트의 위치
};

//라이트 구조체 - 빛색상, 방향, 위치
struct Light
{
	float4 DiffuseColor;
	float4 Direction;
	float3 Position;
	float SpecularPower;
};


//------------------------------------
// 상수버퍼
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

cbuffer JointArr : register(b1)//조인트들의 배열. 현재는 65개가 최대 조인트
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
	int nLights;//라이트갯수

}

cbuffer MaterialData : register(b4)
{
	//float4   gDiffuseAlbedo;//MaterialData(상수버퍼임)
	float    gRoughness;//MaterialData(상수버퍼임)
	float3 CustomMaterialData;//커스텀용데이터 무조건 상수버퍼는 16바이트여야함.
	float4 Emissive;
}