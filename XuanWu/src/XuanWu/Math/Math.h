#pragma once

#include <glm/glm.hpp>

namespace XuanWu::Math
{	
	// 变换矩阵的分解公式
	bool DecomponseTrasnform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

	template<typename T>
	int Sign(T x)
	{
		return (T(0) < x) - (x < T(0));
	}
}