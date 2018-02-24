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

//�ִϸ��̼� ó���� ����Ʈ. �̰ɷ� ����Ʈ������ ���� ����ؼ� ���� ���� ��ġ�� ����
struct Joint
{
	float4 orientation:Orient;//����
	int parentID : pID;//����Ʈ �θ��ε���
	float3 pos:Posj;//����Ʈ�� ��ġ
};

cbuffer ObjectData : register(b0)
{
	float4x4 gWorld; 
	float Scale;
	bool isAnimation;
};

cbuffer JointArr : register(b1)//����Ʈ���� �迭. ����� 65���� �ִ� ����Ʈ
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
	float3 PosL  : POSITION;//��ġ
	float4 Normal : NORMAL;//���
	float2 Tex : TEXTURE;//�ؽ�ó ��ǥ
	float StartWeight : STARTWEIGHT;//������ ����Ʈ �ε���
	int WeightCount : WEIGHTCOUNT;//����Ʈ � �ִ°�?
	float4 JointID : JOINTID;//�ش� ���ؽ��� ������ ����Ʈ ���̵�
	float4 WeightPos1 : WEIGHTPOS0;//����Ʈ�� �����ǵ�. �ִ� 4���� ������ w���ڰ� ���̾��. 
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

	if (isAnimation)//�ִϸ��̼� ������Ʈ�϶��� �����.
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
		//��������� �ִϸ��̼� ó�� �Ϸ�
	}
	//---------------------���⼭ ���� ���� --------------------------


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


