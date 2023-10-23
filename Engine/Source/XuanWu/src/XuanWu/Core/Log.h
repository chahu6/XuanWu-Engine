#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace XuanWu {

	class XUANWU_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

//Core Log macros
#define XW_CORE_TRACE(...)		::XuanWu::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define XW_CORE_INFO(...)		::XuanWu::Log::GetCoreLogger()->info(__VA_ARGS__)
#define XW_CORE_WARN(...)		::XuanWu::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define XW_CORE_ERROR(...)		::XuanWu::Log::GetCoreLogger()->error(__VA_ARGS__)
#define XW_CORE_FATAL(...)		::XuanWu::Log::GetCoreLogger()->fatal(__VA_ARGS__)


//Client Log macros
#define XW_TRACE(...)			::XuanWu::Log::GetClientLogger()->trace(__VA_ARGS__)
#define XW_INFO(...)			::XuanWu::Log::GetClientLogger()->info(__VA_ARGS__)
#define XW_WARN(...)			::XuanWu::Log::GetClientLogger()->warn(__VA_ARGS__)
#define XW_ERROR(...)			::XuanWu::Log::GetClientLogger()->error(__VA_ARGS__)
#define XW_FATAL(...)			::XuanWu::Log::GetClientLogger()->fatal(__VA_ARGS__)
