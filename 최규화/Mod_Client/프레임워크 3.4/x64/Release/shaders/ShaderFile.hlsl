#include "common.hlsl"

struct VertexIn
{
	float3 PosL  : POSITION;//위치
	float3 Normal : NORMAL;//노멀
	float2 Tex : TEXTURE;//텍스처 좌표
	float StartWeight : STARTWEIGHT;//시작할 웨이트 인덱스
	int WeightCount : WEIGHTCOUNT;//웨이트 몇개 있는가?
	float4 JointID : JOINTID;//해당 버텍스가 참조할 조인트 아이디
	float4 WeightPos1 : WEIGHTPOS0;//웨이트의 포지션들. 최대 4개며 마지막 w인자가 바이어스임. 
	float4 WeightPos2 : WEIGHTPOS1;
	float4 WeightPos3 : WEIGHTPOS2;
	float4 WeightPos4 : WEIGHTPOS3;
	float3 Tangent : TANGENT;//탄젠트


};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 PosW :WORLDPOSITION;
	float3 Normal : NORMAL;
	float dummy : DUMMY;
	float2 Tex : TEXTURE;
	float2 dummy2 : Dummy2;//대소문자 관계없으므로 Dummy하면 에러난다
	float3 Tangent : TANGENT;
	float Fog : FOG;
	
};


VertexOut VS(VertexIn vin)
{

	VertexOut vout;

	if (isAnimation==1)//애니메이션 오브젝트일때만 계산함.
	{
		VertexIn tempVert = vin;
		tempVert.PosL = float3(0, 0, 0);

		for (int j = 0; j < tempVert.WeightCount; ++j)
		{
			//Weight tempWeight = MD5Model->weights[tempVert.StartWeight + j];

			Joint tempJoint = interpolatedSkeleton[tempVert.JointID[j]];
			float4 tempJointOrientation = float4(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);
			float4 tempWeightPos;
			switch (j)
			{
			case 0:
				tempWeightPos = float4(tempVert.WeightPos1.x, tempVert.WeightPos1.y, tempVert.WeightPos1.z, tempVert.WeightPos1.w);
				break;
			case 1:
				tempWeightPos = float4(tempVert.WeightPos2.x, tempVert.WeightPos2.y, tempVert.WeightPos2.z, tempVert.WeightPos2.w);
				break;
			case 2:
				tempWeightPos = float4(tempVert.WeightPos3.x, tempVert.WeightPos3.y, tempVert.WeightPos3.z, tempVert.WeightPos3.w);
				break;
			case 3:
				tempWeightPos = float4(tempVert.WeightPos4.x, tempVert.WeightPos4.y, tempVert.WeightPos4.z, tempVert.WeightPos4.w);
				break;

			}
			float4 tempJointOrientationConjugate = float4(-tempJointOrientation.x, -tempJointOrientation.y, -tempJointOrientation.z, tempJointOrientation.w);

			float4 rotatedPoint;

			rotatedPoint = QuaternionMultiply(QuaternionMultiply(tempJointOrientation, float4(tempWeightPos.x, tempWeightPos.y, tempWeightPos.z, 0)), tempJointOrientationConjugate);

			float4 tp = float4(tempJoint.pos, 0);
			float4 rp = float4(rotatedPoint);
			float4 tpv = float4(tempVert.PosL, 0);


			tempVert.PosL = tpv + (tp + rp)*tempWeightPos.w;

		}


		vin.PosL = tempVert.PosL;// *Scale;
		//여기까지가 애니메이션 처리 완료
	}
	
	//---------------------여기서 부터 시작 --------------------------

	vin.PosL = vin.PosL * Scale;
	vout.PosH = mul(float4(vin.PosL, 1), gWorld);
	vout.PosW = vout.PosH;
	vout.Normal = mul(vin.Normal, gWorld);
	vout.Normal = normalize(vout.Normal);
	vout.Tangent = mul(vin.Tangent, gWorld);
	vout.Tangent = normalize(vout.Tangent);
	vout.PosH = mul(vout.PosH, gViewProj);

	vout.Tex = vin.Tex;

	float foglength = 400;
	float fogdensity = 0.231f;

	float fdist = distance(vout.PosW, float4(gEyePos, 1));
	fdist = fdist / foglength * 4;
	float fexp = exp(-(fdist*fogdensity)*(fdist*fogdensity));
	vout.Fog = fexp;


	return vout;
}


float4 PS(VertexOut pin) : SV_Target
{
	float4 textureColor; //텍스쳐 색상
	float3 lightDir;     //빛벡터
	float lightIntensity;
	float3 reflection;   //반사광
	float4 specular = float4(0,0,0,1);
	float4 litColor = float4(1,1,1,1);
	float3 viewDirection;
	float4 finalcolor;

	//텍스쳐의 기본 색상 - 샘플러를 사용하여 값 추출
	if (CustomData1.w >= 100 && CustomData1.w <= 500)
		textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, frac(pin.Tex*CustomData1.w*pow(pin.Tex.x, 3))) * gDiffuse;
	else
		textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.Tex) * gDiffuse;
	//알파 테스트
	//clip(textureColor.a - 0.1f);
	//
	
	//CustomData1의 w가 1234 이면 노멀매핑을 쓰는것.
	if (CustomData1.w == 1234)
	{
		//노멀매핑할때 텍스처를 절반으로 나눠야함. 0~0.5까지는 일반텍스처고 0.5~1.0 까진 노멀맵
		//float2 ptex = pin.Tex;
		//ptex.x = clamp(ptex.x / 2, 0, 0.5f);
		//float2 ntex = ptex;
		//ntex.x = clamp(ntex.x + 0.5f, 0.5f, 1.0f);
		float3 Normaltexture = gNormalMap.Sample(gsamAnisotropicWrap, pin.Tex);
		float3 tangent = normalize(pin.Tangent - dot(pin.Tangent, pin.Normal)*pin.Normal);
		float3 binormal = cross(pin.Normal, tangent);

		float3x3 tbn = float3x3(tangent, binormal, pin.Normal);

		float3 newNormal = (Normaltexture * 2) - 1;//노멀은 -1~1범위를 가져야 하니까 이런처리를 한다.

		pin.Normal = mul(newNormal, tbn);


	}

	float3 NormalVector = pin.Normal;

	float4 diffuseLight = float4(0, 0, 0, 0);
	float4 specularLight = float4(0, 0, 0, 0);

	//카메라 방향 계산
	viewDirection = gEyePos - pin.PosW.xyz;
	viewDirection = normalize(viewDirection);
	//빛의 방향 계산.
	lightDir = -(gLights[0].Direction);
	lightDir = normalize(lightDir);
	//기본적인 빛 색상
	litColor = gLights[0].DiffuseColor;
	//기본적인 빛의 세기
	lightIntensity = max(dot(NormalVector, lightDir),0);
	if(SpecularParamater>=0)
		lightIntensity =ceil(lightIntensity * 5) / 5;
	//기본적인 빛 계산
	diffuseLight += CalcDiffuseLight(pin.Normal, lightDir, litColor, lightIntensity);
	specularLight += CalcSpecularLight(pin.Normal, lightDir,viewDirection, litColor, lightIntensity);

	diffuseLight += CalcDiffuseLight(pin.Normal, -lightDir + float3(0, lightDir.y * 2, 0), litColor, lightIntensity/2);

	if (SpecularParamater >= 0)
	{
		//i가 0일때는 그냥 전역광원(태양 같은걸로 처리) 하고 나머지는 점광원으로 할예정..
		for (int i = 1; i < nLights; ++i)
		{

			float3 PointLightDirection = -pin.PosW.xyz +gLights[i].Position;

			float DistanceSq = lengthSquared(PointLightDirection);

			float radius = 1000;

			
			if (DistanceSq < abs(radius * radius))
			{
				float Distance = sqrt(DistanceSq);

				//노멀화
				PointLightDirection /= Distance;

				float du = Distance / (1 - DistanceSq / (radius * radius - 1));
				
				float denom = du / abs(radius) + 1;

				
				float attenuation = 1 / (denom * denom);

				float pointintensity= max(dot(NormalVector, -normalize(gLights[i].Direction)), 0);
				pointintensity = smoothstep(0.25, 0.7, pointintensity)+0.15f;
		
				diffuseLight += CalcDiffuseLight(pin.Normal, PointLightDirection, gLights[i].DiffuseColor, pointintensity) * attenuation;

				specularLight += CalcSpecularLight(pin.Normal, PointLightDirection, viewDirection, gLights[i].DiffuseColor, pointintensity) * attenuation;
			}

			
			

			

		}

	}

	finalcolor = (textureColor * diffuseLight) + litColor * textureColor * lightIntensity + gAmbientLight *textureColor +specularLight*textureColor;
	finalcolor.w = BlendValue;

	float fogc = pin.Fog;
	
	float4 colorfog = float4(0.85, 0.85 , 0.85, 1);

	finalcolor = lerp(colorfog, finalcolor, fogc);
	

	if (nLights > 0)
	{
		
			return finalcolor;//float4(pin.Normal,1);
		
	}
	else
		return float4(0, 0, 0, 1);

}

float4 PS2(VertexOut pin) : SV_Target
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
	
	//알파 테스트
	//clip(textureColor.a - 0.1f);
	//
	
	viewDirection = gEyePos - pin.PosH.xyz;
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
	
		specular = pow(saturate(dot(reflection, viewDirection)), 32.0f)*SpecularParamater;
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
	
	litColor = saturate(litColor+ Emissive + specular); //마지막으로 스패큘러 더한다.
	litColor.w = BlendValue;
	
	float fogc = pin.Fog;

	float4 colorfog = float4(0.85, 0.85, 0.85, 1);

	litColor = lerp(colorfog, litColor, fogc);

	//litColor.a = textureColor.a;
	
	if (nLights > 0)
	return litColor;
	else
	return float4(0, 0, 0, 1);
	
	
}


float4 SPS(VertexOut pin) : SV_Target
{
	float4 textureColor; //텍스쳐 색상
float3 lightDir;     //빛벡터
float lightIntensity;
float3 reflection;   //반사광
float4 specular = float4(0,0,0,1);
float4 litColor = float4(1,1,1,1);
float3 viewDirection;
float4 finalcolor;

//텍스쳐의 기본 색상 - 샘플러를 사용하여 값 추출
if (CustomData1.w >= 100 && CustomData1.w <= 500)
textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, frac(pin.Tex*CustomData1.w*pow(pin.Tex.x, 3))) * gDiffuse;
else
textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.Tex) * gDiffuse;
//알파 테스트
//clip(textureColor.a - 0.1f);
//

//CustomData1의 w가 1234 이면 노멀매핑을 쓰는것.
if (CustomData1.w == 1234)
{
	//노멀매핑할때 텍스처를 절반으로 나눠야함. 0~0.5까지는 일반텍스처고 0.5~1.0 까진 노멀맵
	//float2 ptex = pin.Tex;
	//ptex.x = clamp(ptex.x / 2, 0, 0.5f);
	//float2 ntex = ptex;
	//ntex.x = clamp(ntex.x + 0.5f, 0.5f, 1.0f);
	float3 Normaltexture = gNormalMap.Sample(gsamAnisotropicWrap, pin.Tex);
	float3 tangent = normalize(pin.Tangent - dot(pin.Tangent, pin.Normal)*pin.Normal);
	float3 binormal = cross(pin.Normal, tangent);

	float3x3 tbn = float3x3(tangent, binormal, pin.Normal);

	float3 newNormal = (Normaltexture * 2) - 1;//노멀은 -1~1범위를 가져야 하니까 이런처리를 한다.

	pin.Normal = mul(newNormal, tbn);


}

float3 NormalVector = pin.Normal;

float4 diffuseLight = float4(0, 0, 0, 0);
float4 specularLight = float4(0, 0, 0, 0);

//카메라 방향 계산
viewDirection = gEyePos - pin.PosW.xyz;
viewDirection = normalize(viewDirection);
//빛의 방향 계산.
lightDir = -(gLights[0].Direction);
lightDir = normalize(lightDir);
//기본적인 빛 색상
litColor = gLights[0].DiffuseColor;
//기본적인 빛의 세기
lightIntensity = max(dot(NormalVector, lightDir),0);
if (SpecularParamater >= 0)
lightIntensity = ceil(lightIntensity * 5) / 5;
//기본적인 빛 계산
diffuseLight += CalcDiffuseLight(pin.Normal, lightDir, litColor, lightIntensity);
specularLight += CalcSpecularLight(pin.Normal, lightDir,viewDirection, litColor, lightIntensity);

diffuseLight += CalcDiffuseLight(pin.Normal, -lightDir + float3(0, lightDir.y * 2, 0), litColor, lightIntensity / 2);

if (SpecularParamater >= 0)
{
	//i가 0일때는 그냥 전역광원(태양 같은걸로 처리) 하고 나머지는 점광원으로 할예정..
	for (int i = 1; i < nLights; ++i)
	{

		float3 PointLightDirection = -pin.PosW.xyz + gLights[i].Position;

		float DistanceSq = lengthSquared(PointLightDirection);

		float radius = 1000;


		if (DistanceSq < abs(radius * radius))
		{
			float Distance = sqrt(DistanceSq);

			//노멀화
			PointLightDirection /= Distance;

			float du = Distance / (1 - DistanceSq / (radius * radius - 1));

			float denom = du / abs(radius) + 1;


			float attenuation = 1 / (denom * denom);

			float pointintensity = max(dot(NormalVector, -normalize(gLights[i].Direction)), 0);
			pointintensity = smoothstep(0.25, 0.7, pointintensity) + 0.15f;

			diffuseLight += CalcDiffuseLight(pin.Normal, PointLightDirection, gLights[i].DiffuseColor, pointintensity) * attenuation;

			specularLight += CalcSpecularLight(pin.Normal, PointLightDirection, viewDirection, gLights[i].DiffuseColor, pointintensity) * attenuation;
		}






	}

}

finalcolor = (textureColor * diffuseLight) + litColor * textureColor * lightIntensity + gAmbientLight * textureColor + specularLight * textureColor;
finalcolor.w = BlendValue;


if (nLights > 0)
{

	return finalcolor;//float4(pin.Normal,1);

}
else
return float4(0, 0, 0, 1);

}