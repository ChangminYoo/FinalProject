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


};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float3 Normal : NORMAL;
	float dummy : DUMMY;
	float2 Tex : TEXTURE;

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
	float4 world;

	vin.PosL = vin.PosL * Scale;
	vout.PosH = mul(float4(vin.PosL, 1), gWorld);

	vout.Normal = mul(vin.Normal, gWorld);
	vout.Normal = normalize(vout.Normal);

	vout.PosH = mul(vout.PosH, gViewProj);

	vout.Tex = vin.Tex;

	return vout;
}


float4 PS(VertexOut pin) : SV_Target
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
	
	
		viewDirection = gLights[0].Position - pin.PosH.xyz;
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
		if (nLights > 0)
			return litColor;
		else
			return float4(0, 0, 0, 1);

}
	

