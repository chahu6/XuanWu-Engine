#ifndef __FIELD_H__
#define __FIELD_H__

#include "type_info.h"

class Class;

class Field : public TypeInfo
{
	typedef std::vector<std::string> Namespace;
public:
	Field(const Cursor& cursor, const Namespace& current_namespace, Class* parent = nullptr);
	bool ShouldCompile() const;

private:
	bool IsAccessible() const;

private:
	bool m_is_const;

	Class* m_parent;

	std::string m_name;
	std::string m_display_name;
	std::string m_type_name;

	std::string m_default;

public:
	inline const std::string& GetName() const { return m_name; }
	inline const std::string& GetTypeName() const { return m_type_name; }
	inline const std::string& GetDisplayName() const { return m_display_name; }
};
#endif // !__FIELD_H__
