#include "common.hlsl"

struct VertexIn
{
	float3 Pos : POSITION;
	float2 Size : SIZE;

};

struct VertexOut
{
	float Pos : POSITION;
	float2 Size : SIZE;
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

	vout.Pos = mul(vin.Pos, gWorld);

	vout.Size = vin.Size;

	return vout;

}

[maxvertexcount(4)]
void GS(point VertexOut gin[1], inout TriangleStream<GeoOut> triStream)
{
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 look = gin[0].Pos - gEyePos;
	look.y = 0.0f;				// xz평면에 투영
	look = normalize(look);
	float3 right = cross(look, up);

	float halfWidth = 0.5f*gin[0].Size.x;								//						 
	float halfHeight = 0.5f*gin[0].Size.y;								//				 v[1]--------v[3]
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
	float3 lightDir;     //빛벡터
	float lightIntensity;
	float3 reflection;   //반사광
	float4 specular;
	float4 litColor;
	float3 viewDirection;
	
	
	//텍스쳐의 기본 색상 - 샘플러를 사용하여 값 추출
	textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.Tex);
	
	clip(textureColor.a - 0.1f);

	viewDirection = gEyePos - pin.PosW.xyz;
	viewDirection = normalize(viewDirection);
	
	lightDir = -(gLights[0].Direction);
	lightDir = normalize(lightDir);
	litColor = gAmbientLight;
	
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	//픽셀당 비치는 빛의양 (0 ~ 1)
	lightIntensity = saturate(dot(pin.Normal, lightDir));   //-> 람베르트 코사인 법칙 
	
															// 0보다 크면 (빛을 받는 부분이면)
	if (lightIntensity > 0.0f)
	{
		litColor += (float4(gLights[0].DiffuseColor) * lightIntensity);
	
		litColor = saturate(litColor);
	
		reflection = normalize(2 * lightIntensity * pin.Normal - lightDir);
	
		specular = pow(saturate(dot(reflection, viewDirection)), 128.0f)*SpecularParamater;
	}
	
	// 0이면 (빛을 안 받는 부분이면)
	else if (lightIntensity <= 0.0f)
	{
	
		litColor += (float4(gLights[0].DiffuseColor) * float4(0.35f, 0.35f, 0.35f, 1.0f)); //여기 부분을 조정하면 빛을 안받는 부분의 음영을 조정할 수 있습니다.
	
		litColor = saturate(litColor);
	
		reflection = normalize(2 * lightIntensity * pin.Normal - lightDir);
	
		specular = pow(saturate(dot(reflection, viewDirection)), 32.0f)*SpecularParamater;
	}
	
	litColor = litColor * textureColor;  //엠비언트 * 텍스쳐 컬러
	
	litColor = saturate(litColor + specular); //마지막으로 스패큘러 더한다.
	litColor.w = BlendValue;

	litColor.a = textureColor.a;

	if (nLights > 0)
		return litColor;
	else
		return float4(0, 0, 0, 1);
}