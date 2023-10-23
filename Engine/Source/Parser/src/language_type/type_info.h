#ifndef __TYPE_INFO_H__
#define __TYPE_INFO_H__
#include "Cursor/cursor.h"
#include "Meta/meta_info.h"

class Cursor;
class MetaInfo;

class TypeInfo
{
	typedef std::vector<std::string> Namespace;
public:
	TypeInfo(const Cursor& cursor, const Namespace& current_namespace);
	virtual ~TypeInfo() = default;

	const MetaInfo& GetMetaData() const;

	std::string GetSourceFile() const;

protected:
	MetaInfo m_meta_data;
	bool m_enable;
	std::string m_alias_cn;
	Namespace m_namespace;

private:
	Cursor m_root_cursor;

public:
	inline const MetaInfo& GetMetaInfo() const { return m_meta_data; }
};

#endif // !__TYPE_INFO_H__
