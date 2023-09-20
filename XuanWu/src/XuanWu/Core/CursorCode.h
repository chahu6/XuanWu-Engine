#pragma once

namespace XuanWu
{
	using CursorCode = int;
	namespace Cursor
	{
		enum : CursorCode
		{
			CursorNormal			= 0x00034001,
			CursorHidden			= 0x00034002,
			CursorDisabled			= 0x00034003,
			CursorCaptured			= 0x00034004
		};
	}
}