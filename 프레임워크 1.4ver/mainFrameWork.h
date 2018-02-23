#include "FrameWork.h"
#include <DirectXColors.h>

using namespace DirectX;

class MainFrameWork : public FrameWork
{
public:
	MainFrameWork(HINSTANCE hInstance);
	~MainFrameWork();

	virtual bool Initialize()override;
private:
	virtual void OnResize()override;
	virtual void FrameAdvance(const GameTimer& gt)override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

	void KeyboardInput(const GameTimer& gt);
};