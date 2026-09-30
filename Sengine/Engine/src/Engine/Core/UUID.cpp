#include "Globals.h"
#include "UUID.h"

namespace Sengine
{
	uint64_t GetRandomInt64Value()
	{
		thread_local std::mt19937_64 engine{ std::random_device{}() };
		thread_local std::uniform_int_distribution<uint64_t> distribution;

		uint64_t value = 0;
		while (value == 0)
		{
			value = distribution(engine);
		}
		return value;
	}

	uint64_t UUID::GetValue()
	{
		return m_Value;
	}

	bool UUID::IsValid()
	{
		return m_Value != 0;
	}
}//namespace Sengine::Random

