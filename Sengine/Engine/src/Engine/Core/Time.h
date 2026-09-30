#pragma once

namespace Sengine::Time
{
	void Init();

	void SetTimeScale(float scale);
	void Pause();
	void Resume();

	float GetTimeScale();
	float GetDeltaTime();
	float GetUnscaledDeltaTime();
	float GetFixedDeltaTime();

	//Note: Internal Function used inside the Engine class
	void CalculateDeltaTime();

	bool OnFixedFrameReady();

}//namespace Sengine::Time