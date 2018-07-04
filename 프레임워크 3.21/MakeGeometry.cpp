#include "MakeGeometry.h"

//-------------------------------------물체 만들기------------------------------------------------
void CreateCube(CMesh * Mesh, float sizex, float sizey, float sizez)
{
	float halfx = 0.5 * sizex;
	float halfy = 0.5 * sizey;
	float halfz = 0.5 * sizez;

	Mesh->SubResource = new CVertex[24];
	Mesh->nVertex = 24;
	Mesh->nStride = sizeof(CVertex);
	Mesh->nOffset = 0;

	//front
	Mesh->SubResource[0].V = XMFLOAT3(-halfx, -halfy, -halfz);
	Mesh->SubResource[1].V = XMFLOAT3(-halfx, +halfy, -halfz);
	Mesh->SubResource[2].V = XMFLOAT3(+halfx, +halfy, -halfz);
	Mesh->SubResource[3].V = XMFLOAT3(+halfx, -halfy, -halfz);

	Mesh->SubResource[0].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[1].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[2].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[3].Tex = XMFLOAT2(1, 1);

	//back
	Mesh->SubResource[4].V = XMFLOAT3(-halfx, -halfy, +halfz);
	Mesh->SubResource[5].V = XMFLOAT3(+halfx, -halfy, +halfz);
	Mesh->SubResource[6].V = XMFLOAT3(+halfx, +halfy, +halfz);
	Mesh->SubResource[7].V = XMFLOAT3(-halfx, +halfy, +halfz);

	Mesh->SubResource[4].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[5].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[6].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[7].Tex = XMFLOAT2(1, 0);

	//top
	Mesh->SubResource[8].V = XMFLOAT3(-halfx, +halfy, -halfz);
	Mesh->SubResource[9].V = XMFLOAT3(-halfx, +halfy, +halfz);
	Mesh->SubResource[10].V = XMFLOAT3(+halfx, +halfy, +halfz);
	Mesh->SubResource[11].V = XMFLOAT3(+halfx, +halfy, -halfz);

	Mesh->SubResource[8].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[9].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[10].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[11].Tex = XMFLOAT2(1, 1);


	//bottom
	Mesh->SubResource[12].V = XMFLOAT3(-halfx, -halfy, -halfz);
	Mesh->SubResource[13].V = XMFLOAT3(+halfx, -halfy, -halfz);
	Mesh->SubResource[14].V = XMFLOAT3(+halfx, -halfy, +halfz);
	Mesh->SubResource[15].V = XMFLOAT3(-halfx, -halfy, +halfz);

	Mesh->SubResource[12].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[13].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[14].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[15].Tex = XMFLOAT2(1, 0);

	//left
	Mesh->SubResource[16].V = XMFLOAT3(-halfx, -halfy, +halfz);
	Mesh->SubResource[17].V = XMFLOAT3(-halfx, +halfy, +halfz);
	Mesh->SubResource[18].V = XMFLOAT3(-halfx, +halfy, -halfz);
	Mesh->SubResource[19].V = XMFLOAT3(-halfx, -halfy, -halfz);

	Mesh->SubResource[16].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[17].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[18].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[19].Tex = XMFLOAT2(1, 1);

	//right
	Mesh->SubResource[20].V = XMFLOAT3(+halfx, -halfy, -halfz);
	Mesh->SubResource[21].V = XMFLOAT3(+halfx, +halfy, -halfz);
	Mesh->SubResource[22].V = XMFLOAT3(+halfx, +halfy, +halfz);
	Mesh->SubResource[23].V = XMFLOAT3(+halfx, -halfy, +halfz);

	Mesh->SubResource[20].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[21].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[22].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[23].Tex = XMFLOAT2(1, 1);

	Mesh->Index = new UINT[36];
	Mesh->nindex = 36;
	Mesh->nioffset = 0;
	Mesh->nisize = sizeof(UINT);

	Mesh->Index[0] = 0; Mesh->Index[1] = 1; Mesh->Index[2] = 2;
	Mesh->Index[3] = 0; Mesh->Index[4] = 2; Mesh->Index[5] = 3;

	Mesh->Index[6] = 4; Mesh->Index[7] = 5; Mesh->Index[8] = 6;
	Mesh->Index[9] = 4; Mesh->Index[10] = 6; Mesh->Index[11] = 7;

	Mesh->Index[12] = 8; Mesh->Index[13] = 9; Mesh->Index[14] = 10;
	Mesh->Index[15] = 8; Mesh->Index[16] = 10; Mesh->Index[17] = 11;

	Mesh->Index[18] = 12; Mesh->Index[19] = 13; Mesh->Index[20] = 14;
	Mesh->Index[21] = 12; Mesh->Index[22] = 14; Mesh->Index[23] = 15;

	Mesh->Index[24] = 16; Mesh->Index[25] = 17; Mesh->Index[26] = 18;
	Mesh->Index[27] = 16; Mesh->Index[28] = 18; Mesh->Index[29] = 19;

	Mesh->Index[30] = 20; Mesh->Index[31] = 21; Mesh->Index[32] = 22;
	Mesh->Index[33] = 20; Mesh->Index[34] = 22; Mesh->Index[35] = 23;


}

void CreatePentagon(CMesh* Mesh, float size, float ypos)
{
	float half = 0.5 * size;
	float y = ypos;

	float s1 = half * sinf(0.4f * MMPE_PI);
	float s2 = half * sinf(0.8f * MMPE_PI);

	float c1 = half * cosf(0.4f * MMPE_PI);
	float c2 = half * cosf(0.2f * MMPE_PI);

	Mesh->SubResource = new CVertex[15];
	Mesh->nVertex = 15;
	Mesh->nStride = sizeof(CVertex);
	Mesh->nOffset = 0;


	Mesh->SubResource[0].V = XMFLOAT3(0, y, half);
	Mesh->SubResource[1].V = XMFLOAT3(0, y, 0);
	Mesh->SubResource[2].V = XMFLOAT3(-s1, y, c1);

	Mesh->SubResource[0].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[1].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[2].Tex = XMFLOAT2(1, 0);

	//
	Mesh->SubResource[3].V = XMFLOAT3(0, y, 0);
	Mesh->SubResource[4].V = XMFLOAT3(0, y, half);
	Mesh->SubResource[5].V = XMFLOAT3(s1, y, c1);

	Mesh->SubResource[3].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[4].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[5].Tex = XMFLOAT2(1, 0);

	//
	Mesh->SubResource[6].V = XMFLOAT3(s2, y, -c2);
	Mesh->SubResource[7].V = XMFLOAT3(0, y, 0);
	Mesh->SubResource[8].V = XMFLOAT3(s1, y, c1);

	Mesh->SubResource[6].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[7].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[8].Tex = XMFLOAT2(1, 1);

	//
	Mesh->SubResource[9] = XMFLOAT3(-s2, y, -c2);
	Mesh->SubResource[10] = XMFLOAT3(0, y, 0);
	Mesh->SubResource[11] = XMFLOAT3(s2, y, -c2);

	Mesh->SubResource[9].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[10].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[11].Tex = XMFLOAT2(1, 0);

	//
	Mesh->SubResource[12] = XMFLOAT3(-s1, y, c1);
	Mesh->SubResource[13] = XMFLOAT3(0, y, 0);
	Mesh->SubResource[14] = XMFLOAT3(-s2, y, -c2);

	Mesh->SubResource[12].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[13].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[14].Tex = XMFLOAT2(1, 0);



	Mesh->Index = new UINT[15];
	Mesh->nindex = 15;
	Mesh->nioffset = 0;
	Mesh->nisize = sizeof(UINT);

	Mesh->Index[0] = 0; Mesh->Index[1] = 1; Mesh->Index[2] = 2;
	Mesh->Index[3] = 3; Mesh->Index[4] = 4; Mesh->Index[5] = 5;
	Mesh->Index[6] = 6; Mesh->Index[7] = 7; Mesh->Index[8] = 8;
	Mesh->Index[9] = 9; Mesh->Index[10] = 10; Mesh->Index[11] = 11;
	Mesh->Index[12] = 12; Mesh->Index[13] = 13; Mesh->Index[14] = 14;




}

void CreateTetrisL(CMesh * Mesh, float sizex, float sizey, float sizez)
{
	float halfx = 0.5 * sizex;
	float halfy = 0.5 * sizey;
	float halfz = 0.5 * sizez;

	Mesh->SubResource = new CVertex[48];
	Mesh->nVertex = 48;
	Mesh->nStride = sizeof(CVertex);
	Mesh->nOffset = 0;

	//front
	Mesh->SubResource[0].V = XMFLOAT3(-halfx, -halfy, -halfz);
	Mesh->SubResource[1].V = XMFLOAT3(-halfx, +halfy, -halfz);
	Mesh->SubResource[2].V = XMFLOAT3(+halfx, +halfy, -halfz);
	Mesh->SubResource[3].V = XMFLOAT3(+halfx, -halfy, -halfz);

	Mesh->SubResource[0].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[1].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[2].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[3].Tex = XMFLOAT2(1, 1);

	//back
	Mesh->SubResource[4].V = XMFLOAT3(-halfx, -halfy, +halfz);
	Mesh->SubResource[5].V = XMFLOAT3(+halfx, -halfy, +halfz);
	Mesh->SubResource[6].V = XMFLOAT3(+halfx, +halfy, +halfz);
	Mesh->SubResource[7].V = XMFLOAT3(-halfx, +halfy, +halfz);

	Mesh->SubResource[4].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[5].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[6].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[7].Tex = XMFLOAT2(1, 0);

	//top
	Mesh->SubResource[8].V = XMFLOAT3(-halfx, +halfy, -halfz);
	Mesh->SubResource[9].V = XMFLOAT3(-halfx, +halfy, +halfz);
	Mesh->SubResource[10].V = XMFLOAT3(+halfx, +halfy, +halfz);
	Mesh->SubResource[11].V = XMFLOAT3(+halfx, +halfy, -halfz);

	Mesh->SubResource[8].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[9].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[10].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[11].Tex = XMFLOAT2(1, 1);


	//bottom
	Mesh->SubResource[12].V = XMFLOAT3(-halfx, -halfy, -halfz);
	Mesh->SubResource[13].V = XMFLOAT3(+halfx, -halfy, -halfz);
	Mesh->SubResource[14].V = XMFLOAT3(+halfx, -halfy, +halfz);
	Mesh->SubResource[15].V = XMFLOAT3(-halfx, -halfy, +halfz);

	Mesh->SubResource[12].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[13].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[14].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[15].Tex = XMFLOAT2(1, 0);

	//left
	Mesh->SubResource[16].V = XMFLOAT3(-halfx, -halfy, +halfz);
	Mesh->SubResource[17].V = XMFLOAT3(-halfx, +halfy, +halfz);
	Mesh->SubResource[18].V = XMFLOAT3(-halfx, +halfy, -halfz);
	Mesh->SubResource[19].V = XMFLOAT3(-halfx, -halfy, -halfz);

	Mesh->SubResource[16].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[17].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[18].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[19].Tex = XMFLOAT2(1, 1);

	//right
	Mesh->SubResource[20].V = XMFLOAT3(+halfx, -halfy, -halfz);
	Mesh->SubResource[21].V = XMFLOAT3(+halfx, +halfy, -halfz);
	Mesh->SubResource[22].V = XMFLOAT3(+halfx, +halfy, +halfz);
	Mesh->SubResource[23].V = XMFLOAT3(+halfx, -halfy, +halfz);

	Mesh->SubResource[20].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[21].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[22].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[23].Tex = XMFLOAT2(1, 1);


	//front
	Mesh->SubResource[24 + 0].V = XMFLOAT3(-halfx + halfx * 2, -halfx + halfx * 2, -halfz);
	Mesh->SubResource[24 + 1].V = XMFLOAT3(-halfx + halfx * 2, +halfx + halfx * 2, -halfz);
	Mesh->SubResource[24 + 2].V = XMFLOAT3(+halfx + halfx * 2, +halfx + halfx * 2, -halfz);
	Mesh->SubResource[24 + 3].V = XMFLOAT3(+halfx + halfx * 2, -halfx + halfx * 2, -halfz);

	Mesh->SubResource[24 + 0].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[24 + 1].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[24 + 2].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[24 + 3].Tex = XMFLOAT2(1, 1);

	//back			  24+
	Mesh->SubResource[24 + 4].V = XMFLOAT3(-halfx + halfx * 2, -halfx + halfx * 2, +halfz);
	Mesh->SubResource[24 + 5].V = XMFLOAT3(+halfx + halfx * 2, -halfx + halfx * 2, +halfz);
	Mesh->SubResource[24 + 6].V = XMFLOAT3(+halfx + halfx * 2, +halfx + halfx * 2, +halfz);
	Mesh->SubResource[24 + 7].V = XMFLOAT3(-halfx + halfx * 2, +halfx + halfx * 2, +halfz);

	Mesh->SubResource[24 + 4].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[24 + 5].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[24 + 6].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[24 + 7].Tex = XMFLOAT2(1, 0);

	//top			  24+
	Mesh->SubResource[24 + 8].V = XMFLOAT3(-halfx + halfx * 2, +halfx + halfx * 2, -halfz);
	Mesh->SubResource[24 + 9].V = XMFLOAT3(-halfx + halfx * 2, +halfx + halfx * 2, +halfz);
	Mesh->SubResource[24 + 10].V = XMFLOAT3(+halfx + halfx * 2, +halfx + halfx * 2, +halfz);
	Mesh->SubResource[24 + 11].V = XMFLOAT3(+halfx + halfx * 2, +halfx + halfx * 2, -halfz);

	Mesh->SubResource[24 + 8].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[24 + 9].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[24 + 10].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[24 + 11].Tex = XMFLOAT2(1, 1);
	//bottom		  24+
	Mesh->SubResource[24 + 12].V = XMFLOAT3(-halfx + halfx * 2, -halfx + halfx * 2, -halfz);
	Mesh->SubResource[24 + 13].V = XMFLOAT3(+halfx + halfx * 2, -halfx + halfx * 2, -halfz);
	Mesh->SubResource[24 + 14].V = XMFLOAT3(+halfx + halfx * 2, -halfx + halfx * 2, +halfz);
	Mesh->SubResource[24 + 15].V = XMFLOAT3(-halfx + halfx * 2, -halfx + halfx * 2, +halfz);


	Mesh->SubResource[24 + 12].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[24 + 13].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[24 + 14].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[24 + 15].Tex = XMFLOAT2(1, 0);

	//left			  24+
	Mesh->SubResource[24 + 16].V = XMFLOAT3(-halfx + halfx * 2, -halfx + halfx * 2, +halfz);
	Mesh->SubResource[24 + 17].V = XMFLOAT3(-halfx + halfx * 2, +halfx + halfx * 2, +halfz);
	Mesh->SubResource[24 + 18].V = XMFLOAT3(-halfx + halfx * 2, +halfx + halfx * 2, -halfz);
	Mesh->SubResource[24 + 19].V = XMFLOAT3(-halfx + halfx * 2, -halfx + halfx * 2, -halfz);

	Mesh->SubResource[24 + 16].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[24 + 17].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[24 + 18].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[24 + 19].Tex = XMFLOAT2(1, 1);

	//right			  24+
	Mesh->SubResource[24 + 20].V = XMFLOAT3(+halfx + halfx * 2, -halfx + halfx * 2, -halfz);
	Mesh->SubResource[24 + 21].V = XMFLOAT3(+halfx + halfx * 2, +halfx + halfx * 2, -halfz);
	Mesh->SubResource[24 + 22].V = XMFLOAT3(+halfx + halfx * 2, +halfx + halfx * 2, +halfz);
	Mesh->SubResource[24 + 23].V = XMFLOAT3(+halfx + halfx * 2, -halfx + halfx * 2, +halfz);

	Mesh->SubResource[24 + 20].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[24 + 21].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[24 + 22].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[24 + 23].Tex = XMFLOAT2(1, 1);



	Mesh->Index = new UINT[72];
	Mesh->nindex = 72;
	Mesh->nioffset = 0;
	Mesh->nisize = sizeof(UINT);

	Mesh->Index[0] = 0; Mesh->Index[1] = 1; Mesh->Index[2] = 2;
	Mesh->Index[3] = 0; Mesh->Index[4] = 2; Mesh->Index[5] = 3;

	Mesh->Index[6] = 4; Mesh->Index[7] = 5; Mesh->Index[8] = 6;
	Mesh->Index[9] = 4; Mesh->Index[10] = 6; Mesh->Index[11] = 7;

	Mesh->Index[12] = 8; Mesh->Index[13] = 9; Mesh->Index[14] = 10;
	Mesh->Index[15] = 8; Mesh->Index[16] = 10; Mesh->Index[17] = 11;

	Mesh->Index[18] = 12; Mesh->Index[19] = 13; Mesh->Index[20] = 14;
	Mesh->Index[21] = 12; Mesh->Index[22] = 14; Mesh->Index[23] = 15;

	Mesh->Index[24] = 16; Mesh->Index[25] = 17; Mesh->Index[26] = 18;
	Mesh->Index[27] = 16; Mesh->Index[28] = 18; Mesh->Index[29] = 19;

	Mesh->Index[30] = 20; Mesh->Index[31] = 21; Mesh->Index[32] = 22;
	Mesh->Index[33] = 20; Mesh->Index[34] = 22; Mesh->Index[35] = 23;


	Mesh->Index[36 + 0] = 24 + 0; Mesh->Index[36 + 1] = 24 + 1; Mesh->Index[36 + 2] = 24 + 2;
	Mesh->Index[36 + 3] = 24 + 0; Mesh->Index[36 + 4] = 24 + 2; Mesh->Index[36 + 5] = 24 + 3;
	Mesh->Index[36 + 6] = 24 + 4; Mesh->Index[36 + 7] = 24 + 5; Mesh->Index[36 + 8] = 24 + 6;
	Mesh->Index[36 + 9] = 24 + 4; Mesh->Index[36 + 10] = 24 + 6; Mesh->Index[36 + 11] = 24 + 7;
	Mesh->Index[36 + 12] = 24 + 8; Mesh->Index[36 + 13] = 24 + 9; Mesh->Index[36 + 14] = 24 + 10;
	Mesh->Index[36 + 15] = 24 + 8; Mesh->Index[36 + 16] = 24 + 10; Mesh->Index[36 + 17] = 24 + 11;
	Mesh->Index[36 + 18] = 24 + 12; Mesh->Index[36 + 19] = 24 + 13; Mesh->Index[36 + 20] = 24 + 14;
	Mesh->Index[36 + 21] = 24 + 12; Mesh->Index[36 + 22] = 24 + 14; Mesh->Index[36 + 23] = 24 + 15;
	Mesh->Index[36 + 24] = 24 + 16; Mesh->Index[36 + 25] = 24 + 17; Mesh->Index[36 + 26] = 24 + 18;
	Mesh->Index[36 + 27] = 24 + 16; Mesh->Index[36 + 28] = 24 + 18; Mesh->Index[36 + 29] = 24 + 19;
	Mesh->Index[36 + 30] = 24 + 20; Mesh->Index[36 + 31] = 24 + 21; Mesh->Index[36 + 32] = 24 + 22;
	Mesh->Index[36 + 33] = 24 + 20; Mesh->Index[36 + 34] = 24 + 22; Mesh->Index[36 + 35] = 24 + 23;

}

void CreateTetrisㅗ(CMesh * Mesh, float sizex, float sizey, float sizez)
{
	float halfx = 0.5 * sizex;
	float halfy = 0.5 * sizey;
	float halfz = 0.5 * sizez;

	Mesh->SubResource = new CVertex[48];
	Mesh->nVertex = 48;
	Mesh->nStride = sizeof(CVertex);
	Mesh->nOffset = 0;

	//front
	Mesh->SubResource[0].V = XMFLOAT3(-halfx, -halfy, -halfz);
	Mesh->SubResource[1].V = XMFLOAT3(-halfx, +halfy, -halfz);
	Mesh->SubResource[2].V = XMFLOAT3(+halfx, +halfy, -halfz);
	Mesh->SubResource[3].V = XMFLOAT3(+halfx, -halfy, -halfz);

	Mesh->SubResource[0].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[1].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[2].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[3].Tex = XMFLOAT2(1, 1);

	//back
	Mesh->SubResource[4].V = XMFLOAT3(-halfx, -halfy, +halfz);
	Mesh->SubResource[5].V = XMFLOAT3(+halfx, -halfy, +halfz);
	Mesh->SubResource[6].V = XMFLOAT3(+halfx, +halfy, +halfz);
	Mesh->SubResource[7].V = XMFLOAT3(-halfx, +halfy, +halfz);

	Mesh->SubResource[4].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[5].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[6].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[7].Tex = XMFLOAT2(1, 0);

	//top
	Mesh->SubResource[8].V = XMFLOAT3(-halfx, +halfy, -halfz);
	Mesh->SubResource[9].V = XMFLOAT3(-halfx, +halfy, +halfz);
	Mesh->SubResource[10].V = XMFLOAT3(+halfx, +halfy, +halfz);
	Mesh->SubResource[11].V = XMFLOAT3(+halfx, +halfy, -halfz);

	Mesh->SubResource[8].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[9].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[10].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[11].Tex = XMFLOAT2(1, 1);


	//bottom
	Mesh->SubResource[12].V = XMFLOAT3(-halfx, -halfy, -halfz);
	Mesh->SubResource[13].V = XMFLOAT3(+halfx, -halfy, -halfz);
	Mesh->SubResource[14].V = XMFLOAT3(+halfx, -halfy, +halfz);
	Mesh->SubResource[15].V = XMFLOAT3(-halfx, -halfy, +halfz);

	Mesh->SubResource[12].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[13].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[14].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[15].Tex = XMFLOAT2(1, 0);

	//left
	Mesh->SubResource[16].V = XMFLOAT3(-halfx, -halfy, +halfz);
	Mesh->SubResource[17].V = XMFLOAT3(-halfx, +halfy, +halfz);
	Mesh->SubResource[18].V = XMFLOAT3(-halfx, +halfy, -halfz);
	Mesh->SubResource[19].V = XMFLOAT3(-halfx, -halfy, -halfz);

	Mesh->SubResource[16].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[17].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[18].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[19].Tex = XMFLOAT2(1, 1);

	//right
	Mesh->SubResource[20].V = XMFLOAT3(+halfx, -halfy, -halfz);
	Mesh->SubResource[21].V = XMFLOAT3(+halfx, +halfy, -halfz);
	Mesh->SubResource[22].V = XMFLOAT3(+halfx, +halfy, +halfz);
	Mesh->SubResource[23].V = XMFLOAT3(+halfx, -halfy, +halfz);

	Mesh->SubResource[20].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[21].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[22].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[23].Tex = XMFLOAT2(1, 1);


	//front
	Mesh->SubResource[24 + 0].V = XMFLOAT3(-halfx + halfx * 2, -halfx, -halfz);
	Mesh->SubResource[24 + 1].V = XMFLOAT3(-halfx + halfx * 2, +halfx, -halfz);
	Mesh->SubResource[24 + 2].V = XMFLOAT3(+halfx + halfx * 2, +halfx, -halfz);
	Mesh->SubResource[24 + 3].V = XMFLOAT3(+halfx + halfx * 2, -halfx, -halfz);

	Mesh->SubResource[24 + 0].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[24 + 1].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[24 + 2].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[24 + 3].Tex = XMFLOAT2(1, 1);

	//back			  24+
	Mesh->SubResource[24 + 4].V = XMFLOAT3(-halfx + halfx * 2, -halfx, +halfz);
	Mesh->SubResource[24 + 5].V = XMFLOAT3(+halfx + halfx * 2, -halfx, +halfz);
	Mesh->SubResource[24 + 6].V = XMFLOAT3(+halfx + halfx * 2, +halfx, +halfz);
	Mesh->SubResource[24 + 7].V = XMFLOAT3(-halfx + halfx * 2, +halfx, +halfz);

	Mesh->SubResource[24 + 4].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[24 + 5].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[24 + 6].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[24 + 7].Tex = XMFLOAT2(1, 0);

	//top			  24+
	Mesh->SubResource[24 + 8].V = XMFLOAT3(-halfx + halfx * 2, +halfx, -halfz);
	Mesh->SubResource[24 + 9].V = XMFLOAT3(-halfx + halfx * 2, +halfx, +halfz);
	Mesh->SubResource[24 + 10].V = XMFLOAT3(+halfx + halfx * 2, +halfx, +halfz);
	Mesh->SubResource[24 + 11].V = XMFLOAT3(+halfx + halfx * 2, +halfx, -halfz);

	Mesh->SubResource[24 + 8].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[24 + 9].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[24 + 10].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[24 + 11].Tex = XMFLOAT2(1, 1);
	//bottom		  24+
	Mesh->SubResource[24 + 12].V = XMFLOAT3(-halfx + halfx * 2, -halfx, -halfz);
	Mesh->SubResource[24 + 13].V = XMFLOAT3(+halfx + halfx * 2, -halfx, -halfz);
	Mesh->SubResource[24 + 14].V = XMFLOAT3(+halfx + halfx * 2, -halfx, +halfz);
	Mesh->SubResource[24 + 15].V = XMFLOAT3(-halfx + halfx * 2, -halfx, +halfz);


	Mesh->SubResource[24 + 12].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[24 + 13].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[24 + 14].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[24 + 15].Tex = XMFLOAT2(1, 0);

	//left			  24+
	Mesh->SubResource[24 + 16].V = XMFLOAT3(-halfx + halfx * 2, -halfx, +halfz);
	Mesh->SubResource[24 + 17].V = XMFLOAT3(-halfx + halfx * 2, +halfx, +halfz);
	Mesh->SubResource[24 + 18].V = XMFLOAT3(-halfx + halfx * 2, +halfx, -halfz);
	Mesh->SubResource[24 + 19].V = XMFLOAT3(-halfx + halfx * 2, -halfx, -halfz);

	Mesh->SubResource[24 + 16].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[24 + 17].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[24 + 18].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[24 + 19].Tex = XMFLOAT2(1, 1);

	//right			  24+
	Mesh->SubResource[24 + 20].V = XMFLOAT3(+halfx + halfx * 2, -halfx, -halfz);
	Mesh->SubResource[24 + 21].V = XMFLOAT3(+halfx + halfx * 2, +halfx, -halfz);
	Mesh->SubResource[24 + 22].V = XMFLOAT3(+halfx + halfx * 2, +halfx, +halfz);
	Mesh->SubResource[24 + 23].V = XMFLOAT3(+halfx + halfx * 2, -halfx, +halfz);

	Mesh->SubResource[24 + 20].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[24 + 21].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[24 + 22].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[24 + 23].Tex = XMFLOAT2(1, 1);



	Mesh->Index = new UINT[72];
	Mesh->nindex = 72;
	Mesh->nioffset = 0;
	Mesh->nisize = sizeof(UINT);

	Mesh->Index[0] = 0; Mesh->Index[1] = 1; Mesh->Index[2] = 2;
	Mesh->Index[3] = 0; Mesh->Index[4] = 2; Mesh->Index[5] = 3;

	Mesh->Index[6] = 4; Mesh->Index[7] = 5; Mesh->Index[8] = 6;
	Mesh->Index[9] = 4; Mesh->Index[10] = 6; Mesh->Index[11] = 7;

	Mesh->Index[12] = 8; Mesh->Index[13] = 9; Mesh->Index[14] = 10;
	Mesh->Index[15] = 8; Mesh->Index[16] = 10; Mesh->Index[17] = 11;

	Mesh->Index[18] = 12; Mesh->Index[19] = 13; Mesh->Index[20] = 14;
	Mesh->Index[21] = 12; Mesh->Index[22] = 14; Mesh->Index[23] = 15;

	Mesh->Index[24] = 16; Mesh->Index[25] = 17; Mesh->Index[26] = 18;
	Mesh->Index[27] = 16; Mesh->Index[28] = 18; Mesh->Index[29] = 19;

	Mesh->Index[30] = 20; Mesh->Index[31] = 21; Mesh->Index[32] = 22;
	Mesh->Index[33] = 20; Mesh->Index[34] = 22; Mesh->Index[35] = 23;


	Mesh->Index[36 + 0] = 24 + 0; Mesh->Index[36 + 1] = 24 + 1; Mesh->Index[36 + 2] = 24 + 2;
	Mesh->Index[36 + 3] = 24 + 0; Mesh->Index[36 + 4] = 24 + 2; Mesh->Index[36 + 5] = 24 + 3;
	Mesh->Index[36 + 6] = 24 + 4; Mesh->Index[36 + 7] = 24 + 5; Mesh->Index[36 + 8] = 24 + 6;
	Mesh->Index[36 + 9] = 24 + 4; Mesh->Index[36 + 10] = 24 + 6; Mesh->Index[36 + 11] = 24 + 7;
	Mesh->Index[36 + 12] = 24 + 8; Mesh->Index[36 + 13] = 24 + 9; Mesh->Index[36 + 14] = 24 + 10;
	Mesh->Index[36 + 15] = 24 + 8; Mesh->Index[36 + 16] = 24 + 10; Mesh->Index[36 + 17] = 24 + 11;
	Mesh->Index[36 + 18] = 24 + 12; Mesh->Index[36 + 19] = 24 + 13; Mesh->Index[36 + 20] = 24 + 14;
	Mesh->Index[36 + 21] = 24 + 12; Mesh->Index[36 + 22] = 24 + 14; Mesh->Index[36 + 23] = 24 + 15;
	Mesh->Index[36 + 24] = 24 + 16; Mesh->Index[36 + 25] = 24 + 17; Mesh->Index[36 + 26] = 24 + 18;
	Mesh->Index[36 + 27] = 24 + 16; Mesh->Index[36 + 28] = 24 + 18; Mesh->Index[36 + 29] = 24 + 19;
	Mesh->Index[36 + 30] = 24 + 20; Mesh->Index[36 + 31] = 24 + 21; Mesh->Index[36 + 32] = 24 + 22;
	Mesh->Index[36 + 33] = 24 + 20; Mesh->Index[36 + 34] = 24 + 22; Mesh->Index[36 + 35] = 24 + 23;

}
