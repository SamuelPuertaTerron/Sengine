/**
 *	Filename: Main.cpp
 *	Date Created: 27/02/25
 *	Purpose: The entry point to the Sengine Editor
 *	Last Modified: N/A
 *	Changes:	   N/A
 ***/

#include "Sengine/Globals.h"
#include "Sengine/Applicatiom/Main.h"

#include "Editor.h"

inline Sengine::SengineAppPtr CreateApp()
{
	return Sengine::MakeSengineAppPtr<SengineEditor::Editor>();
}