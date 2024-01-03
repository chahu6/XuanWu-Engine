#include "xwpch.h"
#include "reflection.h"

namespace XuanWu
{
	namespace Reflection
	{
		constexpr char* k_unknown_type	= "UnknownType";
		constexpr char* k_unknown		= "Unknown";

		static std::unordered_map<std::string, ClassFunctionTuple*>			m_class_map;
		static std::unordered_multimap<std::string, FieldFunctionTuple*>	m_field_map;
		static std::unordered_map<std::string, ArrayFunctionTuple*>			m_array_map;

		void TypeMetaRegisterInterface::registerToClassMap(const char* name, ClassFunctionTuple* value)
		{
			if (m_class_map.find(name) == m_class_map.end())
			{
				m_class_map.emplace(name, value);
			}
			else
			{
				delete value;
			}
		}

		void TypeMetaRegisterInterface::registerToFieldMap(const char* name, FieldFunctionTuple* value)
		{
			m_field_map.insert(std::make_pair(name, value));
		}

		void TypeMetaRegisterInterface::registerToArrayMap(const char* name, ArrayFunctionTuple* value)
		{
			if(m_array_map.find(name) == m_array_map.end())
			{
				m_array_map.emplace(name, value);
			}
			else
			{
				delete value;
			}
		}

		void TypeMetaRegisterInterface::unregisterAll()
		{
			for (const auto& itr : m_field_map)
			{
				delete itr.second;
			}
			m_field_map.clear();
			for (const auto& itr : m_class_map)
			{
				delete itr.second;
			}
			m_class_map.clear();
			for (const auto& itr : m_array_map)
			{
				delete itr.second;
			}
			m_array_map.clear();
		}


		Class::Class(const std::string_view name)
			:m_Name(name)
		{
			auto field_iter = m_field_map.equal_range(name.data());
			while (field_iter.first != field_iter.second)
			{
				// 这里不满意
				m_Fields.emplace_back(std::make_shared<Field>(field_iter.first->second));

				++field_iter.first;
			}
		}

		const std::string& Class::GetName() const
		{
			return m_Name;
		}

		IField* Class::GetField(const std::string_view name) const
		{
			const auto it = std::find_if(m_Fields.begin(), m_Fields.end(), [name](const auto& i)
			{
				return i->GetName() == name.data();
			});

			return ((*it).get());
		}

		IField* Class::GetStaticField(const std::string_view name) const
		{
			return nullptr;
		}

		std::vector<std::unique_ptr<IFunction>> Class::GetMethod(const std::string_view name) const
		{
			return std::vector<std::unique_ptr<IFunction>>();
		}


		Field::Field()
		{
			m_FieldTypeName = k_unknown_type;
			m_FieldName = k_unknown;
			m_Functions = nullptr;
		}

		Field::Field(FieldFunctionTuple* functions)
			:m_Functions(functions)
		{
			if (m_Functions == nullptr) return;

			m_FieldName = (std::get<3>(*m_Functions))();
			m_FieldTypeName = (std::get<4>(*m_Functions))();
		}

		const std::string& Field::GetName() const
		{
			return m_FieldName;
		}

		void Field::SetValue(void* obj, void* value) const
		{
			(std::get<0>(*m_Functions))(obj, value);
		}

		void* Field::GetValue(void* obj) const
		{
			return static_cast<void*>((std::get<1>(*m_Functions))(obj));
		}

		const std::string& Field::GetFieldName() const
		{
			return m_FieldName;
		}

		Field& Field::operator=(const Field& dest)
		{
			if (this == &dest)
			{
				return *this;
			}
			m_Functions = dest.m_Functions;
			m_FieldName = dest.m_FieldName;
			m_FieldTypeName = dest.m_FieldTypeName;
			return *this;
		}

		const std::string& Field::GetFieldTypeName() const
		{
			return m_FieldTypeName;
		}

		bool Field::IsArrayType() const
		{
			return (std::get<5>(*m_Functions))();
		}
	}
}