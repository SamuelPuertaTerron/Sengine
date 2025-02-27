/*
 * The Entry point to the Sengine Editor
 */

#include "Sengine/Globals.h"
#include "Sengine/Applicatiom/Main.h"

#include "Editor.h"

inline Sengine::SengineAppPtr CreateApp()
{
	return Sengine::MakeSengineAppPtr<SengineEditor::Editor>();
}