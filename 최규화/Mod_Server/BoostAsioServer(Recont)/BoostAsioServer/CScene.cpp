#include "CScene.h"



CScene::CScene()
{
}


void CScene::CreateGameObject()
{
	for (int i = 0; i < MAX_BOX_NUM; ++i)
		m_StaticObjectUset.insert(new NormalBoxObject(i));
	
	auto max_fsize = MAX_BOX_NUM + MAX_SMALL_WALL_NUM;
	for (int i = MAX_BOX_NUM; i < max_fsize; ++i)
		m_StaticObjectUset.insert(new SmallWallObject(i));

	auto max_ssize = max_fsize + MAX_BIG_WALL_NUM;
	for (int i = max_fsize; i < max_ssize; ++i)
		m_StaticObjectUset.insert(new BigWallObject(i));

	auto max_tsize = max_ssize + MAX_BULLDING_NUM;
	for (int i = max_ssize; i < max_tsize; ++i)
		m_StaticObjectUset.insert(new Building(i));

	for (auto sobj : m_StaticObjectUset)
	{
		sobj->AfterGravitySystem(0, OBJECT_TYPE::FIXED_OBJECT);
	}
}

CScene::~CScene()
{

}
