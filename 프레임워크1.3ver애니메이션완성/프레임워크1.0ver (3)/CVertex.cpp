#include "CVertex.h"


CVertex::CVertex(void)
{
	XMVECTOR t = XMVectorZero();
	XMStoreFloat3(&V, t);
	XMStoreFloat4(&N, t);
}


CVertex::~CVertex(void)
{
}