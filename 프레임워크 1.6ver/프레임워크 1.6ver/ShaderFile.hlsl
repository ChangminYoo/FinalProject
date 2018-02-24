#define MAXJOINT 65

Texture2D gDiffuseMap : register(t0);
SamplerState gSamLinear : register(s0);

float4 QuaternionMultiply(float4 Q1, float4 Q2)
{
	float4 q;
	q.x = (Q2.w * Q1.x) + (Q2.x * Q1.w) + (Q2.y * Q1.z) - (Q2.z * Q1.y);
	q.y = (Q2.w * Q1.y) - (Q2.x * Q1.z) + (Q2.y * Q1.w) + (Q2.z * Q1.x);
	q.z = (Q2.w * Q1.z) + (Q2.x * Q1.y) - (Q2.y * Q1.x) + (Q2.z * Q1.w);
	q.w = (Q2.w * Q1.w) - (Q2.x * Q1.x) - (Q2.y * Q1.y) - (Q2.z * Q1.z);

	
	return q;
}

//애니메이션 처리용 조인트. 이걸로 웨이트포지션 들을 계산해서 실제 정점 위치가 계산됨
struct Joint
{
	float4 orientation:Orient;//방향
	int parentID : pID;//조인트 부모인덱스
	float3 pos:Posj;//조인트의 위치
};

cbuffer ObjectData : register(b0)
{
	float4x4 gWorld; 
	float Scale;
	bool isAnimation;
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

	float2 gRenderTargetSize;
}

struct VertexIn
{
	float3 PosL  : POSITION;//위치
	float4 Normal : NORMAL;//노멀
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
	float4 Normal : NORMAL;
	float2 Tex : TEXTURE;
};

VertexOut VS(VertexIn vin)
{

	VertexOut vout;

	if (isAnimation)//애니메이션 오브젝트일때만 계산함.
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


		vin.PosL = tempVert.PosL*Scale;
		//여기까지가 애니메이션 처리 완료
	}
	//---------------------여기서 부터 시작 --------------------------


	vout.PosH = mul(float4(vin.PosL, 1), gWorld);

	
	vout.PosH = mul(vout.PosH, gViewProj);


	vout.Tex = vin.Tex;

	return vout;
}


float4 PS(VertexOut pin) : SV_Target
{
	float4 diffuseAlbedo = gDiffuseMap.Sample(gSamLinear, pin.Tex);

	return diffuseAlbedo;
}


