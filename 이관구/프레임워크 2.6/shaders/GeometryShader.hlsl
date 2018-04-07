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


	return vout;

}

[maxvertexcount(4)]
void GS(point VertexOut gin[1], inout TriangleStream<GeoOut> triStream)
{

	float halfWidth;
	float halfHeight;										
	

	if (CustomData1.x == 0)//정체성 0:데미지등, 1: hp바
	{
		halfWidth = 0.5f * Scale;
		halfHeight = 0.5f * Scale;
	}
	else if (CustomData1.x == 1)
	{
		halfWidth = 0.5f * Scale*CustomData1.y;
		halfHeight = 0.5f * Scale* 0.2;
	}


	float4 v[4];
	for (int i = 0; i < 4; i++)
		v[i] = gin[0].Pos;
	
	v[0].x += -halfWidth;
	v[0].y  += -halfHeight;

	v[1].x += -halfWidth;
	v[1].y += halfHeight;

	v[2].x += halfWidth;
	v[2].y += -halfHeight;

	v[3].x += halfWidth;
	v[3].y += halfHeight;

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
	float4 textureColor; //텍스쳐 색상
	float4 litColor;

	
	//텍스쳐의 기본 색상 - 샘플러를 사용하여 값 추출
	textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.Tex);
	
	//알파테스트 실행
	clip(textureColor.a - 0.1f);

	PSOUT pout;
	pout.Color = textureColor;
	
	
	pout.Depth = -1;
	

	return pout;

}

float4 PS2(GeoOut pin) : SV_TARGET
{
	float4 textureColor; //텍스쳐 색상
	float4 litColor;


	//텍스쳐의 기본 색상 - 샘플러를 사용하여 값 추출
	textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.Tex);

	//알파테스트 실행
	clip(textureColor.a - 0.1f);



	return textureColor;

}