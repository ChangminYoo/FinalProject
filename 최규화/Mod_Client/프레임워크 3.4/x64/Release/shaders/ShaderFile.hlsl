#include "common.hlsl"

struct VertexIn
{
	float3 PosL  : POSITION;//��ġ
	float3 Normal : NORMAL;//���
	float2 Tex : TEXTURE;//�ؽ�ó ��ǥ
	float StartWeight : STARTWEIGHT;//������ ����Ʈ �ε���
	int WeightCount : WEIGHTCOUNT;//����Ʈ � �ִ°�?
	float4 JointID : JOINTID;//�ش� ���ؽ��� ������ ����Ʈ ���̵�
	float4 WeightPos1 : WEIGHTPOS0;//����Ʈ�� �����ǵ�. �ִ� 4���� ������ w���ڰ� ���̾��. 
	float4 WeightPos2 : WEIGHTPOS1;
	float4 WeightPos3 : WEIGHTPOS2;
	float4 WeightPos4 : WEIGHTPOS3;
	float3 Tangent : TANGENT;//ź��Ʈ


};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 PosW :WORLDPOSITION;
	float3 Normal : NORMAL;
	float dummy : DUMMY;
	float2 Tex : TEXTURE;
	float2 dummy2 : Dummy2;//��ҹ��� ��������Ƿ� Dummy�ϸ� ��������
	float3 Tangent : TANGENT;
	float Fog : FOG;
	
};


VertexOut VS(VertexIn vin)
{

	VertexOut vout;

	if (isAnimation==1)//�ִϸ��̼� ������Ʈ�϶��� �����.
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
		//��������� �ִϸ��̼� ó�� �Ϸ�
	}
	
	//---------------------���⼭ ���� ���� --------------------------

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
	float4 textureColor; //�ؽ��� ����
	float3 lightDir;     //������
	float lightIntensity;
	float3 reflection;   //�ݻ籤
	float4 specular = float4(0,0,0,1);
	float4 litColor = float4(1,1,1,1);
	float3 viewDirection;
	float4 finalcolor;

	//�ؽ����� �⺻ ���� - ���÷��� ����Ͽ� �� ����
	if (CustomData1.w >= 100 && CustomData1.w <= 500)
		textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, frac(pin.Tex*CustomData1.w*pow(pin.Tex.x, 3))) * gDiffuse;
	else
		textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.Tex) * gDiffuse;
	//���� �׽�Ʈ
	//clip(textureColor.a - 0.1f);
	//
	
	//CustomData1�� w�� 1234 �̸� ��ָ����� ���°�.
	if (CustomData1.w == 1234)
	{
		//��ָ����Ҷ� �ؽ�ó�� �������� ��������. 0~0.5������ �Ϲ��ؽ�ó�� 0.5~1.0 ���� ��ָ�
		//float2 ptex = pin.Tex;
		//ptex.x = clamp(ptex.x / 2, 0, 0.5f);
		//float2 ntex = ptex;
		//ntex.x = clamp(ntex.x + 0.5f, 0.5f, 1.0f);
		float3 Normaltexture = gNormalMap.Sample(gsamAnisotropicWrap, pin.Tex);
		float3 tangent = normalize(pin.Tangent - dot(pin.Tangent, pin.Normal)*pin.Normal);
		float3 binormal = cross(pin.Normal, tangent);

		float3x3 tbn = float3x3(tangent, binormal, pin.Normal);

		float3 newNormal = (Normaltexture * 2) - 1;//����� -1~1������ ������ �ϴϱ� �̷�ó���� �Ѵ�.

		pin.Normal = mul(newNormal, tbn);


	}

	float3 NormalVector = pin.Normal;

	float4 diffuseLight = float4(0, 0, 0, 0);
	float4 specularLight = float4(0, 0, 0, 0);

	//ī�޶� ���� ���
	viewDirection = gEyePos - pin.PosW.xyz;
	viewDirection = normalize(viewDirection);
	//���� ���� ���.
	lightDir = -(gLights[0].Direction);
	lightDir = normalize(lightDir);
	//�⺻���� �� ����
	litColor = gLights[0].DiffuseColor;
	//�⺻���� ���� ����
	lightIntensity = max(dot(NormalVector, lightDir),0);
	if(SpecularParamater>=0)
		lightIntensity =ceil(lightIntensity * 5) / 5;
	//�⺻���� �� ���
	diffuseLight += CalcDiffuseLight(pin.Normal, lightDir, litColor, lightIntensity);
	specularLight += CalcSpecularLight(pin.Normal, lightDir,viewDirection, litColor, lightIntensity);

	diffuseLight += CalcDiffuseLight(pin.Normal, -lightDir + float3(0, lightDir.y * 2, 0), litColor, lightIntensity/2);

	if (SpecularParamater >= 0)
	{
		//i�� 0�϶��� �׳� ��������(�¾� �����ɷ� ó��) �ϰ� �������� ���������� �ҿ���..
		for (int i = 1; i < nLights; ++i)
		{

			float3 PointLightDirection = -pin.PosW.xyz +gLights[i].Position;

			float DistanceSq = lengthSquared(PointLightDirection);

			float radius = 1000;

			
			if (DistanceSq < abs(radius * radius))
			{
				float Distance = sqrt(DistanceSq);

				//���ȭ
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
	float4 textureColor; //�ؽ��� ����
	float3 lightDir;     //������
	float lightIntensity;
	float3 reflection;   //�ݻ籤
	float4 specular;
	float4 litColor;
	float3 viewDirection;
	
	
	//�ؽ����� �⺻ ���� - ���÷��� ����Ͽ� �� ����
	textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.Tex);
	
	//���� �׽�Ʈ
	//clip(textureColor.a - 0.1f);
	//
	
	viewDirection = gEyePos - pin.PosH.xyz;
	viewDirection = normalize(viewDirection);
	
	
	lightDir = -(gLights[0].Direction);
	lightDir = normalize(lightDir);
	litColor = gAmbientLight;
	
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	//�ȼ��� ��ġ�� ���Ǿ� (0 ~ 1)
	lightIntensity = saturate(dot(pin.Normal, lightDir));   //-> ������Ʈ �ڻ��� ��Ģ 
	
															// 0���� ũ�� (���� �޴� �κ��̸�)
	if (lightIntensity > 0.0f)
	{
		litColor += (float4(gLights[0].DiffuseColor) * lightIntensity);
	
		litColor = saturate(litColor);
	
		reflection = normalize(2 * lightIntensity * pin.Normal - lightDir);
	
		specular = pow(saturate(dot(reflection, viewDirection)), 32.0f)*SpecularParamater;
	}
	
	// 0�̸� (���� �� �޴� �κ��̸�)
	else if (lightIntensity <= 0.0f)
	{
	
		litColor += (float4(gLights[0].DiffuseColor) * float4(0.35f, 0.35f, 0.35f, 1.0f)); //���� �κ��� �����ϸ� ���� �ȹ޴� �κ��� ������ ������ �� �ֽ��ϴ�.
	
		litColor = saturate(litColor);
	
		reflection = normalize(2 * lightIntensity * pin.Normal - lightDir);
	
		specular = pow(saturate(dot(reflection, viewDirection)), 32.0f)*SpecularParamater;
	}
	
	litColor = litColor * textureColor;  //�����Ʈ * �ؽ��� �÷�
	
	litColor = saturate(litColor+ Emissive + specular); //���������� ����ŧ�� ���Ѵ�.
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
	float4 textureColor; //�ؽ��� ����
float3 lightDir;     //������
float lightIntensity;
float3 reflection;   //�ݻ籤
float4 specular = float4(0,0,0,1);
float4 litColor = float4(1,1,1,1);
float3 viewDirection;
float4 finalcolor;

//�ؽ����� �⺻ ���� - ���÷��� ����Ͽ� �� ����
if (CustomData1.w >= 100 && CustomData1.w <= 500)
textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, frac(pin.Tex*CustomData1.w*pow(pin.Tex.x, 3))) * gDiffuse;
else
textureColor = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.Tex) * gDiffuse;
//���� �׽�Ʈ
//clip(textureColor.a - 0.1f);
//

//CustomData1�� w�� 1234 �̸� ��ָ����� ���°�.
if (CustomData1.w == 1234)
{
	//��ָ����Ҷ� �ؽ�ó�� �������� ��������. 0~0.5������ �Ϲ��ؽ�ó�� 0.5~1.0 ���� ��ָ�
	//float2 ptex = pin.Tex;
	//ptex.x = clamp(ptex.x / 2, 0, 0.5f);
	//float2 ntex = ptex;
	//ntex.x = clamp(ntex.x + 0.5f, 0.5f, 1.0f);
	float3 Normaltexture = gNormalMap.Sample(gsamAnisotropicWrap, pin.Tex);
	float3 tangent = normalize(pin.Tangent - dot(pin.Tangent, pin.Normal)*pin.Normal);
	float3 binormal = cross(pin.Normal, tangent);

	float3x3 tbn = float3x3(tangent, binormal, pin.Normal);

	float3 newNormal = (Normaltexture * 2) - 1;//����� -1~1������ ������ �ϴϱ� �̷�ó���� �Ѵ�.

	pin.Normal = mul(newNormal, tbn);


}

float3 NormalVector = pin.Normal;

float4 diffuseLight = float4(0, 0, 0, 0);
float4 specularLight = float4(0, 0, 0, 0);

//ī�޶� ���� ���
viewDirection = gEyePos - pin.PosW.xyz;
viewDirection = normalize(viewDirection);
//���� ���� ���.
lightDir = -(gLights[0].Direction);
lightDir = normalize(lightDir);
//�⺻���� �� ����
litColor = gLights[0].DiffuseColor;
//�⺻���� ���� ����
lightIntensity = max(dot(NormalVector, lightDir),0);
if (SpecularParamater >= 0)
lightIntensity = ceil(lightIntensity * 5) / 5;
//�⺻���� �� ���
diffuseLight += CalcDiffuseLight(pin.Normal, lightDir, litColor, lightIntensity);
specularLight += CalcSpecularLight(pin.Normal, lightDir,viewDirection, litColor, lightIntensity);

diffuseLight += CalcDiffuseLight(pin.Normal, -lightDir + float3(0, lightDir.y * 2, 0), litColor, lightIntensity / 2);

if (SpecularParamater >= 0)
{
	//i�� 0�϶��� �׳� ��������(�¾� �����ɷ� ó��) �ϰ� �������� ���������� �ҿ���..
	for (int i = 1; i < nLights; ++i)
	{

		float3 PointLightDirection = -pin.PosW.xyz + gLights[i].Position;

		float DistanceSq = lengthSquared(PointLightDirection);

		float radius = 1000;


		if (DistanceSq < abs(radius * radius))
		{
			float Distance = sqrt(DistanceSq);

			//���ȭ
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