#pragma once
#include"CMesh.h"
#include"MMPE\MyMiniPysicsEngine.h"
//======================= 도형만들기====================================
void CreateCube(CMesh* Mesh, float sizex, float sizey, float sizez);
void CreatePentagon(CMesh* Mesh, float size, float sizey = 0);
void CreateTetrisL(CMesh* Mesh, float sizex, float sizey, float sizez);
void CreateTetrisㅗ(CMesh* Mesh, float sizex, float sizey, float sizez);