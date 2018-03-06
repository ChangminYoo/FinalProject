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
	//�߷½ý����� ��ó���� ���ش�. �����İ��� ���� �о����.
	//�̳༮�� ���б� ���Ŀ� ��ġ�� ������� ó���ؾ��Ѵ�.
	//���� ��ġ�� ����ǹǷ� �ݵ�� UpdatePPosCenterPos�� �Ұ�.
	virtual void AfterGravitySystem(const GameTimer& gt);
private:
	virtual void OnResize()override;
	virtual void FrameAdvance(const GameTimer& gt)override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

};