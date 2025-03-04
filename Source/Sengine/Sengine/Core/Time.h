/**
 *	Filename: Time.h
 *	Date Created: 27/02/25
 *	Purpose: A cross platform time class for ease of access. 
 ***/
#pragma once

namespace Sengine
{
	using HighResClock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::high_resolution_clock::time_point;
	using TimeDifference = std::chrono::duration<float>;

	//TODO: Make this class Cross Platform
	class Time
	{
	public:
		static std::string GetCurrentTimeAsString();
		static std::string GetCurrentDateAsString();

		static TimePoint GetTime();
	};
}


