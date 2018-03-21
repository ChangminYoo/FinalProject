#include "common.hlsl"

struct VertexIn
{
	float3 Pos : POSITION;

};

struct VertexOut
{
	float4 Pos : POSITION;
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
	//vout.Pos = mul(vout.Pos, gView);


	return vout;

}

[maxvertexcount(4)]
void GS(point VertexOut gin[1], inout TriangleStream<GeoOut> triStream)
{
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 look = gin[0].Pos - gEyePos;
	
	look = normalize(look);
	float3 right = normalize(cross(look, up));

	up = normalize(cross(right, look));


	float halfWidth = 0.5f*Scale;			 
	float halfHeight = 0.5f*Scale;										//				 v[1]--------v[3]
																		//					\		   \ 
	float4 v[4];														//					 \	/center	\																
	v[0] = float4((gin[0].Pos + halfWidth*right - halfHeight*up), 1.0f);//					  v[0]--------v[2]
	v[1] = float4((gin[0].Pos + halfWidth*right + halfHeight*up), 1.0f);//				     /
	v[2] = float4((gin[0].Pos - halfWidth*right - halfHeight*up), 1.0f);//	                /
	v[3] = float4((gin[0].Pos - halfWidth*right + halfHeight*up), 1.0f);//				  cam

	
	float2 tex[4];
	tex[0] = float2(0.0f, 1.0f);
	tex[1] = float2(0.0f, 0.0f);
	tex[2] = float2(1.0f, 1.0f);
	tex[3] = float2(1.0f, 0.0f);


	GeoOut gout;
	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		gout.PosH = mul(v[i], gViewProj);
		gout.PosW = v[i].xyz;
		gout.Normal = look;
		gout.Tex = tex[i];

		triStream.Append(gout);

	}
}


float4 PS(GeoOut pin) : SV_TARGET
{
	float4 textureColor; //텍스쳐 색상
	float4 litColor;

	
	//텍스쳐의 기본 색상 - 샘플러를 사용하여 값 추출
	textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.Tex);
	
	//알파테스트 실행
	clip(textureColor.a - 0.1f);


	return textureColor;

}