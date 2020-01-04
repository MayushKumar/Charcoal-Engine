#pragma once

#include <memory>
#include <utility>
#include <iostream>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <Charcoal/Core/Core.h>
#include <Charcoal/Debug/Instrumentor.h>

#ifdef CH_PLATFORM_WINDOWS
	#include <Windows.h>
#else 
	#error Charcoal only supports Windows currently
#endif