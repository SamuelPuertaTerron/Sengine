#include "Globals.h"
#include "Time.h"

namespace Sengine::Time
{
	namespace
	{
		float m_UnscaledDeltaTime;  // Raw delta time
		float m_DeltaTime; //Scaled delta time used inside the tick functions. 
		float m_CurrentTime;
		float m_PreviousTime;
		float m_TimeScale;
		float m_FixedDeltaTime;
		float m_Accumulator;
	}

	void Init()
	{
		Resume();
		m_PreviousTime = static_cast<float>(Raylib::GetTime());
		m_FixedDeltaTime = 1.0f / 60.0f;
		m_Accumulator = 0.0f;
	}

	void SetTimeScale(float scale)
	{
		m_TimeScale = scale;
	}

	float GetTimeScale()
	{
		return m_TimeScale;
	}

	void Pause()
	{
		SetTimeScale(0.0f);
	}

	void Resume()
	{
		SetTimeScale(1.0f);
	}

	float GetDeltaTime()
	{
		return m_DeltaTime;
	}

	float GetUnscaledDeltaTime()
	{
		return m_UnscaledDeltaTime;
	}

	float GetFixedDeltaTime()
	{
		return m_FixedDeltaTime;
	}

	void CalculateDeltaTime()
	{
		m_CurrentTime = static_cast<float>(Raylib::GetTime());
		m_UnscaledDeltaTime = m_CurrentTime - m_PreviousTime;
		m_DeltaTime = m_UnscaledDeltaTime * m_TimeScale;
		m_PreviousTime = m_CurrentTime;

		m_Accumulator += m_DeltaTime;
	}

	bool OnFixedFrameReady()
	{
		if (m_Accumulator >= m_FixedDeltaTime)
		{
			m_Accumulator -= m_FixedDeltaTime;
			return true;
		}
		return false;
	}
}//namespace Sengine::Time
