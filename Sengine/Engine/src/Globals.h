#pragma once
#include <functional>
#include <memory>
#include <iterator>

#include <queue>
#include <set>
#include <random>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

//Disables Windows functions for Raylib.
#ifdef FE_RELEASE
#define NOGDI             
#define NOUSER
#endif

#include "imgui/imgui.h"

namespace Raylib
{
#include "raylib/raylib.h"

	namespace ImGui
	{
		#include "rlImGui/rlImGui.h"
	}//namespace Raylib::ImGui

	namespace GUI 
	{
		#include "raylib/raygui.h"
	}//namespace Raylib::GUI

	namespace Math
	{
		#include "raylib/raymath.h"
	}//namespace Raylib::Math
}///namespace Raylib

namespace Sengine
{
	namespace fs = std::filesystem;
}

#include "Engine/IO/Logger.h"
#include "Engine/IO/Filesystem.h"
