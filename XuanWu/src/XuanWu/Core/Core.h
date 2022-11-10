#pragma once

#include <memory>

#ifdef XW_PLATFORM_WINDOWS
	#if XW_DYNAMIC_LINK
		#ifdef XW_BUILD_DLL
			#define XUANWU_API __declspec(dllexport)
		#else
			#define XUANWU_API __declspec(dllimport)
		#endif // XW_BUILD_DLL
	#else
		#define XUANWU_API
	#endif
#else
	#error XuanWu only supports Windows!
#endif // XW_PLATFORM_WINDOWS

#ifdef XW_DEBUG
	#define XW_ENABLE_ASSERTS
#endif

#ifdef XW_ENABLE_ASSERTS
	#define XW_ASSERT(x, ...) { if(!(s)) { XW_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define XW_CORE_ASSERT(x, ...) { if(!(x)) { XW_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define XW_ASSERT(x, ...)
	#define XW_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define XW_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace XuanWu {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}