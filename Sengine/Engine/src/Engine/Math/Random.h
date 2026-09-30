#pragma once

#include <bit>
#include <cassert>
#include <cstdint>
#include <limits>

namespace Sengine::Random
{
	class Generator
	{
	public:
		using result_type = uint64_t;

		Generator() : Generator(0x853C49E6748FEA9Bull) {}
		explicit Generator(uint64_t seed) { Seed(seed); }

		void Seed(uint64_t seed)
		{
			m_Seed = seed;

			//SplitMix64 spreads one 64-bit seed across the 256-bit state.
			//It never produces an all-zero state, which xoshiro cannot recover from.
			uint64_t x = seed;
			for (uint64_t& word : m_State)
			{
				word = SplitMix64(x);
			}
		}

		[[nodiscard]] uint64_t GetSeed() const { return m_Seed; }

		//std::uniform_random_bit_generator interface. Parenthesised names guard against Windows min/max macros.
		static constexpr result_type(min)() { return 0; }
		static constexpr result_type(max)() { return (std::numeric_limits<result_type>::max)(); }
		result_type operator()() { return Next(); }

		uint64_t Next()
		{
			const uint64_t result = std::rotl(m_State[1] * 5, 7) * 9;
			const uint64_t t = m_State[1] << 17;

			m_State[2] ^= m_State[0];
			m_State[3] ^= m_State[1];
			m_State[1] ^= m_State[2];
			m_State[0] ^= m_State[3];
			m_State[2] ^= t;
			m_State[3] = std::rotl(m_State[3], 45);

			return result;
		}

		//Upper bits are the highest quality, so take those.
		uint32_t Next32() { return static_cast<uint32_t>(Next() >> 32); }

		//Inclusive on both ends: Range(1, 6) can return 1..6.
		int Range(int min, int max)
		{
			assert(min <= max && "Random::Range called with min > max");

			const uint32_t span = static_cast<uint32_t>(max) - static_cast<uint32_t>(min);
			if (span == (std::numeric_limits<uint32_t>::max)())
			{
				return static_cast<int>(Next32()); //Full int range.
			}
			return static_cast<int>(static_cast<uint32_t>(min) + Bounded(span + 1));
		}

		//[min, max]. Hitting max exactly is possible only through float rounding.
		float Range(float min, float max)
		{
			assert(min <= max && "Random::Range called with min > max");
			return min + (max - min) * Value();
		}

		//[0, 1). Uses the top 24 bits, which is all the precision a float's mantissa can hold.
		float Value()
		{
			return static_cast<float>(Next() >> 40) * 0x1.0p-24f;
		}

		bool Bool() { return (Next() >> 63) != 0; }

		//True with the given probability, e.g. Chance(0.25f) is true 25% of the time.
		bool Chance(float probability) { return Value() < probability; }

		int Sign() { return Bool() ? 1 : -1; }

	private:
		//Unbiased integer in [0, bound) using Lemire's multiply-shift method (rarely needs a division).
		uint32_t Bounded(uint32_t bound)
		{
			uint64_t product = static_cast<uint64_t>(Next32()) * bound;
			uint32_t low = static_cast<uint32_t>(product);

			if (low < bound)
			{
				const uint32_t threshold = (0u - bound) % bound;
				while (low < threshold)
				{
					product = static_cast<uint64_t>(Next32()) * bound;
					low = static_cast<uint32_t>(product);
				}
			}
			return static_cast<uint32_t>(product >> 32);
		}

		static uint64_t SplitMix64(uint64_t& x)
		{
			uint64_t z = (x += 0x9E3779B97F4A7C15ull);
			z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
			z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
			return z ^ (z >> 31);
		}

	private:
		uint64_t m_State[4]{};
		uint64_t m_Seed = 0;
	};

	//----- Global generator: usable from anywhere, same API as before -----

	void Init();								//Reseeds from the OS (std::random_device). Optional; it is already randomly seeded at startup.
	void Seed(uint64_t seed);					//Deterministic seed, e.g. for a daily run or a replay.
	[[nodiscard]] uint64_t GetSeed();			//Store this to reproduce a run.
	[[nodiscard]] Generator& GetGenerator();	//For std::shuffle(v.begin(), v.end(), Random::GetGenerator()).

	int Range(int min, int max);
	float Range(float min, float max);
	float Value();
	bool Bool();
	bool Chance(float probability);
	int Sign();
}//namespace Sengine::Random