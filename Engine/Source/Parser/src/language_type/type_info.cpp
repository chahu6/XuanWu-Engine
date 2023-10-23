#include "xwpch.h"
#include "type_info.h"

#include "Meta/meta_data_config.h"

TypeInfo::TypeInfo(const Cursor& cursor, const Namespace& current_namespace)
	:m_root_cursor(cursor), m_meta_data(cursor), m_enable(m_meta_data.GetFlag(NativeProperty::Enable)),
	m_namespace(current_namespace)
{}

const MetaInfo& TypeInfo::GetMetaData() const
{
	return m_meta_data;
}

std::string TypeInfo::GetSourceFile() const
{
	return m_root_cursor.GetSourceFile();
}
