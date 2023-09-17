#pragma once

#include "XuanWu/Scene/Scene.h"

namespace XuanWu
{
	class Serializer
	{
	public:
		virtual void SetContext(Ref<Scene>& context) = 0;

		virtual void Serialize(const std::string_view filepath) = 0;
		virtual void SerializeRuntime(const std::string_view filepath) = 0;

		virtual bool Deserialize(const std::string_view filepath) = 0;
		virtual bool DeserializeRuntime(const std::string_view filepath) = 0;

		static Ref<Serializer> Create(Ref<Scene>& scene);
	};
}