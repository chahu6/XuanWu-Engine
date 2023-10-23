#pragma once

#include "GLFW/glfw3.h"

namespace XuanWu {

	class Platform
	{
	public:
		inline static float GetTime() { return static_cast<float>(glfwGetTime()); }
	};
}