#pragma once

namespace Sengine
{
	class InputManagerSerialization : public ISerializable
	{
	public:
		InputManagerSerialization(ActionMap map)
			: m_ActionMap(std::move(map)) {}

		virtual ~InputManagerSerialization() override = default;

		virtual void SerializeData(Json& out) override;
		virtual void DeserializeData(Json& in) override;

	private:
		ActionMap m_ActionMap;
	};
}


