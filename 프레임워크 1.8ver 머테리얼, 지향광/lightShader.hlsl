#ifndef NUM_DIR_LIGHTS
#define NUM_DIR_LIGHTS 3
#endif

#ifndef NUM_POINT_LIGHTS
#define NUM_POINT_LIGHTS 0
#endif

#ifndef NUM_SPOT_LIGHTS
#define NUM_SPOT_LIGHTS 0
#endif

#define LIGHT_MAX 10

struct Light
{
	float3 Color;
	float3 Direction;
	float3 Position;
	float SpotPower;
};

struct Material
{
	float4 DiffuseAlbedo;  //�л�
	float3 FresnelR0;      //������
	float Shininess;       //��¦��
};



//--------------------------
// ������ �������� ���� �ٻ縦 ���Ѵ�. ������ n�� ǥ�鿡��
// ������ ȿ���� �ݻ�Ǵ� ���� ������ �� ���� L�� ǥ����� n������ ������
// �ٰ��ؼ� �ٻ��Ѵ�.
// �����ٻ�� ������ �ݻ��� �ٻ��� �����Ѵ�.
// R0 = ( (n-1)/(n+1) )^2, ���⼭ n�� ���� �����̴�.
//-------------------------
float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
	float cosIncidentAngle = saturate(dot(normal, lightVec));

	float f0 = 1.0f - cosIncidentAngle;
	float3 reflectPercent = R0 + (1.0f - R0)*(f0*f0*f0*f0*f0);

	return reflectPercent;
}


//-----------------------------
// ���� ������ �ݻ籤�� ���� ���. ��, �л�ݻ�� �ݿ��ݻ��� ���� ���Ѵ�.
//-----------------------------
float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{
	//loughness���� ���� ���õ��κ��� m�� ����
	const float m = mat.Shininess * 256.0f;
	float3 halfVec = normalize(toEye + lightVec);

	float roughnessFactor = (m + 8.0f)*pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
	float3 fresnelFactor = SchlickFresnel(mat.FresnelR0, halfVec, lightVec);

	float3 specAlbedo = fresnelFactor*roughnessFactor;

	// �ݿ��ݻ��� ������ [0,1] ������ �ٱ��� ���� �� ���� ������, 
	// �츮�� LDR�������� �����ϹǷ�, �ݻ����� 1 �̸����� �����.
	specAlbedo = specAlbedo / (specAlbedo + 1.0f);

	return (mat.DiffuseAlbedo.rgb + specAlbedo) * lightStrength;
}

//--------------------------------
//���Ɽ ���
//--------------------------------
float3 ComputeDirectionalLight(Light L, Material mat, float3 normal, float3 toEye)
{

	// �� ���ʹ� �������� ���ư��� ������ �ݴ� ������ ����Ų��.
	float3 lightVec = -L.Direction;

	// ������Ʈ �ڻ��ι�Ģ�� ���� �� ���⸦ ���δ�.
	float ndotl = max(dot(lightVec, normal), 0.0f);

	float3 lightStrength = L.Color * ndotl;

	return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);

}

float4 ComputeLighting(Light gLights[LIGHT_MAX], Material mat, float3 pos, float3 normal, float3 toEye, float3 shadowFactor)
{
	float3 result = { 0.0f,0.0f,0.0f };

	int i = 0;

	
	//�ٸ� ������ �߰��Ǹ� ���� �� �߰��������
	result += shadowFactor[0] * ComputeDirectionalLight(gLights[0], mat, normal, toEye);


	return float4(result, 0.0f);
}

