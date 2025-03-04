#include "Globals.h"
#include "InputManagerSerialization.h"

namespace Sengine
{
	void InputManagerSerialization::SerializeData(Json& out)
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
	}

	void InputManagerSerialization::DeserializeData(Json& in)
	{

	}
}