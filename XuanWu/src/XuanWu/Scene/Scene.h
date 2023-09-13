#pragma once

#include <entt.hpp>
#include "XuanWu/Core/Timestep.h"

namespace XuanWu
{
	class Scene
	{
	public:
		friend class Entity;
		Scene();
		~Scene();

		Entity CreateEntity(const std::string_view name = "");

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		inline entt::registry& Reg() { return m_Registry; }
	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	};
}