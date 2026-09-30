#pragma once

namespace Sengine
{
	[[nodiscard]] uint64_t GetRandomInt64Value();

	class UUID
	{
	public:

		UUID()
			: m_Value(GetRandomInt64Value()) { }

		explicit constexpr UUID(uint64_t value)
			: m_Value(value) { }

		[[nodiscard]] uint64_t GetValue();
		[[nodiscard]] bool IsValid();

		[[nodiscard]] operator uint64_t() const
		{
			return m_Value;
		}

	private:
		uint64_t m_Value{ 0 };
	};

}//namespace Sengine::Random
