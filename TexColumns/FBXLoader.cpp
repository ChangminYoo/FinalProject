#include <cstdio>
#include <fbxsdk.h>
#include "FBXLoader.h"

// 정점데이터 획득
static void GetFBXVertexData(FbxMesh* pMesh, VertexDataArray& outVertexData);

// 머티리얼 표시
static void GetMatrialData(FbxSurfaceMaterial* mat, MatDataArray& outMatData);

// 메쉬처리(재귀함수)
static void GetMeshData(FbxNode *child, VertexDataArray& outVertexData, MatDataArray& outMatData);

// FBX데이터에서 정점 데이터 변환
// 
//const char* filename = "../datas/box.fbx";
bool LoadFBXConvertToVertexData(const char* filename, VertexDataArray& outVertexData, MatDataArray& outMatData)
{
	//==============================================================================
	// FBX객체 초기화
	//==============================================================================
	// FBX관리자 생성
	FbxManager* pFBXManager = FbxManager::Create();

	// Scene생성
	FbxScene* pScene = FbxScene::Create(pFBXManager, "");

	// FBX의IO구성 객체 생성
	FbxIOSettings *pIO = FbxIOSettings::Create(pFBXManager, IOSROOT);
	pFBXManager->SetIOSettings(pIO);

	// 가져올 객체 생성
	FbxImporter* pImporter = FbxImporter::Create(pFBXManager, "");

	// 파일 가져오기
	if (pImporter->Initialize(filename, -1, pFBXManager->GetIOSettings()) == false)
	{
		return false;
	}

	// Scene에 가져오기
	if (pImporter->Import(pScene) == false)
	{
		return false;
	}

	// 이시점에서 가져온 객체는 필요없음
	pImporter->Destroy();

	//==============================================================================
	// FBX객체의 처리
	//==============================================================================
	// Scene의 모든것을 삼각형화
	FbxGeometryConverter geometryConverte(pFBXManager);
	geometryConverte.Triangulate(pScene, true);

	// 메쉬 처리
	GetMeshData(pScene->GetRootNode(), outVertexData, outMatData);

	//==============================================================================
	// FBX객체 파괴
	//==============================================================================
	pIO->Destroy();
	pScene->Destroy();
	pFBXManager->Destroy();

	getchar();

	return true;
}

// 메쉬처리(재귀함수)
void GetMeshData(FbxNode *parent, VertexDataArray& outVertexData, MatDataArray& outMatData)
{
	// 메쉬만 처리
	int numKids = parent->GetChildCount();
	for (int i = 0; i < numKids; i++)
	{
		FbxNode *child = parent->GetChild(i);

		// 메쉬를 찾으면
		if (child->GetMesh())
		{
			FbxMesh* pMesh = child->GetMesh();// static_cast<FbxMesh*>(child->GetNodeAttribute());
			printf("메쉬발견\n");

			printf("이름:%s\n", pMesh->GetName());
			printf("폴리곤 수:%d\n", pMesh->GetPolygonCount());
			printf("메테리얼 수:%d\n", pMesh->GetElementMaterialCount());

			printf("컨트롤 포인트(정점 좌표):%d\n", pMesh->GetControlPointsCount());
			printf("UV수:%d\n", pMesh->GetTextureUVCount());

			FbxArray<FbxVector4> normals;
			pMesh->GetPolygonVertexNormals(normals);
			printf("법선의 수:%d\n", normals.GetCount());

			// 정점정보 획득
			GetFBXVertexData(pMesh, outVertexData);
		}

		// 자료
		int numMat = child->GetMaterialCount();
		for (int j = 0; j < numMat; ++j)
		{
			FbxSurfaceMaterial* mat = child->GetMaterial(j);
			if (mat)
			{
				GetMatrialData(mat, outMatData);
			}
		}

		if (numMat == 0)
		{
			printf("자료없음\n");
		}

		child->GetChild(0);

		//child 처리 
		GetMeshData(child, outVertexData, outMatData);
	}
}

// 정점 데이터 획득
void GetFBXVertexData(FbxMesh* pMesh, VertexDataArray& outVertexData)
{
	// 정점 좌표와 법선 벡터 획득 
	std::vector<FbxVector4> positions, normals, indexes;
	FbxVector4 normal;

	//GetPolygonCount = 면수 ,GetPolygonSize = 정점수
	for (int i = 0; i < pMesh->GetPolygonCount(); i++)
	{
		for (int j = 0; j < pMesh->GetPolygonSize(i); j++)
		{
			// 정점좌표
			positions.push_back(pMesh->GetControlPointAt(pMesh->GetPolygonVertex(i, j)));


			// 법선벡터
			pMesh->GetPolygonVertexNormal(i, j, normal);
			normals.push_back(normal);
		}
	}

	// 배열 초기화
	outVertexData.resize(positions.size());

	// 정점 및 일반 정보를 저장
	for (size_t i = 0; i < outVertexData.size(); ++i)
	{
		fbxsdk::FbxVector4& pos = positions[i];
		fbxsdk::FbxVector4& normal = normals[i];

		outVertexData[i].pos = DirectX::XMFLOAT3(pos.mData[0], pos.mData[1], pos.mData[2]);
		outVertexData[i].normal = DirectX::XMFLOAT4(normal.mData[0], normal.mData[1], normal.mData[2], normal.mData[3]);
	}

	//인덱스 정보

	for (int i = 0; i < pMesh->GetPolygonCount(); i++)
	{
		outVertexData[i * 3 + 0].index = i * 3 + 0;
		outVertexData[i * 3 + 1].index = i * 3 + 1;
		outVertexData[i * 3 + 2].index = i * 3 + 2;
	}

	// UV세트의 이름 배열 획득
	FbxStringList uvSetNames;
	pMesh->GetUVSetNames(uvSetNames);

	bool unmapped = false;
	int UVCount = 0;

	for (int i = 0; i < uvSetNames.GetCount(); ++i)
	{
		for (int j = 0; j < pMesh->GetPolygonCount(); ++j)
		{
			for (int k = 0; k < pMesh->GetPolygonSize(j); ++k)
			{
				FbxVector2 UV;
				pMesh->GetPolygonVertexUV(j, k, uvSetNames.GetStringAt(i), UV, unmapped);

				if (outVertexData.size() > UVCount)
				{
					outVertexData[UVCount].uv = DirectX::XMFLOAT2(UV.mData[0], UV.mData[1]);
				}
				UVCount++;
			}
		}
	}
}

// 머터리얼 특성 획득
FbxDouble3 GetMaterialProperty(
	const FbxSurfaceMaterial * pMaterial,
	const char * pPropertyName,
	const char * pFactorPropertyName)
{
	FbxDouble3 lResult(0, 0, 0);
	const FbxProperty lProperty = pMaterial->FindProperty(pPropertyName);
	const FbxProperty lFactorProperty = pMaterial->FindProperty(pFactorPropertyName);
	if (lProperty.IsValid() && lFactorProperty.IsValid())
	{
		lResult = lProperty.Get<FbxDouble3>();
		double lFactor = lFactorProperty.Get<FbxDouble>();
		if (lFactor != 1)
		{
			lResult[0] *= lFactor;
			lResult[1] *= lFactor;
			lResult[2] *= lFactor;
		}
	}

	if (lProperty.IsValid())
	{
		printf("텍스쳐\n");
		const int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
		for (int i = 0; i<lTextureCount; i++)
		{
			FbxFileTexture* lFileTexture = lProperty.GetSrcObject<FbxFileTexture>(i);
			if (lFileTexture)
			{
				FbxString uvsetName = lFileTexture->UVSet.Get();
				std::string uvSetString = uvsetName.Buffer();
				std::string filepath = lFileTexture->GetFileName();

				printf("UVSet이름=%s\n", uvSetString.c_str());
				printf("텍스쳐 이름=%s\n", filepath.c_str());
			}
		}
		puts("");

		printf("레이어드 텍스쳐\n");
		const int lLayeredTextureCount = lProperty.GetSrcObjectCount<FbxLayeredTexture>();
		for (int i = 0; i<lLayeredTextureCount; i++)
		{
			FbxLayeredTexture* lLayeredTexture = lProperty.GetSrcObject<FbxLayeredTexture>(i);

			const int lTextureFileCount = lLayeredTexture->GetSrcObjectCount<FbxFileTexture>();

			for (int j = 0; j<lTextureFileCount; j++)
			{
				FbxFileTexture* lFileTexture = lLayeredTexture->GetSrcObject<FbxFileTexture>(j);
				if (lFileTexture)
				{
					FbxString uvsetName = lFileTexture->UVSet.Get();
					std::string uvSetString = uvsetName.Buffer();
					std::string filepath = lFileTexture->GetFileName();

					printf("UVSet이름=%s\n", uvSetString.c_str());
					printf("텍스쳐 이름=%s\n", filepath.c_str());
				}
			}
		}
		puts("");
	}

	return lResult;
}

// 머터리얼 표시
void GetMatrialData(FbxSurfaceMaterial* mat, MatDataArray& outMatData)
{
	if (mat == nullptr)
	{
		return;
	}

	puts("");
	if (mat->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		printf("램버트 타입\n");
	}
	else if (mat->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		printf("퐁 타입\n");
	}

	const FbxDouble3 lEmissive = GetMaterialProperty(mat, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);
	printf("이미시브컬러:r = %f, g = %f, b = %f\n", lEmissive.mData[0], lEmissive.mData[1], lEmissive.mData[2]);

	const FbxDouble3 lAmbient = GetMaterialProperty(mat, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	printf("엠비언트색상:r = %f, g = %f, b = %f\n", lAmbient.mData[0], lAmbient.mData[1], lAmbient.mData[2]);

	const FbxDouble3 lDiffuse = GetMaterialProperty(mat, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	printf("디퓨즈컬러:r = %f, g = %f, b = %f\n", lDiffuse.mData[0], lDiffuse.mData[1], lDiffuse.mData[2]);

	const FbxDouble3 lSpecular = GetMaterialProperty(mat, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	printf("반사컬러:r = %f, g = %f, b = %f\n", lSpecular.mData[0], lSpecular.mData[1], lSpecular.mData[2]);

	FbxProperty lTransparencyFactorProperty = mat->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	if (lTransparencyFactorProperty.IsValid())
	{
		double lTransparencyFactor = lTransparencyFactorProperty.Get<FbxDouble>();
		printf("투명도 = %lf\n", lTransparencyFactor);
	}

	FbxProperty lShininessProperty = mat->FindProperty(FbxSurfaceMaterial::sShininess);
	if (lShininessProperty.IsValid())
	{
		double lShininess = lShininessProperty.Get<FbxDouble>();
		printf("반사 = %lf\n", lShininess);
	}
}
