#pragma once

#include "Engine/Game/Scripting/Script.h"

namespace Sengine
{
	struct ScriptComponent
	{
		std::shared_ptr<Scripting::Script> Script;
	};
}//namespace Sengine
