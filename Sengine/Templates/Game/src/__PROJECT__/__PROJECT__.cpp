#include "__PROJECT__Globals.h"
#include "__PROJECT__/__PROJECT__.h"

namespace __PROJECT__
{
	void GameLayer::OnCreate()
	{
		
	}

	void GameLayer::OnTick(float deltaTime)
	{
		Renderer2D::BeginFrame(Raylib::BLUE);
		Renderer2D::EndFrame();
	}

	void GameLayer::OnDestroy()
	{
		
	}
}//namespace __PROJECT__