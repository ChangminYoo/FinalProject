
#include "FBXLoader.h"

// ���������� ȹ��
static void GetFBXVertexData(FbxMesh* pMesh, VertexDataArray& outVertexData);

// ��Ƽ���� ǥ��
static void GetMatrialData(FbxSurfaceMaterial* mat, MatDataArray& outMatData);

// �޽�ó��(����Լ�)
static void GetMeshData(FbxNode *child, VertexDataArray& outVertexData, MatDataArray& outMatData);

// ���͸��� Ư�� ȹ��
FbxDouble3 GetMaterialProperty(const FbxSurfaceMaterial * pMaterial, const char * pPropertyName, const char * pFactorPropertyName);



void ProcessSkeletonHeirarchy(FbxNode* root);
void ProcessSkeletonHeirarchyre(FbxNode* node, int depth, int index, int parentindex);

unsigned int FindJointIndex(const std::string& jointname);

DirectX::XMMATRIX GetAnimatedMatrix(int index);

//===================================
Skeleton skeleton;

FbxScene *pScene;

std::map<int, int> controlpoints;
//===================================


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
	pScene = FbxScene::Create(pFBXManager, "");

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


	ProcessSkeletonHeirarchy(pScene->GetRootNode());

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

		outVertexData[i].Pos = DirectX::XMFLOAT3(pos.mData[0], pos.mData[1], pos.mData[2]);
		outVertexData[i].Normal = DirectX::XMFLOAT3(normal.mData[0], normal.mData[1], normal.mData[2]);
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
					outVertexData[UVCount].TexC = DirectX::XMFLOAT2(UV.mData[0], 1 - UV.mData[1]);
				}
				UVCount++;
			}
		}
	}
	///////////////////////////////////////////////////////
	const FbxVector4 lT = pMesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = pMesh->GetNode()->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = pMesh->GetNode()->GetGeometricScaling(FbxNode::eSourcePivot);

	FbxAMatrix geometryTransform = FbxAMatrix(lT, lR, lS);

	std::vector<Vertex> meshvertices;

	for (unsigned int deformerIndex = 0; deformerIndex < pMesh->GetDeformerCount(); ++deformerIndex)
	{
		FbxSkin* skin = reinterpret_cast<FbxSkin*>(pMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!skin)
			continue;

		for (unsigned int clusterIndex = 0; clusterIndex < skin->GetClusterCount(); ++clusterIndex)
		{
			FbxCluster* cluster = skin->GetCluster(clusterIndex);
			std::string jointname = cluster->GetLink()->GetName();
			unsigned int jointIndex = FindJointIndex(jointname);
			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			cluster->GetTransformMatrix(transformMatrix);
			cluster->GetTransformLinkMatrix(transformLinkMatrix);
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;

			skeleton.mJoints[jointIndex].mGlobalBindposeInverse = globalBindposeInverseMatrix;
			skeleton.mJoints[jointIndex].mNode = cluster->GetLink();

			for (unsigned int i = 0; i < cluster->GetControlPointIndicesCount(); ++i)
			{
				int vertexid = controlpoints[cluster->GetControlPointIndices()[i]];

				if (outVertexData[vertexid].boneids.x == 0) outVertexData[vertexid].boneids.x = jointIndex;
				if (outVertexData[vertexid].boneids.y == 0) outVertexData[vertexid].boneids.y = jointIndex;
				if (outVertexData[vertexid].boneids.z == 0) outVertexData[vertexid].boneids.z = jointIndex;
				if (outVertexData[vertexid].boneids.w == 0) outVertexData[vertexid].boneids.w = jointIndex;
				if (outVertexData[vertexid].weights.x == 0) outVertexData[vertexid].weights.x = cluster->GetControlPointWeights()[i];
				if (outVertexData[vertexid].weights.y == 0) outVertexData[vertexid].weights.y = cluster->GetControlPointWeights()[i];
				if (outVertexData[vertexid].weights.z == 0) outVertexData[vertexid].weights.z = cluster->GetControlPointWeights()[i];
				if (outVertexData[vertexid].weights.w == 0) outVertexData[vertexid].weights.w = cluster->GetControlPointWeights()[i];
			}

			FbxAnimStack* animstack = pScene->GetSrcObject<FbxAnimStack>(0);
			FbxString animstackname = animstack->GetName();
			FbxTakeInfo* takeinfo = pScene->GetTakeInfo(animstackname);
			FbxTime start = takeinfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeinfo->mLocalTimeSpan.GetStop();
			FbxLongLong animationlength = end.GetFrameCount(FbxTime::eFrames30) - start.GetFrameCount(FbxTime::eFrames30) + 1;
			Keyframe** anim = &skeleton.mJoints[jointIndex].mAnimation;

			for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames30); i <= end.GetFrameCount(FbxTime::eFrames30); ++i)
			{
				FbxTime time;
				time.SetFrame(i, FbxTime::eFrames30);
				*anim = new Keyframe();
				(*anim)->mFrameNum = i;
				FbxAMatrix transformoffset = pMesh->GetNode()->EvaluateGlobalTransform(1.0f) * geometryTransform;
				(*anim)->mGlobalTransform = transformoffset.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(time);
				anim = &((*anim)->mNext);
			}
		}
	}

	///////////////////////////////////////////////////////////

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

// ���͸��� Ư�� ȹ��
FbxDouble3 GetMaterialProperty(const FbxSurfaceMaterial * pMaterial, const char * pPropertyName, const char * pFactorPropertyName)
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

DirectX::XMMATRIX GetAnimatedMatrix(int index)
{
	DirectX::XMMATRIX bonematxm;

	FbxAMatrix bonemat = skeleton.mJoints[index].mGlobalBindposeInverse; //* skeleton.mJoints[0].mAnimation->mGlobalTransform;

	bonematxm  = DirectX::XMMatrixTranslation(bonemat.GetT().mData[0], bonemat.GetT().mData[1], bonemat.GetT().mData[2]);
	bonematxm *= DirectX::XMMatrixRotationX(bonemat.GetR().mData[0]);
	bonematxm *= DirectX::XMMatrixRotationY(bonemat.GetR().mData[1]);
	bonematxm *= DirectX::XMMatrixRotationZ(bonemat.GetR().mData[2]);

	return bonematxm;
} 


void ProcessSkeletonHeirarchy(FbxNode * rootnode)
{
	for (int childindex = 0; childindex < rootnode->GetChildCount(); ++childindex)
	{
		FbxNode *node = rootnode->GetChild(childindex);
		ProcessSkeletonHeirarchyre(node, 0, 0, -1);
	}
}

void ProcessSkeletonHeirarchyre(FbxNode * node, int depth, int index, int parentindex)
{
	if (node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint joint;
		joint.mParentIndex = parentindex;
		joint.mName = node->GetName();
		skeleton.mJoints.push_back(joint);
	}
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		ProcessSkeletonHeirarchyre(node->GetChild(i), depth + 1, skeleton.mJoints.size(), index);
	}
}

unsigned int FindJointIndex(const std::string & jointname)
{
	for (unsigned int i = 0; i < skeleton.mJoints.size(); ++i)
	{
		if (skeleton.mJoints[i].mName == jointname)
		{
			return i;
		}
	}
}