#include "FrameWork.h"
#include <DirectXColors.h>

using namespace DirectX;

class MainFrameWork : public FrameWork
{
public:
	MainFrameWork(HINSTANCE hInstance);
	~MainFrameWork();

	virtual bool Initialize()override;
	virtual void CollisionSystem(const GameTimer& gt);
	virtual void System(const GameTimer& gt);
	virtual void GravitySystem(const GameTimer& gt);
	//중력시스템의 후처리를 해준다. 땅에파고든걸 위로 밀어버림.
	//이녀석은 적분기 이후에 위치가 변경된후 처리해야한다.
	//또한 위치가 변경되므로 반드시 UpdatePPosCenterPos를 할것.
	virtual void AfterGravitySystem(const GameTimer& gt);
private:
	virtual void OnResize()override;
	virtual void FrameAdvance(const GameTimer& gt)override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

};