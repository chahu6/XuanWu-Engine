#pragma once
#include "Serializer.h"

namespace XuanWu
{
	class SceneSerializer : public Serializer
	{
	public:
		SceneSerializer(Ref<Scene>& scene);

		virtual void Serialize(const std::string_view filepath) override;
		virtual void SerializeRuntime(const std::string_view filepath) override;

		virtual bool Deserialize(const std::string_view filepath) override;
		virtual bool DeserializeRuntime(const std::string_view filepath) override;
	protected:
		Ref<Scene> m_Scene;
	};
}