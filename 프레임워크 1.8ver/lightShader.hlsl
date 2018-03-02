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
	float4 DiffuseAlbedo;  //분산
	float3 FresnelR0;      //프레넬
	float Shininess;       //반짝임
};



//--------------------------
// 프레넬 방정식의 슐릭 근사를 구한다. 법선이 n인 표면에서
// 프레넬 효과에 반사되는 빛의 비율을 빛 벡터 L과 표면법선 n사이의 각도에
// 근거해서 근사한다.
// 슐릭근사는 프레넬 반사의 근삿값을 제공한다.
// R0 = ( (n-1)/(n+1) )^2, 여기서 n은 굴절 지수이다.
//-------------------------
float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
	float cosIncidentAngle = saturate(dot(normal, lightVec));

	float f0 = 1.0f - cosIncidentAngle;
	float3 reflectPercent = R0 + (1.0f - R0)*(f0*f0*f0*f0*f0);

	return reflectPercent;
}


//-----------------------------
// 눈에 도달한 반사광의 양을 계산. 즉, 분산반사와 반영반사의 합을 구한다.
//-----------------------------
float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{
	//loughness에서 얻은 광택도로부터 m을 유도
	const float m = mat.Shininess * 256.0f;
	float3 halfVec = normalize(toEye + lightVec);

	float roughnessFactor = (m + 8.0f)*pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
	float3 fresnelFactor = SchlickFresnel(mat.FresnelR0, halfVec, lightVec);

	float3 specAlbedo = fresnelFactor*roughnessFactor;

	// 반영반사율 공식이 [0,1] 구간의 바깥의 값을 낼 수도 있지만, 
	// 우리는 LDR랜더링을 구현하므로, 반사율을 1 미만으로 낮춘다.
	specAlbedo = specAlbedo / (specAlbedo + 1.0f);

	return (mat.DiffuseAlbedo.rgb + specAlbedo) * lightStrength;
}

//--------------------------------
//지향광 계산
//--------------------------------
float3 ComputeDirectionalLight(Light L, Material mat, float3 normal, float3 toEye)
{

	// 빛 벡터는 광선들이 나아가는 방향의 반대 방향을 가리킨다.
	float3 lightVec = -L.Direction;

	// 람베르트 코사인법칙에 따라 빛 세기를 줄인다.
	float ndotl = max(dot(lightVec, normal), 0.0f);

	float3 lightStrength = L.Color * ndotl;

	return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);

}

float4 ComputeLighting(Light gLights[LIGHT_MAX], Material mat, float3 pos, float3 normal, float3 toEye, float3 shadowFactor)
{
	float3 result = { 0.0f,0.0f,0.0f };

	int i = 0;

	
	//다른 광원이 추가되면 여기 또 추가해줘야함
	result += shadowFactor[0] * ComputeDirectionalLight(gLights[0], mat, normal, toEye);


	return float4(result, 0.0f);
}

