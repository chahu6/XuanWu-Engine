#pragma once

#include "Actor.h"

namespace XuanWu
{
	class Character : public Actor
	{
	public:
		Character();

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
	};
}