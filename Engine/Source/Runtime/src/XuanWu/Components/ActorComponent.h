#pragma once
#include "XuanWu/CoreUObject/Object.h"

namespace XuanWu
{
	class ActorComponent : public Object
	{
	public:
		virtual void BeginPlay();
		virtual void TickComponent(float DeltaTime);
	};
}