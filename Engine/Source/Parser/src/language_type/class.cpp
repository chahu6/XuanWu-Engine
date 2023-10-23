#include "xwpch.h"
#include "class.h"

#include "Cursor/cursor.h"
#include "field.h"
#include "Meta/meta_info.h"
#include "Cursor/cursor_type.h"

BaseClass::BaseClass(const Cursor& cursor)
	:name(Utils::GetTypeNameWithoutNamespace(cursor.GetType()))
{}

Class::Class(const Cursor& cursor, const Namespace& current_namespace)
	:TypeInfo(cursor, current_namespace), m_name(cursor.GetDisplayName()),
	m_type_name(Utils::GetTypeNameWithoutNamespace(cursor.GetType())),
	m_display_type_name(Utils::GetNameWithoutFirstM(m_type_name))
{
	// 这两个好像没有声明用，因为不可能发生
	Utils::ReplaceAll(m_name, " ", "");
	Utils::ReplaceAll(m_name, "XuanWu::", "");

	for (auto& child : cursor.GetChildren())
	{
		switch (child.GetKind())
		{
			case CXCursor_CXXBaseSpecifier:
			{
				auto base_class = new BaseClass(child);

				m_base_classes.emplace_back(base_class);
				break;
			}
			case CXCursor_FieldDecl:
			{
				m_fields.emplace_back(new Field(child, current_namespace, this));
				break;
			}
		default:
			break;
		}
	}
}

bool Class::ShouldCompile() const
{
	return ShouldCompileFilds();
}

bool Class::ShouldCompileFilds() const
{
	return m_meta_data.GetFlag(NativeProperty::All) || m_meta_data.GetFlag(NativeProperty::Fields) ||
		m_meta_data.GetFlag(NativeProperty::WhiteListFields);
}
