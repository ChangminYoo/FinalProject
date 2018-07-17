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
	
	//�ؽ����� �⺻ ���� - ���÷��� ����Ͽ� �� ����
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


	if (SpecularParamater >= 0)
	{
		for (int i = 0; i < nLights; ++i)
		{

			viewDirection = gEyePos - pin.PosW.xyz;
			viewDirection = normalize(viewDirection);

			lightDir = -(gLights[i].Direction);
			lightDir = normalize(lightDir);
			litColor = gAmbientLight;

			specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

			//�ȼ��� ��ġ�� ���Ǿ� (0 ~ 1)
			lightIntensity = saturate(dot(NormalVector, lightDir));   //-> ������Ʈ �ڻ��� ��Ģ 

																//lightIntensity = round(lightIntensity * 4) / 3;

																// 0���� ũ�� (���� �޴� �κ��̸�)
			if (lightIntensity > 0.0f)
			{

				litColor += (float4(gLights[i].DiffuseColor) * float4(0.37, 0.37, 0.37, 1));




				litColor += (float4(gLights[i].DiffuseColor) * lightIntensity);

				litColor = saturate(litColor);

				reflection = normalize(2 * lightIntensity * NormalVector - lightDir);

				//������ 6.0 ��� 32����.
				specular = pow(saturate(dot(reflection, viewDirection)), 32.0f)*SpecularParamater;
			}

			// 0�̸� (���� �� �޴� �κ��̸�)
			else if (lightIntensity <= 0.0f)
			{

				litColor += (float4(gLights[i].DiffuseColor) * float4(0.672f, 0.672f, 0.672f, 1.0f)); //���� �κ��� �����ϸ� ���� �ȹ޴� �κ��� ������ ������ �� �ֽ��ϴ�.

				litColor = saturate(litColor);

				reflection = normalize(2 * lightIntensity * pin.Normal - lightDir);

				specular = pow(saturate(dot(reflection, viewDirection)), 32.0f)*SpecularParamater;
			}

		}

	}

	litColor = litColor * textureColor;  //�����Ʈ * �ؽ��� �÷�
	
	litColor = saturate(litColor + specular); //���������� ����ŧ�� ���Ѵ�.
	litColor.w = BlendValue;
	
	//litColor = textureColor;
	//litColor.a = textureColor.a;
	
	//��ָ����� �Ǿ����� Ȯ��. �ٹ��̰� ������ ��ָ��ζ����� �� �ٹ������� ����� 0 �̵ǹǷ� ����Ȱ�!
	//litColor = float4(pin.Normal,1);
	if (nLights > 0)
	return litColor;
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
	
	//litColor.a = textureColor.a;
	
	if (nLights > 0)
	return litColor;
	else
	return float4(0, 0, 0, 1);
	
	
}
