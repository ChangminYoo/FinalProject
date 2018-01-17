#include <cstdio>
#include <fbxsdk.h>
#include "FBXLoader.h"

// ���������� ȹ��
static void GetFBXVertexData(FbxMesh* pMesh, VertexDataArray& outVertexData);

// ��Ƽ���� ǥ��
static void GetMatrialData(FbxSurfaceMaterial* mat, MatDataArray& outMatData);

// �޽�ó��(����Լ�)
static void GetMeshData(FbxNode *child, VertexDataArray& outVertexData, MatDataArray& outMatData);

// FBX�����Ϳ��� ���� ������ ��ȯ
// 
//const char* filename = "../datas/box.fbx";
bool LoadFBXConvertToVertexData(const char* filename, VertexDataArray& outVertexData, MatDataArray& outMatData)
{
	//==============================================================================
	// FBX��ü �ʱ�ȭ
	//==============================================================================
	// FBX������ ����
	FbxManager* pFBXManager = FbxManager::Create();

	// Scene����
	FbxScene* pScene = FbxScene::Create(pFBXManager, "");

	// FBX��IO���� ��ü ����
	FbxIOSettings *pIO = FbxIOSettings::Create(pFBXManager, IOSROOT);
	pFBXManager->SetIOSettings(pIO);

	// ������ ��ü ����
	FbxImporter* pImporter = FbxImporter::Create(pFBXManager, "");

	// ���� ��������
	if (pImporter->Initialize(filename, -1, pFBXManager->GetIOSettings()) == false)
	{
		return false;
	}

	// Scene�� ��������
	if (pImporter->Import(pScene) == false)
	{
		return false;
	}

	// �̽������� ������ ��ü�� �ʿ����
	pImporter->Destroy();

	//==============================================================================
	// FBX��ü�� ó��
	//==============================================================================
	// Scene�� ������ �ﰢ��ȭ
	FbxGeometryConverter geometryConverte(pFBXManager);
	geometryConverte.Triangulate(pScene, true);

	// �޽� ó��
	GetMeshData(pScene->GetRootNode(), outVertexData, outMatData);

	//==============================================================================
	// FBX��ü �ı�
	//==============================================================================
	pIO->Destroy();
	pScene->Destroy();
	pFBXManager->Destroy();

	getchar();

	return true;
}

// �޽�ó��(����Լ�)
void GetMeshData(FbxNode *parent, VertexDataArray& outVertexData, MatDataArray& outMatData)
{
	// �޽��� ó��
	int numKids = parent->GetChildCount();
	for (int i = 0; i < numKids; i++)
	{
		FbxNode *child = parent->GetChild(i);

		// �޽��� ã����
		if (child->GetMesh())
		{
			FbxMesh* pMesh = child->GetMesh();// static_cast<FbxMesh*>(child->GetNodeAttribute());
			printf("�޽��߰�\n");

			printf("�̸�:%s\n", pMesh->GetName());
			printf("������ ��:%d\n", pMesh->GetPolygonCount());
			printf("���׸��� ��:%d\n", pMesh->GetElementMaterialCount());

			printf("��Ʈ�� ����Ʈ(���� ��ǥ):%d\n", pMesh->GetControlPointsCount());
			printf("UV��:%d\n", pMesh->GetTextureUVCount());

			FbxArray<FbxVector4> normals;
			pMesh->GetPolygonVertexNormals(normals);
			printf("������ ��:%d\n", normals.GetCount());

			// �������� ȹ��
			GetFBXVertexData(pMesh, outVertexData);
		}

		// �ڷ�
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
			printf("�ڷ����\n");
		}

		child->GetChild(0);

		//child ó�� 
		GetMeshData(child, outVertexData, outMatData);
	}
}

// ���� ������ ȹ��
void GetFBXVertexData(FbxMesh* pMesh, VertexDataArray& outVertexData)
{
	// ���� ��ǥ�� ���� ���� ȹ�� 
	std::vector<FbxVector4> positions, normals, indexes;
	FbxVector4 normal;

	//GetPolygonCount = ��� ,GetPolygonSize = ������
	for (int i = 0; i < pMesh->GetPolygonCount(); i++)
	{
		for (int j = 0; j < pMesh->GetPolygonSize(i); j++)
		{
			// ������ǥ
			positions.push_back(pMesh->GetControlPointAt(pMesh->GetPolygonVertex(i, j)));


			// ��������
			pMesh->GetPolygonVertexNormal(i, j, normal);
			normals.push_back(normal);
		}
	}

	// �迭 �ʱ�ȭ
	outVertexData.resize(positions.size());

	// ���� �� �Ϲ� ������ ����
	for (size_t i = 0; i < outVertexData.size(); ++i)
	{
		fbxsdk::FbxVector4& pos = positions[i];
		fbxsdk::FbxVector4& normal = normals[i];

		outVertexData[i].pos = DirectX::XMFLOAT3(pos.mData[0], pos.mData[1], pos.mData[2]);
		outVertexData[i].normal = DirectX::XMFLOAT4(normal.mData[0], normal.mData[1], normal.mData[2], normal.mData[3]);
	}

	//�ε��� ����

	for (int i = 0; i < pMesh->GetPolygonCount(); i++)
	{
		outVertexData[i * 3 + 0].index = i * 3 + 0;
		outVertexData[i * 3 + 1].index = i * 3 + 1;
		outVertexData[i * 3 + 2].index = i * 3 + 2;
	}

	// UV��Ʈ�� �̸� �迭 ȹ��
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

// ���͸��� Ư�� ȹ��
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
		printf("�ؽ���\n");
		const int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
		for (int i = 0; i<lTextureCount; i++)
		{
			FbxFileTexture* lFileTexture = lProperty.GetSrcObject<FbxFileTexture>(i);
			if (lFileTexture)
			{
				FbxString uvsetName = lFileTexture->UVSet.Get();
				std::string uvSetString = uvsetName.Buffer();
				std::string filepath = lFileTexture->GetFileName();

				printf("UVSet�̸�=%s\n", uvSetString.c_str());
				printf("�ؽ��� �̸�=%s\n", filepath.c_str());
			}
		}
		puts("");

		printf("���̾�� �ؽ���\n");
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

					printf("UVSet�̸�=%s\n", uvSetString.c_str());
					printf("�ؽ��� �̸�=%s\n", filepath.c_str());
				}
			}
		}
		puts("");
	}

	return lResult;
}

// ���͸��� ǥ��
void GetMatrialData(FbxSurfaceMaterial* mat, MatDataArray& outMatData)
{
	if (mat == nullptr)
	{
		return;
	}

	puts("");
	if (mat->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		printf("����Ʈ Ÿ��\n");
	}
	else if (mat->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		printf("�� Ÿ��\n");
	}

	const FbxDouble3 lEmissive = GetMaterialProperty(mat, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);
	printf("�̹̽ú��÷�:r = %f, g = %f, b = %f\n", lEmissive.mData[0], lEmissive.mData[1], lEmissive.mData[2]);

	const FbxDouble3 lAmbient = GetMaterialProperty(mat, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	printf("�����Ʈ����:r = %f, g = %f, b = %f\n", lAmbient.mData[0], lAmbient.mData[1], lAmbient.mData[2]);

	const FbxDouble3 lDiffuse = GetMaterialProperty(mat, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	printf("��ǻ���÷�:r = %f, g = %f, b = %f\n", lDiffuse.mData[0], lDiffuse.mData[1], lDiffuse.mData[2]);

	const FbxDouble3 lSpecular = GetMaterialProperty(mat, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	printf("�ݻ��÷�:r = %f, g = %f, b = %f\n", lSpecular.mData[0], lSpecular.mData[1], lSpecular.mData[2]);

	FbxProperty lTransparencyFactorProperty = mat->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	if (lTransparencyFactorProperty.IsValid())
	{
		double lTransparencyFactor = lTransparencyFactorProperty.Get<FbxDouble>();
		printf("���� = %lf\n", lTransparencyFactor);
	}

	FbxProperty lShininessProperty = mat->FindProperty(FbxSurfaceMaterial::sShininess);
	if (lShininessProperty.IsValid())
	{
		double lShininess = lShininessProperty.Get<FbxDouble>();
		printf("�ݻ� = %lf\n", lShininess);
	}
}
