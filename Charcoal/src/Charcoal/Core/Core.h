#pragma once

#include <memory>
#include "Charcoal/Core/Log.h"

#ifndef CH_PLATFORM_WINDOWS
#error Charcoal only supports Windows currently
#endif

#define BIT(x) 1 << x

#ifdef CH_ENABLE_ASSERTS
	#define CH_CORE_ASSERT(x, ...) if (!x) { CH_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); }
	#define CH_ASSERT(x, ...) if (!x) { CH_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); }
#else
	#define CH_CORE_ASSERT(x, ...)
	#define CH_ASSERT(x, ...)
#endif

#define CH_BIND_EVENT_FUNC(func) std::bind(&func, this, std::placeholders::_1)

namespace Charcoal
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}