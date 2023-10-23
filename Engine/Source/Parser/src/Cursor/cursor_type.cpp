#include "xwpch.h"
#include "cursor_type.h"

CursorType::CursorType(const CXType& handle)
	:m_handle(handle)
{
}

std::string CursorType::GetDisplayName() const
{
	std::string display_name;

	Utils::ToString(clang_getTypeSpelling(m_handle), display_name);

	return display_name;
}

bool CursorType::IsConst() const
{
	return clang_isConstQualifiedType(m_handle) ? true : false;
}
