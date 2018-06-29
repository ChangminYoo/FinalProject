#include "CVertex.h"


CVertex::CVertex(void)
{
	XMVECTOR t = XMVectorZero();
	XMStoreFloat3(&V, t);
	XMStoreFloat4(&N, t);
	XMStoreFloat3(&Tangent, t);

}


CVertex::~CVertex(void)
{
}

