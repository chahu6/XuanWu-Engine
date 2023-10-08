#include "xwpch.h"
#include "ScriptGlue.h"

#include <glm/glm.hpp>
#include <mono/metadata/object.h>

namespace XuanWu
{
	#define XW_ADD_INTERNAL_CALL(Name) mono_add_internal_call("XuanWu.InternalCalls::" #Name, Name) 

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);

		mono_free(cStr);
		XW_CORE_TRACE("{},{}", str, parameter); // {}里面不写数字也可
	}

	static void NativeLog_Vector(glm::vec3* vec, glm::vec3* out)
	{
		//HZ_CORE_WARN("Value: {0}", *vec); // 这会错的，并不支持输出向量
		std::cout << vec->x << "," << vec->y << "," << vec->z << std::endl;
		//*out = glm::cross(*vec, glm::vec3(vec->x, vec->y, -vec->z)); // 通过out返回指针
		*out = glm::normalize(*vec);
	}

	static float NativeLog_VectorDot(glm::vec3* vec)
	{
		std::cout << vec->x << "," << vec->y << "," << vec->z << std::endl;
		return glm::dot(*vec, *vec);
	}

	void ScriptGlue::RegisterFunctions()
	{
		XW_ADD_INTERNAL_CALL(NativeLog);
		XW_ADD_INTERNAL_CALL(NativeLog_Vector);
		XW_ADD_INTERNAL_CALL(NativeLog_VectorDot);
	}
}
