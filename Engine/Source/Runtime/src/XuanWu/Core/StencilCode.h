#pragma once

namespace XuanWu
{
	using StencilCode = unsigned int;
	namespace Stencil
	{
		enum : StencilCode
		{
			Keep		=	0x1E00,
			Replace		=	0x1E01,

			Never		=	0x0200,
			Less		=	0x0201,
			Equal		=	0x0202,
			LEqual		=	0x0203,
			Greater		=	0x0204,
			NotEqual	=	0x0205,
			GEqual		=	0x0206,
			Always		=	0x0207,
			Zero		=	0,
			One			=	1
		};
	}
}