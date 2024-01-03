#include "xwpch.h"
#include "reflection_register.h"

#include "reflection.h"

namespace XuanWu
{
	namespace Reflection
	{
		void TypeMetaRegister::Unregister()
		{
			TypeMetaRegisterInterface::unregisterAll();
		}
	}
}
