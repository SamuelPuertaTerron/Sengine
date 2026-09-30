#include "Globals.h"
#include "Random.h"

namespace Sengine::Random
{
	namespace
	{
		uint64_t MakeOsSeed()
		{
			//random_device returns 32 bits per call; combine two for a full 64-bit seed.
			std::random_device rd;
			return (static_cast<uint64_t>(rd()) << 32) | static_cast<uint64_t>(rd());
		}

		Generator m_Generator{ MakeOsSeed() };
	}//namespace

	void Init()
	{
		m_Generator.Seed(MakeOsSeed());
	}

	void Seed(uint64_t seed)
	{
		m_Generator.Seed(seed);
	}

	uint64_t GetSeed()
	{
		return m_Generator.GetSeed();
	}

	Generator& GetGenerator()
	{
		return m_Generator;
	}

	int Range(int min, int max)
	{
		return m_Generator.Range(min, max);
	}

	float Range(float min, float max)
	{
		return m_Generator.Range(min, max);
	}

	float Value()
	{
		return m_Generator.Value();
	}

	bool Bool()
	{
		return m_Generator.Bool();
	}

	bool Chance(float probability)
	{
		return m_Generator.Chance(probability);
	}

	int Sign()
	{
		return m_Generator.Sign();
	}
}//namespace Sengine::Random