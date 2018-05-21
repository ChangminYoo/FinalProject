#pragma once
#include "CStaticObject.h"

class CScene
{
private:
	
public:
	CScene();

	unordered_set<CMonoObject*> m_StaticObjectUset;
	void CreateGameObject(); 
	~CScene();
};

