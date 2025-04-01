#pragma once
#include "Applicatiom/Input/InputManager.h"

namespace Sengine
{
	class InputManagerSerialization : public ISerializable
	{
	public:
		InputManagerSerialization(ActionMap map)
			: m_ActionMap(std::move(map)) {}

		virtual ~InputManagerSerialization() override = default;

		virtual void SerializeData(nlohmann::json& out) override;
		virtual void DeserializeData(nlohmann::json& in) override;

	private:
		ActionMap m_ActionMap;
	};
}//namespace Sengine