#pragma once
#include <XuanWu/Reflection/reflection.h>
#include <glm/glm.hpp>

namespace XuanWu
{
	REFLECTION_TYPE(Color)
	CLASS(Color, Fields)
	{
		REFLECTION_BODY(Color)
	public:
		Color(float r, float g, float b)
			:r(r), g(g), b(b) {}
		glm::vec3 toVector3() const { return glm::vec3(r, g, b); }
	private:
		float r;
		float g;
		float b;
	};
}