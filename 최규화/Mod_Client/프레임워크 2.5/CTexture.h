#pragma once
#pragma once
#include "CMesh.h"

struct CTexture
{

	string Name;

	wstring Filename;

	ComPtr<ID3D12Resource> Resource = nullptr;
	ComPtr<ID3D12Resource> UploadHeap = nullptr;

};