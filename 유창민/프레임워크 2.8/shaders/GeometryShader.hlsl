#include "common.hlsl"

struct VertexIn
{
	float3 Pos : POSITION;

};

struct VertexOut
{
	float4 Pos : POSITION;
};

struct PSOUT
{
	float4 Color : SV_TARGET;
	float Depth : SV_DEPTH;
};

struct GeoOut
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXTURE;
	
};


VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.Pos = mul(float4(vin.Pos,1), gWorld);
	vout.Pos = mul(vout.Pos, gView);


	if (CustomData1.x == 2)//�Ѿ� ��ƼŬ
	{
		vout.Pos.x += cos(Velocity.x * PTime * 3.141592);
		vout.Pos.z += sin(Velocity.z * PTime * 3.141592);
	}


	return vout;

}

[maxvertexcount(4)]
void GS(point VertexOut gin[1], inout TriangleStream<GeoOut> triStream)
{

	float halfWidth;
	float halfHeight;										
	

	float4 v[4];
	for (int i = 0; i < 4; i++)
		v[i] = gin[0].Pos;

	//CustomData1.x : ��ƼŬ�� ��ü��
	if (CustomData1.x == 0)//������ ��, ���ӵ� ������ ũ�⸦ ���� ���.
	{
		halfWidth = 0.5f * Scale;
		halfHeight = 0.5f * Scale;

		v[0].x += -halfWidth;
		v[0].y  += -halfHeight;

		v[1].x += -halfWidth;
		v[1].y += halfHeight;

		v[2].x += halfWidth;
		v[2].y += -halfHeight;

		v[3].x += halfWidth;
		v[3].y += halfHeight;
	}
	else if (CustomData1.x == 1)//HP��
	{
		halfWidth = 0.5f * Scale;
		halfHeight = 0.1f * Scale * 0.7;
		v[0].x += -halfWidth;
		v[0].y  += -halfHeight;

		v[1].x += -halfWidth;
		v[1].y += halfHeight;
	
		v[2].x += halfWidth -2*halfWidth*(1-CustomData1.y);
		v[2].y += -halfHeight;

		v[3].x += halfWidth  -2*halfWidth *(1-CustomData1.y);
		v[3].y += halfHeight;


	}
	else if (CustomData1.x == 2)//�Ѿ� ��ƼŬ
	{
		halfWidth = 0.5f * Scale;
		halfHeight = 0.5f * Scale;
		
		

		v[0].x += -halfWidth;
		v[0].y  += -halfHeight;

		v[1].x += -halfWidth;
		v[1].y += halfHeight;

		v[2].x += halfWidth;
		v[2].y += -halfHeight;

		v[3].x += halfWidth;
		v[3].y += +halfHeight;


	}
	else if(CustomData1.x == 3) // ��ų ui
	{
		halfWidth = 0.5f * Scale;
		halfHeight = 0.5f * Scale*0.7;
		
		v[0].x += -halfWidth;
		v[0].y  += -halfHeight;

		v[1].x += -halfWidth;
		v[1].y += halfHeight;

		v[2].x += halfWidth;
		v[2].y += -halfHeight;

		v[3].x += halfWidth;
		v[3].y += halfHeight;

	}
	else if (CustomData1.x == 4)//Hp frame
	{
		halfWidth = 0.5f * Scale;
		halfHeight = 0.1f * Scale*0.7;

		v[0].x += -halfWidth;
		v[0].y += -halfHeight;

		v[1].x += -halfWidth;
		v[1].y += halfHeight;

		v[2].x += halfWidth;
		v[2].y += -halfHeight;

		v[3].x += halfWidth;
		v[3].y += +halfHeight;
	}

	
	

	float2 tex[4];
	tex[0] = float2(0.0f, 1.0f);
	tex[1] = float2(0.0f, 0.0f);
	tex[2] = float2(1.0f, 1.0f);
	tex[3] = float2(1.0f, 0.0f);


	GeoOut gout;
	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		gout.PosH = mul(v[i], gProj);
		gout.PosW = v[i].xyz;
		gout.Normal = float3(0,1,0);
		gout.Tex = tex[i];

		triStream.Append(gout);

	}
}


PSOUT PS(GeoOut pin) 
{
	float4 textureColor; //�ؽ��� ����
	float4 litColor;

	
	//�ؽ����� �⺻ ���� - ���÷��� ����Ͽ� �� ����
	textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.Tex);
	
	//�����׽�Ʈ ����
	clip(textureColor.a - 0.1f);

	PSOUT pout;
	pout.Color = textureColor;
	
	
	pout.Depth = -1;
	

	return pout;

}

float4 PS2(GeoOut pin) : SV_TARGET
{
	float4 textureColor; //�ؽ��� ����
	float4 litColor;


	//�ؽ����� �⺻ ���� - ���÷��� ����Ͽ� �� ����
	textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.Tex);

	//�����׽�Ʈ ����
	clip(textureColor.a - 0.1f);



	return textureColor;

}