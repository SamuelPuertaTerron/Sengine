/**
 *	Filename: Main.cpp
 *	Date Created: 27/02/25
 *	Purpose: The entry point to the Sengine Editor
 ***/

#include "Sengine/Globals.h"
#include "Sengine/Applicatiom/Main.h"

#include "Editor.h"

inline std::shared_ptr<Sengine::ISengineApp> CreateApp()
{
	return std::make_shared<SengineEditor::Editor>();
}