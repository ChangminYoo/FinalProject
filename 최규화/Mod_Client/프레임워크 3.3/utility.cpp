
#include "utility.h"
#include<fstream>
#include<string>
#include"CMesh.h"
#include"CGameObject.h"
using namespace std;

bool LoadMD5Model(std::wstring filename, CMesh* MD5Model, int numberofMesh, float scale)
{
	std::wifstream fileIn(filename.c_str());
	int qi = 0;
	std::wstring checkString;
	if (fileIn)
	{
		while (fileIn)
		{
			int num = 0;

			fileIn >> checkString;
			if (checkString == L"MD5Version")
			{

			}
			else if (checkString == L"commandline") {
				std::getline(fileIn, checkString);
			}
			else if (checkString == L"numJoints")
			{
				fileIn >> MD5Model->numJoints;
				MD5Model->joints.reserve(MD5Model->numJoints + 1);

			}
			else if (checkString == L"numMeshes")
			{
				fileIn >> num;
			}
			//여기까지가 헤더파일을 읽는것.

			//여기서 부터 본격적인 데이터를 읽어들임


			else if (checkString == L"joints")
			{
				Joint tempJoint;
				fileIn >> checkString;// Skip the "{" 

				for (int i = 0; i < MD5Model->numJoints; i++)
				{
					fileIn >> tempJoint.name;

					if (tempJoint.name[tempJoint.name.size() - 1] != '"')
					{
						wchar_t checkChar;
						bool jointNameFound = false;
						while (!jointNameFound) {
							checkChar = fileIn.get();
							if (checkChar == '"')
								jointNameFound = true;
							tempJoint.name += checkChar;
						}
					}
					fileIn >> tempJoint.parentID;
					fileIn >> checkString; // Skip the "(" 
					fileIn >> tempJoint.pos.x >> tempJoint.pos.z >> tempJoint.pos.y; fileIn >> checkString >> checkString;

					fileIn >> tempJoint.orientation.x >> tempJoint.orientation.z >> tempJoint.orientation.y;
					tempJoint.name.erase(0, 1);
					tempJoint.name.erase(tempJoint.name.size() - 1, 1);

					float t = 1.0f - (tempJoint.orientation.x * tempJoint.orientation.x) - (tempJoint.orientation.y * tempJoint.orientation.y) - (tempJoint.orientation.z * tempJoint.orientation.z);
					if (t < 0.0f)
					{
						tempJoint.orientation.w = 0.0f;
					}
					else
					{
						tempJoint.orientation.w = -sqrtf(t);
					}
					std::getline(fileIn, checkString); // Skip rest of this line 

					MD5Model->joints.emplace_back(tempJoint);
					// Store the joint into this models joint vector 

				}
				fileIn >> checkString; // Skip the "}"

			}

			else if (checkString == L"mesh") {
				CMesh subset;
				int numVerts, numTris, numWeights;
				fileIn >> checkString;
				// Skip the "{" 
				fileIn >> checkString;
				while (checkString != L"}") // Read until '}' 
				{
					if (checkString == L"shader") // Load the texture or material 
					{
						std::wstring fileNamePath;
						fileIn >> fileNamePath;
						// Get texture's filename  
						if (fileNamePath[fileNamePath.size() - 1] != '"')
						{
							wchar_t checkChar;
							bool fileNameFound = false;
							while (!fileNameFound)
							{
								checkChar = fileIn.get();
								if (checkChar == '"')
									fileNameFound = true;
								fileNamePath += checkChar;
							}
						}
						fileNamePath.erase(0, 1);
						fileNamePath.erase(fileNamePath.size() - 1, 1);

						bool alreadyLoaded = false;

						if (!alreadyLoaded)
						{


						}
						std::getline(fileIn, checkString);

					}
					else if (checkString == L"numverts")
					{

						fileIn >> numVerts;
						//1차적으로 버텍스배열생성
						if (qi == numberofMesh)
						{
							MD5Model->SubResource = new CVertex[numVerts];
							MD5Model->nVertex = numVerts;
							MD5Model->nStride = sizeof(CVertex);
							MD5Model->nOffset = 0;
						}
						std::getline(fileIn, checkString);
						for (int i = 0; i < numVerts; i++)
						{
							CVertex tempVert;
							fileIn >> checkString >> checkString >> checkString;
							fileIn >> tempVert.Tex.x >> tempVert.Tex.y;
							//tempVert.Tex.z = spec;
							fileIn >> checkString;
							fileIn >> tempVert.StartWeight;
							fileIn >> tempVert.WeightCount;
							std::getline(fileIn, checkString);
							if (qi == numberofMesh)
							{

								MD5Model->SubResource[i] = tempVert;

							}
						}
					}
					else if (checkString == L"numtris")
					{
						fileIn >> numTris;
						//2차적으로 인덱스배열생성
						if (qi == numberofMesh)
						{
							MD5Model->Index = new UINT[numTris * 3];
							MD5Model->nindex = numTris * 3;
							MD5Model->nioffset = 0;
							MD5Model->nisize = sizeof(UINT);
						}
						subset.numTriangles = numTris;
						std::getline(fileIn, checkString);
						for (int i = 0; i < numTris; i++)
						{
							DWORD tempIndex;
							fileIn >> checkString;
							fileIn >> checkString;
							for (int k = 0; k < 3; k++)
							{
								fileIn >> tempIndex;
								if (qi == numberofMesh)
									MD5Model->Index[3 * i + k] = tempIndex;
							}
							std::getline(fileIn, checkString);
						}
					}
					else if (checkString == L"numweights")
					{
						fileIn >> numWeights;
						std::getline(fileIn, checkString);
						for (int i = 0; i < numWeights; i++)
						{
							Weight tempWeight;
							fileIn >> checkString >> checkString;
							fileIn >> tempWeight.jointID;
							fileIn >> tempWeight.bias;
							fileIn >> checkString;
							fileIn >> tempWeight.pos.x >> tempWeight.pos.z >> tempWeight.pos.y; std::getline(fileIn, checkString);
							if (qi == numberofMesh)
								MD5Model->weights.emplace_back(tempWeight);
						}
					}
					else std::getline(fileIn, checkString);
					fileIn >> checkString;


				}
				if (qi == numberofMesh)
				{
					for (UINT i = 0; i < MD5Model->nVertex; ++i)
					{
						//여기서 정점의 위치를 계산함.

						CVertex tempVert = MD5Model->SubResource[i];
						tempVert.V = XMFLOAT3(0, 0, 0);
						for (int j = 0; j < tempVert.WeightCount; ++j)
						{
							Weight tempWeight = MD5Model->weights[tempVert.StartWeight + j];
							Joint tempJoint = MD5Model->joints[tempWeight.jointID];

							XMVECTOR tempJointOrientation = XMVectorSet(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w); 
							XMVECTOR tempWeightPos = XMVectorSet(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, 0.0f);

							XMVECTOR tempJointOrientationConjugate = XMVectorSet(-tempJoint.orientation.x, -tempJoint.orientation.y, -tempJoint.orientation.z, tempJoint.orientation.w);

							XMFLOAT3 rotatedPoint; 
							XMStoreFloat3(&rotatedPoint, XMQuaternionMultiply(XMQuaternionMultiply(tempJointOrientation, tempWeightPos), tempJointOrientationConjugate));

							
							tempVert.V.x += (tempJoint.pos.x + rotatedPoint.x) * tempWeight.bias;
							tempVert.V.y += (tempJoint.pos.y + rotatedPoint.y) * tempWeight.bias;
							tempVert.V.z += (tempJoint.pos.z + rotatedPoint.z) * tempWeight.bias;

							
							MD5Model->SubResource[i].JointID[j] = tempWeight.jointID;
							
							switch (j)
							{
							case 0:
								MD5Model->SubResource[i].WeightPos1 = XMFLOAT4(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, tempWeight.bias);
								break;
							case 1:
								MD5Model->SubResource[i].WeightPos2 = XMFLOAT4(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, tempWeight.bias);
								break;
							case 2:
								MD5Model->SubResource[i].WeightPos3 = XMFLOAT4(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, tempWeight.bias);
								break;
							case 3:
								MD5Model->SubResource[i].WeightPos4 = XMFLOAT4(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, tempWeight.bias);
								break;

							}
							
						}
						MD5Model->SubResource[i].V = XMFLOAT3(tempVert.V.x * scale, tempVert.V.y * scale, tempVert.V.z * scale);
					}

					return true;
				}
				else
					qi++;

			}

		}
	}
	return true;
}




bool LoadMD5Anim(ID3D12Device* m_Device, std::wstring filename, CMesh* MD5Model, CGameObject* obj, vector<ModelAnimation>& av, int numberofMesh)
{

	obj->ObjData.isAnimation = true;
	int qi = 0;
	ModelAnimation tempAnim; // 애니메이션 데이터 임시저장공간
	std::wifstream fileIn(filename.c_str()); // Open file
	std::wstring checkString; // Stores the next string from our file
	if (fileIn) // Check if the file was opened 
	{
		while (fileIn) // Loop until the end of the file is reached
		{
			fileIn >> checkString; // Get next string from file
			if (checkString == L"MD5Version") // Get MD5 version (this function supports version 10) 
			{
				fileIn >> checkString; /*MessageBox(0, checkString.c_str(), //display message L"MD5Version", MB_OK);*/
			}
			else if (checkString == L"commandline")
			{
				std::getline(fileIn, checkString); // Ignore the rest of this line 
			}
			else if (checkString == L"numFrames")
			{
				fileIn >> tempAnim.numFrames; // Store number of frames in this animation 
			}
			else if (checkString == L"numJoints")
			{
				fileIn >> tempAnim.numJoints; // Store number of joints (must match .md5mesh) 

			}
			else if (checkString == L"frameRate")
			{
				fileIn >> tempAnim.frameRate; // Store animation's frame rate (frames per second)
			}
			else if (checkString == L"numAnimatedComponents")
			{
				fileIn >> tempAnim.numAnimatedComponents; // Number of components in each frame section 
			}
			else if (checkString == L"hierarchy")
			{
				fileIn >> checkString; // Skip opening bracket "{" 
				for (int i = 0; i < tempAnim.numJoints; i++) // Load in each joint 
				{
					AnimJointInfo tempJoint;
					fileIn >> tempJoint.name; // Get joints name 
					if (tempJoint.name[tempJoint.name.size() - 1] != '"')
					{
						wchar_t checkChar;
						bool jointNameFound = false;
						while (!jointNameFound)
						{
							checkChar = fileIn.get();
							if (checkChar == '"')
								jointNameFound = true;
							tempJoint.name += checkChar;
						}
					} // Remove the quotation marks from joints name 
					tempJoint.name.erase(0, 1);
					tempJoint.name.erase(tempJoint.name.size() - 1, 1);
					fileIn >> tempJoint.parentID; // Get joints parent ID 
					fileIn >> tempJoint.flags; // Get flags 
					fileIn >> tempJoint.startIndex; // Get joints start index 
					bool jointMatchFound = false;
					for (int k = 0; k < MD5Model->numJoints; k++)
					{
						if (MD5Model->joints[k].name == tempJoint.name)
						{
							if (MD5Model->joints[k].parentID == tempJoint.parentID)
							{
								jointMatchFound = true;
								tempAnim.jointInfo.emplace_back(tempJoint);
							}
						}
					}
					if (!jointMatchFound) // If the skeleton system does not match up, 
						return false; // You might want to add an error message here
					std::getline(fileIn, checkString); // Skip rest of this line 
				}
			}
			else if (checkString == L"bounds") // Load in the AABB for each animation 
			{
				fileIn >> checkString; // Skip opening bracket "{" 
				for (int i = 0; i < tempAnim.numFrames; i++)
				{
					Boundingbox tempBB;
					fileIn >> checkString; // Skip "(" 
					fileIn >> tempBB.min.x >> tempBB.min.z >> tempBB.min.y;
					fileIn >> checkString >> checkString; // Skip ") ("
					fileIn >> tempBB.max.x >> tempBB.max.z >> tempBB.max.y;
					fileIn >> checkString; // Skip ")" 
										   //내가만든 고유의 데이터를쓸거므로 제외
										   //		tempAnim.frameBounds.emplace_back(tempBB); 
				}
			}
			else if (checkString == L"baseframe")
			{
				fileIn >> checkString; // Skip opening bracket "{" 
				for (int i = 0; i < tempAnim.numJoints; i++)
				{
					Joint tempBFJ; fileIn >> checkString; // Skip "(" 
					fileIn >> tempBFJ.pos.x >> tempBFJ.pos.z >> tempBFJ.pos.y;
					fileIn >> checkString >> checkString; // Skip ") (" 
					fileIn >> tempBFJ.orientation.x >> tempBFJ.orientation.z >> tempBFJ.orientation.y;
					fileIn >> checkString; // Skip ")" 
					tempAnim.baseFrameJoints.emplace_back(tempBFJ);
				}
			}
			else if (checkString == L"frame") // Load in each frames skeleton (the parts of each joint that changed from the base frame) 
			{
				FrameData tempFrame;
				fileIn >> tempFrame.frameID; // Get the frame ID 
				fileIn >> checkString; // Skip opening bracket "{" 
				for (int i = 0; i < tempAnim.numAnimatedComponents; i++)
				{
					float tempData;
					fileIn >> tempData; // Get the data 
					tempFrame.frameData.emplace_back(tempData);
				}
				tempAnim.frameData.emplace_back(tempFrame); ///*** build the frame skeleton ***/// 
				std::vector<Joint> tempSkeleton;
				for (int i = 0; i < tempAnim.jointInfo.size(); i++)
				{
					int k = 0; // Keep track of position in frameData array 
					Joint tempFrameJoint = tempAnim.baseFrameJoints[i];
					tempFrameJoint.parentID = tempAnim.jointInfo[i].parentID; // Notice how I have been flipping y and z. this is because some modeling programs such as // 3ds max (which is what I use) use a right handed coordinate system. Because of this, we // need to flip the y and z axes. If your having problems loading some models, it's possible // the model was created in a left hand coordinate system. in that case, just reflip all the // y and z axes in our md5 mesh and anim loader.
					if (tempAnim.jointInfo[i].flags & 1) // pos.x ( 000001 ) 
						tempFrameJoint.pos.x = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];
					if (tempAnim.jointInfo[i].flags & 2) // pos.y ( 000010 )
						tempFrameJoint.pos.z = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];
					if (tempAnim.jointInfo[i].flags & 4) // pos.z ( 000100 ) 
						tempFrameJoint.pos.y = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];
					if (tempAnim.jointInfo[i].flags & 8) // orientation.x ( 001000 ) 
						tempFrameJoint.orientation.x = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];
					if (tempAnim.jointInfo[i].flags & 16) // orientation.y ( 010000 ) 
						tempFrameJoint.orientation.z = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];
					if (tempAnim.jointInfo[i].flags & 32) // orientation.z ( 100000 ) 
						tempFrameJoint.orientation.y = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];
					// Compute the quaternions w 
					float t = 1.0f - (tempFrameJoint.orientation.x * tempFrameJoint.orientation.x) - (tempFrameJoint.orientation.y * tempFrameJoint.orientation.y) - (tempFrameJoint.orientation.z * tempFrameJoint.orientation.z);
					if (t < 0.0f) {
						tempFrameJoint.orientation.w = 0.0f;
					}
					else
					{
						tempFrameJoint.orientation.w = -sqrtf(t);
					} // Now, if the upper arm of your skeleton moves, you need to also move the lower part of your arm, and then the hands, and then finally the fingers (possibly weapon or tool too) // This is where joint hierarchy comes in. We start at the top of the hierarchy, and move down to each joints child, rotating and translating them based on their parents rotation // and translation. We can assume that by the time we get to the child, the parent has already been rotated and transformed based of it's parent. We can assume this because // the child should never come before the parent in the files we loaded in. 
					if (tempFrameJoint.parentID >= 0)
					{
						Joint parentJoint = tempSkeleton[tempFrameJoint.parentID]; // Turn the XMFLOAT3 and 4's into vectors for easier computation 
						XMVECTOR parentJointOrientation = XMVectorSet(parentJoint.orientation.x, parentJoint.orientation.y, parentJoint.orientation.z, parentJoint.orientation.w);
						XMVECTOR tempJointPos = XMVectorSet(tempFrameJoint.pos.x, tempFrameJoint.pos.y, tempFrameJoint.pos.z, 0.0f); 
						XMVECTOR parentOrientationConjugate = XMVectorSet(-parentJoint.orientation.x, -parentJoint.orientation.y, -parentJoint.orientation.z, parentJoint.orientation.w);

						
						XMFLOAT3 rotatedPos; 
						XMStoreFloat3(&rotatedPos, XMQuaternionMultiply(XMQuaternionMultiply(parentJointOrientation, tempJointPos), parentOrientationConjugate));
						
						tempFrameJoint.pos.x = rotatedPos.x + parentJoint.pos.x; 
						tempFrameJoint.pos.y = rotatedPos.y + parentJoint.pos.y; 
						tempFrameJoint.pos.z = rotatedPos.z + parentJoint.pos.z;

						
						XMVECTOR tempJointOrient = XMVectorSet(tempFrameJoint.orientation.x, tempFrameJoint.orientation.y, tempFrameJoint.orientation.z, tempFrameJoint.orientation.w); 
						tempJointOrient = XMQuaternionMultiply(parentJointOrientation, tempJointOrient);
						tempJointOrient = XMQuaternionNormalize(tempJointOrient); 
						XMStoreFloat4(&tempFrameJoint.orientation, tempJointOrient);



					} // Store the joint into our temporary frame skeleton 
					tempSkeleton.emplace_back(tempFrameJoint);
				} // Push back our newly created frame skeleton into the animation's frameSkeleton array 
				tempAnim.frameSkeleton.emplace_back(tempSkeleton);
				fileIn >> checkString; // Skip closing bracket "}" 
			}
		} // Calculate and store some usefull animation data 
		tempAnim.frameTime = 1.0f / tempAnim.frameRate; // Set the time per frame 
		tempAnim.totalAnimTime = tempAnim.numFrames * tempAnim.frameTime; // Set the total time the animation takes 
		tempAnim.currAnimTime = 0.0f; // Set the current time to zero 
		av.emplace_back(tempAnim); // Push back the animation into our model object 
	}
	else // If the file was not loaded 
	{
		return false;
	}
	return true;
}


//오브젝트는 다수의 애니메이션을 가질수있음.animation 인자는 몇번째 애니메이션을 쓸건지를 나타낸다.
void UpdateMD5Model(ID3D12GraphicsCommandList* m_DC, CMesh* MD5Model, CGameObject* obj, float deltaTime, int animation, vector<ModelAnimation>& av, UploadBuffer<JointArr>* jarr)
{
	//토탈 타임과 커렌트타임으로 애니메이션을 분할하자.

	obj->currAnimTime += deltaTime; //최근애니메이션시간 갱신 

	if (obj->currAnimTime > av[animation].totalAnimTime)
	{
		obj->currAnimTime = 0.0f; // 애니메이션시간이 끝을벗어나면 0부터시작

								  //애니메이션이 끝나면 애니메이션 후처리 함수를 호출한다. 예를들면 공격이 끝나면 아이들애니메이션
								  //으로 전환하던가 한다.
		obj->EndAnimation(animation);

	}


	float currentFrame = obj->currAnimTime * av[animation].frameRate; //현재 프레임 계산 예를들어 5.36가 나오면 5번프레임과 6번프레임의 중간프레임이다.


	int frame0 = floorf(currentFrame); //5번프레임과 6번프레임일때 5번프레임
	int frame1 = frame0 + 1; // 5번과 6번일때 6번프레임
	if (frame0 == av[animation].numFrames - 1) //마지막프레임 이상이면 
		frame1 = 0; //0번프레임으로
	float interpolation = currentFrame - frame0; //선형보간하자
	std::vector<Joint> interpolatedSkeleton; // 보간된 프레임을 임시적으로 저장할공간
	for (int i = 0; i < av[animation].numJoints; i++)
	{
		Joint tempJoint;
		//모델의애니메이션들중 animation번째의 애니메이션의 frame0번째 키프레임의 i번째 뼈
		//예를들면
		//모델애니메이션중 0번째 애니메이션의 5번 키프래임의 12번째 뼈
		Joint joint0 = av[animation].frameSkeleton[frame0][i];
		Joint joint1 = av[animation].frameSkeleton[frame1][i];

		tempJoint.parentID = joint0.parentID;
		XMVECTOR joint0Orient = XMVectorSet(joint0.orientation.x, joint0.orientation.y, joint0.orientation.z, joint0.orientation.w);
		XMVECTOR joint1Orient = XMVectorSet(joint1.orientation.x, joint1.orientation.y, joint1.orientation.z, joint1.orientation.w);

		XMVECTOR tj0p = XMLoadFloat3(&joint0.pos);
		XMVECTOR tj1p = XMLoadFloat3(&joint1.pos);

		XMStoreFloat3(&tempJoint.pos, tj0p + (interpolation*(tj1p - tj0p)));

	//	tempJoint.pos.x = joint0.pos.x + (interpolation * (joint1.pos.x - joint0.pos.x));
	//	tempJoint.pos.y = joint0.pos.y + (interpolation * (joint1.pos.y - joint0.pos.y));
	//	tempJoint.pos.z = joint0.pos.z + (interpolation * (joint1.pos.z - joint0.pos.z));

		XMStoreFloat4(&tempJoint.orientation, XMQuaternionSlerp(joint0Orient, joint1Orient, interpolation));
		interpolatedSkeleton.emplace_back(tempJoint);

		
	}
	//업데이트 모델에서는 조인트들을 업데이트한다음, 조인트를 상수버퍼로 갱신한다.
	if (jarr != NULL)
	{
		JointArr tempjwa;
		for (int i = 0; i < av[animation].numJoints; i++)
		{
			tempjwa.j[i].parentID = interpolatedSkeleton[i].parentID;
			tempjwa.j[i].orientation = interpolatedSkeleton[i].orientation;
			tempjwa.j[i].pos = interpolatedSkeleton[i].pos;
		}
		auto ts = sizeof(tempjwa);
		jarr->CopyData(0, tempjwa);

	}

	//for (int i = 0; i < MD5Model->nVertex; ++i)
	//{
	//	CVertex tempVert = MD5Model->SubResource[i];
	//	tempVert.V = XMFLOAT3(0, 0, 0);
	//	tempVert.N = XMFLOAT4(0, 0, 0, 0); // Clear vertices normal 
	//	for (int j = 0; j < tempVert.WeightCount; ++j)
	//	{
	//		Weight tempWeight = MD5Model->weights[tempVert.StartWeight + j];

	//		Joint tempJoint = interpolatedSkeleton[tempWeight.jointID];
	//		XMVECTOR tempJointOrientation = XMVectorSet(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);
	//		XMVECTOR tempWeightPos = XMVectorSet(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, 0.0f);

	//		XMVECTOR tempJointOrientationConjugate = XMQuaternionInverse(tempJointOrientation);

	//		XMFLOAT3 rotatedPoint;

	//		XMStoreFloat3(&rotatedPoint, XMQuaternionMultiply(XMQuaternionMultiply(tempJointOrientation, tempWeightPos), tempJointOrientationConjugate));
	//		
	//		XMVECTOR tp = XMLoadFloat3(&tempJoint.pos);
	//		XMVECTOR rp = XMLoadFloat3(&rotatedPoint);
	//		XMVECTOR tpv = XMLoadFloat3(&tempVert.V);
	//		
	//	//	tempVert.V.x += (tempJoint.pos.x + rotatedPoint.x) * tempWeight.bias;
	//	//	tempVert.V.y += (tempJoint.pos.y + rotatedPoint.y) * tempWeight.bias;
	//	//	tempVert.V.z += (tempJoint.pos.z + rotatedPoint.z) * tempWeight.bias; // Compute the normals for this frames skeleton using the weight normals from before // We can comput the normals the same way we compute the vertices position, only we don't have to translate them (just rotate)

	//		XMStoreFloat3(&tempVert.V, tpv+(tp + rp)*tempWeight.bias);

	//																			  //만약 제대로 노멀이 안되면 밑에부분을 없애면 됨
	//																			  /*
	//																			  D3DXQUATERNION tempWeightNormal{ tempWeight.normal.x, tempWeight.normal.y, tempWeight.normal.z, 0.0f }; // Rotate the normal
	//																			  D3DXQuaternionMultiply(&tq,D3DXQuaternionMultiply(&tq,&tempJointOrientation, &tempWeightNormal), &tempJointOrientationConjugate);
	//																			  rotatedPoint = XMFLOAT3{ tq.x,tq.y,tq.z };
	//																			  tempVert.N.x -= rotatedPoint.x * tempWeight.bias;
	//																			  tempVert.N.y -= rotatedPoint.y * tempWeight.bias;
	//																			  tempVert.N.z -= rotatedPoint.z * tempWeight.bias;
	//																			  */
	//	}
	//	MD5Model->SubResource[i].V = XMFLOAT3(tempVert.V.x *scale, tempVert.V.y *scale, tempVert.V.z *scale); // Store the vertices position in the position vector instead of straight into the vertex vector 
	//												   //D3DXVec4Normalize(&tempVert.N, &tempVert.N); // Store the vertices normal 
	//												   //	MD5Model->SubResource[i].N = tempVert.N;
	//
	//}

	//
	////값을 갱신함
	//if (obj->DynamicVertexBuffer != NULL)
	//{
	//	int a = sizeof(CVertex);
	//	obj->DynamicVertexBuffer->CopyData(0, MD5Model->SubResource[0],MD5Model->nVertex);

	//}
}