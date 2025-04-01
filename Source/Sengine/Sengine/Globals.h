/**
 *	Filename: Globals.h
 *	Date Created: 21/02/25
 *	Purpose: A recompiled header to include all necessary include files
 ***/
#pragma once

/* C++ standard library includes */
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <cstddef>

#include <string>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <random>
#include <any>
#include <array>
#include <map>
#include <chrono>

#ifdef SE_PLATFORM_WINDOWS
#include <Windows.h>
#endif //SE_PLATFORM_WINDOWS

/* Core Sengine includes */

#include "Core/Types.h"

#include "Core/Logging/Assert.h"
#include "Core/Logging/Logger.h"

#include "Core/IO/Filesystem.h"
#include "Core/IO/JsonSerialization.h"

#include "Core/Time.h"

#include "Core/Platform/Platform.h"

#include "Core/Math/Math.h"

/* Sengine Generic Includes */

#include "Applicatiom/Application.h"
#include "Applicatiom/ApplicationState.h"
#include "Applicatiom/Window/Window.h"