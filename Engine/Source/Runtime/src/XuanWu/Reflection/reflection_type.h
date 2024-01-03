#pragma once

namespace XuanWu
{
	namespace Reflection
	{
		class IType
		{
		public:
			virtual ~IType() = default;

			virtual const std::string& GetName() const = 0;
		};

		struct Parameter
		{
			std::string Name;
			std::string Type;
		};

		class IFunction : public IType
		{
		public:
			virtual int GetParameterCound() const = 0;
			virtual Parameter GetReturnType() const = 0;
			virtual Parameter GetParameter(int i) const = 0;

			template<typename... Ts>
			std::any operator()(Ts... ts)
			{

			}

			virtual std::any Invoke(const std::vector<std::any>& args) = 0;
		};

		class IField : public IType
		{
		public:
			virtual void SetValue(void* obj, void* value) const = 0;
			virtual void* GetValue(void* obj) const = 0;

			virtual const std::string& GetFieldName() const = 0;
			virtual const std::string& GetFieldTypeName() const = 0;
			virtual bool IsArrayType() const = 0;
		};

		class IClass : public IType
		{
		public:
			virtual IField* GetField(const std::string_view name) const = 0;

			virtual IField* GetStaticField(const std::string_view name) const = 0;

			virtual std::vector<std::unique_ptr<IFunction>> GetMethod(const std::string_view name) const = 0;
		};

		class IEnum : public IType
		{
		public:
			virtual std::vector<std::string> GetStringValues() const = 0;
			virtual std::vector<int> GetIntValues() const = 0;
			virtual bool TryTranslate(const std::string_view value, int& out) = 0;
			virtual bool TryTranslate(int value, std::string_view out) = 0;
		};

		namespace registry
		{
			std::vector<IType*> GetByName(const std::string_view name);
			std::vector<IFunction*> GetFunctionByName(const std::string_view name);
			IClass* GetClassByName(const std::string_view name);
			IEnum* GetEnumByName(const std::string_view name);

			namespace internal
			{
				void Register(std::unique_ptr<IFunction>&& f);
				void Register(std::unique_ptr<IClass>&& f);
				void Register(std::unique_ptr<IEnum>&& e);
			}
		}

		/*class Exception : virtual public std::exception
		{
		public:
			Exception(std::string error);
			const char* what() const noexcept override;

		private:
			std::string what_;
		};*/

		template<>
		std::any IFunction::operator() < > ();
	}
}