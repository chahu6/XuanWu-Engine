#ifndef __CLASS_H__
#define __CLASS_H__

#include "type_info.h"

class Cursor;
class Field;

struct BaseClass
{
	BaseClass(const Cursor& cursor);

	std::string name;
};

class Class : public TypeInfo
{
	typedef std::vector<std::string> Namespace;

	template<typename T>
	using SharedPtrVector = std::vector<std::shared_ptr<T>>;

public:
	Class(const Cursor& cursor, const Namespace& current_namespace);

	bool ShouldCompile() const;

private:
	bool ShouldCompileFilds() const;

private:
	std::string m_name;
	std::string m_type_name;
	SharedPtrVector<BaseClass> m_base_classes;

	SharedPtrVector<Field> m_fields;

	std::string m_display_type_name;
	
public:
	inline const std::string& GetClassName() const { return m_name; }
	inline const std::string& GetDisplayName() const { return m_display_type_name; }
	inline bool IsAccessible() const { return m_enable; }
	inline const SharedPtrVector<BaseClass>& GetBaseClasses() const { return m_base_classes; }
	inline const SharedPtrVector<Field> GetFields() const { return m_fields; }
};

#endif // !__CLASS_H__
