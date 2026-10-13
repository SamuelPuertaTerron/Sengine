#pragma once

#include "__PROJECT__Globals.h"

namespace __PROJECT__
{
	class GameLayer : public ILayer
	{
	public:
		void OnCreate() override;
		void OnTick(float deltaTime) override;
		void OnDestroy() override;
	};
}//namespace __PROJECT__
