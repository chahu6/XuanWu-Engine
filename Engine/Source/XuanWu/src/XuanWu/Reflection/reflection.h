#pragma once
#include "reflection_type.h"

namespace XuanWu
{
#if defined(__REFLECTION_PARSER__)
#define META(...) __attribute__((annotate(#__VA_ARGS__)))
#define CLASS(class_name, ...) class __attribute__((annotate(#__VA_ARGS__))) class_name
#define STRUCT(struct_name, ...) struct __attribute__((annotate(#__VA_ARGS__))) struct_name
#else
#define META(...)
#define CLASS(class_name, ...) class class_name
#define STRUCT(struct_name, ...) struct struct_name
#endif

#define REFLECTION_BODY(class_name) \
	friend class Reflection::TypeFieldReflectionOperator::Type##class_name##Operator;	\
public:		\
	static Reflection::IClass* StaticClass()	\
	{	\
		static Reflection::Class s_##class_name##Class(#class_name);	\
		return &s_##class_name##Class;	\
	}
	//friend class PSerializer;

#define REFLECTION_TYPE(class_name) \
	namespace Reflection \
	{	\
		namespace TypeFieldReflectionOperator \
		{	\
			class Type##class_name##Operator;	\
		}	\
	}

#define REGISTER_FIELD_TO_MAP(name, value) TypeMetaRegisterInterface::registerToFieldMap(name, value);
#define REGISTER_BASE_CLASS_TO_MAP(name, value) TypeMetaRegisterInterface::registerToClassMap(name, value);
#define REGISTER_ARRAY_TO_MAP(name, value) TypeMetaRegisterInterface::registerToArrayMap(name, value);
#define UNREGISTER_ALL TypeMetaRegisterInterface::unregisterAll();


	template<typename T, typename U, typename = void> // 第三个模板参数是占位用的，用于下面
	struct is_safely_castable : std::false_type {};

	template<typename T, typename U>
	struct is_safely_castable<T, U, std::void_t<decltype(static_cast<U>(std::declval<T>))>> : std::true_type {};

	namespace Reflection
	{
		class TypeMeta;
		class FieldAccessor;
		class ArrayAccessor;
		class ReflectionInstance;
	}

	typedef std::function<void(void*, void*)>		SetFunction;
	typedef std::function<void* (void*)>			GetFunction;
	typedef std::function<const char* ()>			GetNameFunction;
	typedef std::function<void(int, void*, void*)>	SetArrayFunc;
	typedef std::function<void* (int, void*)>		GetArrayFunc;
	typedef std::function<int(void*)>				GetSizeFunc;
	typedef std::function<bool()>					GetBoolFunc;

	typedef std::function<int(Reflection::ReflectionInstance*&, void*)> GetBaseClassReflectionInstanceListFunc;

	typedef std::tuple<GetBaseClassReflectionInstanceListFunc>	ClassFunctionTuple;
	typedef std::tuple<SetFunction, GetFunction, GetNameFunction, GetNameFunction, GetNameFunction, GetBoolFunc> FieldFunctionTuple;

	typedef std::tuple<SetArrayFunc, GetArrayFunc, GetSizeFunc, GetNameFunction, GetNameFunction> ArrayFunctionTuple;

	namespace Reflection
	{
		class TypeMetaRegisterInterface
		{
		public:
			static void registerToClassMap(const char* name, ClassFunctionTuple* value);
			static void registerToFieldMap(const char* name, FieldFunctionTuple* value);
			static void registerToArrayMap(const char* name, ArrayFunctionTuple* value);

			static void unregisterAll();
		};

		
		class Class : public IClass
		{
		public:
			Class(const std::string_view name);

			virtual const std::string& GetName() const override;

			virtual IField* GetField(const std::string_view name) const override;

			virtual IField* GetStaticField(const std::string_view name) const override;

			virtual std::vector<std::unique_ptr<IFunction>> GetMethod(const std::string_view name) const override;

		private:
			std::string m_Name;
			std::vector<std::shared_ptr<IField>> m_Fields;
			std::vector<std::shared_ptr<IFunction>> m_Functions;
		};

		class Field : public IField
		{
		public:
			Field();
			Field(FieldFunctionTuple* functions);

			virtual const std::string& GetName() const override;

			virtual void SetValue(void* obj, void* value) const override;
			virtual void* GetValue(void* obj) const override;

			virtual const std::string& GetFieldName() const override;
			virtual const std::string& GetFieldTypeName() const override;
			virtual bool IsArrayType() const override;

			Field& operator=(const Field& dest);

		private:
			FieldFunctionTuple* m_Functions;

			std::string m_FieldName;
			std::string m_FieldTypeName;
		};
	}
}