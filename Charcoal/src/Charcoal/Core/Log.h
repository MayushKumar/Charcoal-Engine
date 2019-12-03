#pragma once

#include <chpch.h>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Charcoal 
{

	class Log
	{

	private:
		Log() {};
		~Log() {};

	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; };
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; };

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};

}

#define CH_CORE_TRACE(...) ::Charcoal::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CH_CORE_INFO(...) ::Charcoal::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CH_CORE_WARN(...) ::Charcoal::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CH_CORE_CRITICAL(...) ::Charcoal::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CH_CORE_ERROR(...) ::Charcoal::Log::GetCoreLogger()->error(__VA_ARGS__)
//#define CH_CORE_FATAL(...) ::Charcoal::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define CH_TRACE(...) ::Charcoal::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CH_INFO(...) ::Charcoal::Log::GetClientLogger()->info(__VA_ARGS__)
#define CH_WARN(...) ::Charcoal::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CH_CRITICAL(...) ::Charcoal::Log::GetClientLogger()->critical(__VA_ARGS__)
#define CH_ERROR(...) ::Charcoal::Log::GetClientLogger()->error(__VA_ARGS__)
//#define CH_FATAL(...) ::Charcoal::Log::GetClientLogger()->fatal(__VA_ARGS__)