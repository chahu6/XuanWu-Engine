#pragma once

#include <glm/glm.hpp>

namespace XuanWu::Math
{	
	// �任����ķֽ⹫ʽ
	bool DecomponseTrasnform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
}