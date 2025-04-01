#include "Globals.h"
#include "InputManagerSerialization.h"

namespace Sengine
{
	void InputManagerSerialization::SerializeData(nlohmann::json& out)
	{
		out["Action Map Name"] = m_ActionMap.Name;

		int index = 0;
		for (auto& [actionName, action] : m_ActionMap.ActionList)
		{
			int actionCode = -1;
			if (action.Code.type() == typeid(EKeyCode))
			{
				actionCode = static_cast<int>(std::any_cast<EKeyCode>(action.Code));
			}
			if (action.Code.type() == typeid(EMouseButton))
			{
				actionCode = std::any_cast<int>(action.Code);
			}

			out["Action " + std::to_string(index)]["Name"] = actionName;
			out["Action " + std::to_string(index)]["Code"] = actionCode;
			index++;
		}

		out["Action Index"] = index;
	}

	void InputManagerSerialization::DeserializeData(nlohmann::json& in)
	{
		m_ActionMap = {};

		m_ActionMap.Name = in["Action Map Name"];

		int index = in["Action Index"].get<int>();

		for (int i = 0; i < index; i++)
		{
			std::string actionName = EmptyString;
			std::any code = {};

			actionName = in["Action " + std::to_string(i)]["Name"].get<std::string>();
			code = static_cast<EKeyCode>(in["Action " + std::to_string(i)]["Code"].get<int>());
		}
	}
}//namespace Sengine