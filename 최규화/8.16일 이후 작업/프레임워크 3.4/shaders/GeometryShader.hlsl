#include "common.hlsl"

struct VertexIn
{
	float3 Pos : POSITION;

	float4 Normal : NORMAL;
};

struct VertexOut
{
	float4 Pos : POSITION;
	float4 PosW : POSITIONWORLD;
	float4 Normal : NORMAL;
	float Fog : FOG;
};

struct PSOUT
{
	float4 Color : SV_TARGET;
	float Depth : SV_DEPTH;
};

struct GeoOut
{
	float4 PosH : SV_POSITION;
	float4 PrevNormal : PNORMAL;
	float3 PosW : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXTURE;
	bool Clipping : CLIPPING;
	float Fog : FOG;
};


VertexOut VS(VertexIn vin)
{
	const float CDX2 = 20;
	const float CDX10 = 60;
	const float CDX11 = 10;
	VertexOut vout;

	vout.Pos = mul(float4(vin.Pos, 1), gWorld);
	vout.PosW = vout.Pos;
	vout.Normal = vin.Normal;
	//반드시 월드변환 이후에 해야함. 
	if (CustomData1.x == 2)//총알 파티클
	{
		//	vout.Pos.x += cos(20*Velocity.x * PTime * 3.141592/180 )*5;
		//	vout.Pos.z += cos(20 * Velocity.z * PTime * 3.141592 ) * 5;
		//	vout.Pos.y += sin(20 * Velocity.y * PTime * 3.141592/180 ) * 5;
		//여기서 Normal은 진짜 노멀이 아니라, 버텍스마다 다른속도를 갖게하기 위한 '속도' 이다.
		float3 NV = float3(0, 0, 0);
		NV.x = (Velocity.x + vin.Normal.x);
		NV.y = (Velocity.y + vin.Normal.y);
		NV.z = (Velocity.z + vin.Normal.z);
		NV = normalize(NV)*CDX2;
		vout.Pos.y += NV.y* frac(PTime*vin.Normal.w / 100) * 8;
		vout.Pos.z += NV.z* frac(PTime*vin.Normal.w / 100) * 8;
		vout.Pos.x += NV.x* frac(PTime*vin.Normal.w / 100) * 8;
	}
	
	else if (CustomData1.x == 10)//총알 파티클
	{
		//	vout.Pos.x += cos(20*Velocity.x * PTime * 3.141592/180 )*5;
		//	vout.Pos.z += cos(20 * Velocity.z * PTime * 3.141592 ) * 5;
		//	vout.Pos.y += sin(20 * Velocity.y * PTime * 3.141592/180 ) * 5;
		//여기서 Normal은 진짜 노멀이 아니라, 버텍스마다 다른속도를 갖게하기 위한 '속도' 이다.
		float3 NV = float3(0, 0, 0);
		NV.x = (Velocity.x + vin.Normal.x);
		NV.y = (Velocity.y + vin.Normal.y);
		NV.z = (Velocity.z + vin.Normal.z);
		NV = normalize(NV)*CDX10;
		vout.Pos.y += NV.y*(PTime + vin.Normal.w / 700) - 35 * PTime*PTime;
		vout.Pos.z += NV.z*(PTime * 3 + vin.Normal.w / 200);
		vout.Pos.x += NV.x*(PTime * 2 + vin.Normal.w / 100);
	}
	else if (CustomData1.x ==11)//총알 파티클
	{
		float3 NV = float3(0, 0, 0);
		NV.x = (Velocity.x + vin.Normal.x);
		NV.y = (Velocity.y + vin.Normal.y);
		NV.z = (Velocity.z + vin.Normal.z);
		NV = normalize(NV)*CDX11;
		vout.Pos.y += NV.y* frac(PTime*4*vin.Normal.w / 400) * 0.48;
		vout.Pos.z += NV.z* frac(PTime * 4 *vin.Normal.w / 300) * 0.48;
		vout.Pos.x += NV.x* frac(PTime * 4 *vin.Normal.w / 500) * 0.48;
	}
	vout.Pos = mul(vout.Pos, gView);


	float foglength = 300;
	float fogdensity = 0.32f;

	float fdist = distance(vout.PosW, float4(gEyePos, 1));
	fdist = fdist / foglength * 4;
	float fexp = exp(-(fdist*fogdensity)*(fdist*fogdensity));
	vout.Fog = fexp;


	return vout;

}

[maxvertexcount(4)]
void GS(point VertexOut gin[1], inout TriangleStream<GeoOut> triStream)
{
	bool Clip =false;
	float halfWidth;
	float halfHeight;


	float4 v[4];
	for (int i = 0; i < 4; i++)
		v[i] = gin[0].Pos;

	//CustomData1.x : 파티클의 정체성
	if (CustomData1.x == 0)//데미지 등 고정된 크기를 갖는 경우.
	{
		halfWidth = 0.5f * Scale;
		halfHeight = 0.5f * Scale;

		v[0].x += -halfWidth;
		v[0].y += -halfHeight;

		v[1].x += -halfWidth;
		v[1].y += halfHeight;

		v[2].x += halfWidth;
		v[2].y += -halfHeight;

		v[3].x += halfWidth;
		v[3].y += halfHeight;
	}
	else if (CustomData1.x == 1)//HP바
	{
		halfWidth = 0.5f * Scale;
		halfHeight = 0.1f * Scale * 0.7;
		v[0].x += -halfWidth;
		v[0].y += -halfHeight;

		v[1].x += -halfWidth;
		v[1].y += halfHeight;

		v[2].x += halfWidth - 2 * halfWidth*(1 - CustomData1.y);
		v[2].y += -halfHeight;

		v[3].x += halfWidth - 2 * halfWidth *(1 - CustomData1.y);
		v[3].y += halfHeight;


	}
	else if (CustomData1.x == 2)//총알 파티클
	{
		//파티클과 카메라 사이 길이가 150보다 크면 쿼드형이 아니라 정점형으로 처리.
		float4 distV = gin[0].Pos - float4(gEyePos, 1);
		float dist = dot(distV, distV);
		dist = sqrt(dist);
		if (dist > 150)
			Clip = true;

		halfWidth = 0.5f * Scale;
		halfHeight = 0.5f * Scale;


		v[0].x += -halfWidth;
		v[0].y += -halfHeight;

		v[1].x += -halfWidth;
		v[1].y += halfHeight;

		v[2].x += halfWidth;
		v[2].y += -halfHeight;

		v[3].x += halfWidth;
		v[3].y += +halfHeight;


	}
	else if (CustomData1.x == 3) // 스킬 ui
	{
		halfWidth = 0.5f * Scale;
		halfHeight = 0.5f * Scale*0.7;

		v[0].x += -halfWidth;
		v[0].y += -halfHeight;

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
	else if (CustomData1.x == 5)//백그라운드
	{
		halfWidth = 0.5f * Scale;
		halfHeight = 0.5f *CustomData1.y;

		v[0].x += -halfWidth;
		v[0].y += -halfHeight;

		v[1].x += -halfWidth;
		v[1].y += halfHeight;

		v[2].x += halfWidth;
		v[2].y += -halfHeight;

		v[3].x += halfWidth;
		v[3].y += +halfHeight;
	}
	else if (CustomData1.x == 7)
	{
		halfWidth = 0.5f * Scale;
		halfHeight = 0.5f * Scale;

		v[0].x += -halfWidth;
		v[0].y += -halfHeight;

		v[1].x += -halfWidth;
		v[1].y += halfHeight;

		v[2].x += halfWidth;
		v[2].y += -halfHeight;

		v[3].x += halfWidth;
		v[3].y += halfHeight;
	}
	else if (CustomData1.x == 8)//스킬쿨바
	{
		halfWidth = 0.5f * Scale;
		halfHeight = 0.1f * Scale * 0.7;
		v[0].x += -halfWidth;
		v[0].y += -halfHeight;

		v[1].x += -halfWidth;
		v[1].y += halfHeight;

		v[2].x += halfWidth - 2 * halfWidth*(1 - CustomData1.y);
		v[2].y += -halfHeight;

		v[3].x += halfWidth - 2 * halfWidth *(1 - CustomData1.y);
		v[3].y += halfHeight;


	}
	else if (CustomData1.x == 10)//총알이 부딪힐때
	{
		halfWidth = 0.5f *  Scale*sin(PTime * 3 + gin[0].Normal.w / 100) + 0.5f;
		halfHeight = 0.5f * Scale*sin(PTime * 3 + gin[0].Normal.w / 100) + 0.5f;


		v[0].x += -halfWidth;
		v[0].y += -halfHeight;

		v[1].x += -halfWidth;
		v[1].y += halfHeight;

		v[2].x += halfWidth;
		v[2].y += -halfHeight;

		v[3].x += halfWidth;
		v[3].y += +halfHeight;


	}
	else if (CustomData1.x == 11)//보스 스택 기술
	{
		halfWidth = 0.5f *  Scale*max(0.6,sin(PTime * 10 + gin[0].Normal.w / 120)) + 0.5f;
		halfHeight = 0.5f * Scale*max(0.5,sin(PTime * 10 + gin[0].Normal.w / 140)) + 0.5f;


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
	tex[0] = float2(TexClamp.x, 1.0f);
	tex[1] = float2(TexClamp.x, 0.0f);
	tex[2] = float2(TexClamp.y, 1.0f);
	tex[3] = float2(TexClamp.y, 0.0f);


	GeoOut gout;
	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		gout.PosH = mul(v[i], gProj);
		gout.PosW = gin[0].PosW.xyz;
		gout.Normal = float3(0, 1, 0);
		gout.Tex = tex[i];
		gout.Fog = gin[0].Fog;
		gout.PrevNormal = gin[0].Normal;
		if(Clip==false)
			gout.Clipping = false;
		else
		{
			if (i == 0)
				gout.Clipping = false;
			else
				gout.Clipping = true;
		}
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
	

	pout.Depth = -1;

	//피격시 별모양 파티클
	if (CustomData1.x == 10)
	{

		textureColor.x = max(cos(PTime * 3 * pin.PrevNormal.w / 120), 0.3) + 0.15;
		textureColor.y = max(sin(PTime * 4 * pin.PrevNormal.w / 120), 0.5) + 0.15;
		textureColor.z = max(cos(PTime * 5 * pin.PrevNormal.w / 120), 0.4) + 0.15;


	}
	pout.Color = textureColor;

	float fogc = pin.Fog;

	float4 colorfog = float4(0.8, 0.8, 0.8, 1);

	if (CustomData1.x != 7 && CustomData1.x != 8 && CustomData1.x != 3 && CustomData1.x != 5)	
		pout.Color = lerp(colorfog, pout.Color, fogc);


	return pout;

}

float4 PS2(GeoOut pin) : SV_TARGET
{
	float4 textureColor; //텍스쳐 색상
	float4 litColor;


	//텍스쳐의 기본 색상 - 샘플러를 사용하여 값 추출
	textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.Tex);

	//textureColor += Emissive;

	//알파테스트 실행
	clip(textureColor.a - 0.1f);

	if(pin.Clipping)
		discard;
	
	//pout.Depth = 1.0f;
	if (CustomData1.x == 2)
	{
		textureColor.x = max(cos(PTime * 4 * pin.PrevNormal.w / 100), 0.23) + 0.1215;
		textureColor.y = max(cos(PTime * 6 * pin.PrevNormal.w / 100), 0.2) + 0.125;
		textureColor.z = max(sin(PTime * 3 * pin.PrevNormal.w / 100), 0.14) + 0.115;
	}
	
	else if (CustomData1.x == 11)
	{

		textureColor.x = max(cos(PTime * 2 * pin.PrevNormal.w / 131), 0.3) + 0.15;
		textureColor.y =sin(PTime * 14 * pin.PrevNormal.w / 324) + 0.45;
		textureColor.z = cos(PTime * 5 * pin.PrevNormal.w / 650) + 0.25;
	
	}
	
	float fogc = pin.Fog;

	float4 colorfog = float4(0.8, 0.8, 0.8, 1);

	if (CustomData1.x != 7 && CustomData1.x!=8 && CustomData1.x!=3&&CustomData1.x!=5)
	textureColor = lerp(colorfog, textureColor, fogc);

	return  textureColor;

}