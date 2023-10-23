#include "xwpch.h"
#include "field.h"

#include "Cursor/cursor.h"
#include "Cursor/cursor_type.h"
#include "Meta/meta_info.h"
#include "class.h"

Field::Field(const Cursor& cursor, const Namespace& current_namespace, Class* parent)
	:TypeInfo(cursor, current_namespace), m_is_const(cursor.GetType().IsConst()), m_parent(parent),
	m_name(cursor.GetSpelling()), m_display_name(Utils::GetNameWithoutFirstM(m_name)),
	m_type_name(Utils::GetTypeNameWithoutNamespace(cursor.GetType()))
{
	Utils::ReplaceAll(m_type_name, " ", "");
	Utils::ReplaceAll(m_type_name, "XuanWu::", "");

	auto&& ret_string = Utils::GetStringWithoutQuot(m_meta_data.GetProperty("default"));
	m_default = ret_string;
}

bool Field::ShouldCompile() const
{
	return IsAccessible();
}

bool Field::IsAccessible() const
{
	return ((m_parent->GetMetaData().GetFlag(NativeProperty::Fields) ||
		m_parent->GetMetaData().GetFlag(NativeProperty::All)) &&
		!m_meta_data.GetFlag(NativeProperty::Disable)) ||
		(m_parent->GetMetaData().GetFlag(NativeProperty::WhiteListFields) &&
			m_meta_data.GetFlag(NativeProperty::Enable));
}
