#pragma once

#include <entt.hpp>
#include "XuanWu/Core/Timestep.h"

namespace XuanWu
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		void OnUpdate(Timestep ts);

		inline entt::registry& Reg() { return m_Registry; }
	private:
		entt::registry m_Registry;
	};
}