#pragma once

#include <memory>
#include "Charcoal/Core/Log.h"

#if !(defined CH_PLATFORM_LINUX || defined CH_PLATFORM_WINDOWS)
	#error Charcoal only supports Windows & Linux currently
#endif

#define BIT(x) 1 << x

#ifdef CH_ENABLE_ASSERTS
	#ifdef CH_PLATFORM_WINDOWS
		#define CH_CORE_ASSERT(x, ...) if (!x) { CH_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); }
		#define CH_ASSERT(x, ...) if (!x) { CH_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); }
	#elif defined CH_PLATFORM_LINUX
		#include <signal.h>
		#define CH_CORE_ASSERT(x, ...) if (!x) { CH_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); raise(SIGTRAP); }
		#define CH_ASSERT(x, ...) if (!x) { CH_ERROR("Assertion failed: {0}", __VA_ARGS__); raise(SIGTRAP); }
	#endif
#else
	#define CH_CORE_ASSERT(x, ...)
	#define CH_ASSERT(x, ...)
#endif

#define CH_BIND_EVENT_FUNC(func) std::bind(&func, this, std::placeholders::_1)

namespace Charcoal
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args) ...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args) ...);
	}
}
