#pragma once
#include <tuple>
#include <vector>
#include <assert.h>

#define DECLARE_DELEGATE(DelegateName, ...) typedef DELEGATE::Delegate<void, __VA_ARGS__> DelegateName

#define DECLARE_DELEGATE_Ret(ReturnType, DelegateName, ...) typedef DELEGATE::Delegate<ReturnType, __VA_ARGS__> DelegateName

#define DECLARE_MULTICAST_DELEGATE(DelegateName, ...) typedef DELEGATE::MulticastDelegate<__VA_ARGS__> DelegateName

namespace DELEGATE
{
	template<typename RetVal, typename... Args>
	class IDelegate
	{
	public:
		inline virtual RetVal Execute(Args... args) = 0;
		virtual ~IDelegate() = default;
	};

	template<bool bIsConst, class C, typename Res, typename... ParamsType>
	struct TMemFunPtrType;
	template<class C, typename Res, typename... ParamsType, typename... ValType>
	struct TMemFunPtrType<false, C, Res(ParamsType...), ValType...>
	{
		using Type = Res(C::*)(ParamsType..., ValType...);
	};
	template<class C, typename Res, typename... ParamsType, typename... ValType>
	struct TMemFunPtrType<true, C, Res(ParamsType...), ValType...>
	{
		using Type = Res(C::*)(ParamsType..., ValType...) const;
	};

	// 绑定不同类型委托的具体实现

	/**
	* 绑定类成员函数的委托
	*/
	template<bool bIsConst, class C, typename Res, typename... ParamsType>
	class MemberDelegate;
	template<bool bIsConst, class C, typename Res, typename... ParamsType, typename... ValType>
	class MemberDelegate<bIsConst, C, Res(ParamsType...), ValType...> : public IDelegate<Res, ParamsType...>
	{
		using MethodPtr = typename TMemFunPtrType<bIsConst, C, Res(ParamsType...), ValType...>::Type;
	public:
		MemberDelegate() = default;
		MemberDelegate(C* p, MethodPtr FuncPtr, ValType... payload)
			: m_p(p), Callback(FuncPtr), m_payload(std::move(payload)...)
		{

		}

		MemberDelegate(C* p, MethodPtr FuncPtr, const std::tuple<ValType...>& payload)
			: m_p(p), Callback(FuncPtr), m_payload(payload)
		{

		}

		~MemberDelegate() = default;

	public:
		inline virtual Res Execute(ParamsType... args)
		{
			return Execute_Internal(std::move(args)..., std::make_index_sequence<sizeof...(ValType)>());
		}

	private:
		template<std::size_t... Index>
		inline Res Execute_Internal(ParamsType... args, std::index_sequence<Index...>)
		{
			return (m_p->*Callback)(std::move(args)..., std::get<Index>(m_payload)...);
		}

	private:
		MethodPtr Callback;	// 函数指针
		C* m_p;
		std::tuple<ValType...> m_payload;
	};


	/**
	* 绑定全局函数或类静态函数
	*/
	template<typename Res, typename... ParamsType>
	class StaticDelegate;
	template<typename Res, typename... ParamsType, typename... ValType>
	class StaticDelegate<Res(ParamsType...), ValType...> : public IDelegate<Res, ParamsType...>
	{
	public:
		StaticDelegate() = default;
		StaticDelegate(Res(*FuncPtr)(ParamsType..., ValType...), ValType... payload)
			: Callback(FuncPtr), m_payload(std::forward<ValType>(payload)...)
		{}

		StaticDelegate(Res(*FuncPtr)(ParamsType..., ValType...), const std::tuple<ValType...>& payload)
			: Callback(FuncPtr), m_payload(payload)
		{}

		~StaticDelegate() = default;

	public:
		inline virtual Res Execute(ParamsType... args)
		{
			return Execute_Internal(std::move(args)..., std::make_index_sequence<sizeof...(ValType)>());
		}

	private:
		template<std::size_t... Index>
		inline Res Execute_Internal(ParamsType... args, std::index_sequence<Index...>)
		{
			return Callback(std::move(args)..., std::get<Index>(m_payload)...);
		}

	private:
		Res(*Callback)(ParamsType..., ValType...);
		std::tuple<ValType...> m_payload;
	};

	/**
	* 绑定Lambda表达式
	*/
	template<typename TLambda, typename Res, typename... ParamsType>
	class LambdaDelegate;
	template<typename TLambda, typename Res, typename... ParamsType, typename... ValType>
	class LambdaDelegate<TLambda, Res(ParamsType...), ValType...> : public IDelegate<Res, ParamsType...>
	{
	public:
		LambdaDelegate() = default;
		LambdaDelegate(TLambda Lambda, ValType... payload)
			: m_Lambda(Lambda), m_payload(std::move(payload)...)
		{}
		LambdaDelegate(TLambda Lambda, const std::tuple<ValType...>& payload)
			: m_Lambda(Lambda), m_payload(payload)
		{}

		~LambdaDelegate() = default;
	public:
		inline virtual Res Execute(ParamsType... args)
		{
			return Execute_Internal(std::move(args)..., std::make_index_sequence<sizeof...(ValType)>());
		}

	private:
		template<std::size_t... Index>
		inline Res Execute_Internal(ParamsType... args, std::index_sequence<Index...>)
		{
			return (Res)(m_Lambda(std::move(args)..., std::get<Index>(m_payload)...));
		}

	private:
		TLambda m_Lambda;
		std::tuple<ValType...> m_payload;
	};

	// 真正使用的委托类
	template<typename Res, typename... ParamsType>
	class Delegate
	{
	public:
		Delegate() = default;
		~Delegate() = default;

		template<class C, typename... ValType>
		inline void CreateMember(C* pObject, Res(C::* FuncPtr)(ParamsType..., ValType...), ValType... payload)
		{
			p = new MemberDelegate<false, C, Res(ParamsType...), ValType...>(pObject, FuncPtr, std::move(payload)...);
		}
		template<class C, typename... ValType>
		inline void CreateMember(C* pObject, Res(C::* FuncPtr)(ParamsType..., ValType...) const, ValType... payload)
		{
			p = new MemberDelegate<true, C, Res(ParamsType...), ValType...>(pObject, FuncPtr, std::move(payload)...);
		}
		template<class C, typename... ValType>
		inline void BindMember(C* pObject, Res(C::* FuncPtr)(ParamsType..., ValType...), ValType... payload)
		{
			CreateMember<C, ValType...>(pObject, FuncPtr, std::forward<ValType>(payload)...);
		}
		template<class C, typename... ValType>
		inline void BindMember(C* pObject, Res(C::* FuncPtr)(ParamsType..., ValType...) const, ValType... payload)
		{
			CreateMember<C, ValType...>(pObject, FuncPtr, std::forward<ValType>(payload)...);
		}

		// 处理子类指针调用父类函数或父类指针指向子类对象调用子类函数
		template<class C, class D, typename... ValType>
		inline void BindMember(C* pObject, Res(D::* FuncPtr)(ParamsType..., ValType...), ValType... payload)
		{
			D* pObject2 = static_cast<C*>(pObject);
			if (pObject2)
			{
				CreateMember<D, ValType...>(pObject2, FuncPtr, std::forward<ValType>(payload)...);
			}
		}
		template<class C, class D, typename... ValType>
		inline void BindMember(C* pObject, Res(D::* FuncPtr)(ParamsType..., ValType...) const, ValType... payload)
		{
			D* pObject2 = static_cast<C*>(pObject);
			if (pObject2)
			{
				CreateMember<D, ValType...>(pObject2, FuncPtr, std::forward<ValType>(payload)...);
			}
		}

		// 绑定类的静态函数或全局函数
		template<typename... ValType>
		inline void CreateStatic(Res(*FuncPtr)(ParamsType..., ValType...), ValType... payload)
		{
			p = new StaticDelegate<Res(ParamsType...), ValType...>(FuncPtr, std::forward<ValType>(payload)...);
		}
		template<typename... ValType>
		inline void BindStatic(Res(*FuncPtr)(ParamsType..., ValType...), ValType... payload)
		{
			CreateStatic<ValType...>(FuncPtr, std::forward<ValType>(payload)...);
		}

		// 绑定Lambda表达式，新增Lambda表达式的检查，是都是可以运行的
		template<typename TLambda, typename... ValType>
		void CreateLambda(TLambda Lambda, ValType... payload)
		{
			p = new LambdaDelegate<TLambda, Res(ParamsType...), ValType...>(Lambda, std::move(payload)...);
		}
		template<typename TLambda, typename... ValType>
		inline void BindLambda(TLambda Lambda, ValType... payload)
		{
			CreateLambda<TLambda, ValType...>(Lambda, payload...);
		}

		inline bool IsBound() const
		{
			return p != nullptr;
		}

		// p为IDelegate*指针，指向的实际对象为其具体子类，调用抽象类IDelegate的Execute函数，实际上调用的是子类的Execute函数
		inline Res Execute(ParamsType... args)
		{
			assert(("委托类指针为空", p != nullptr));
			return p->Execute(std::move(args)...);
		}
		inline Res ExecuteIfBound(ParamsType... args)
		{
			if (IsBound())
			{
				return Execute(args...);
			}
		}

		// 解绑
		inline void UnBound()
		{
			if (p)
			{
				delete p;
				p = nullptr;
			}
		}
	private:
		IDelegate<Res, ParamsType...>* p;
	};

	// 多播代理，顺序执行，多播代理没有返回值，都是void
	template<typename... ParamsType>
	class MulticastDelegate
	{
	public:
		MulticastDelegate() = default;
		~MulticastDelegate() = default;

		// 使用Delegate的绑定函数进行单个代理的绑定

		// 绑定类的成员函数
		template<class C, typename... ValType>
		inline void AddMember(C* pObject, void(C::*FuncPtr)(ParamsType..., ValType...), ValType... payload)
		{
			Delegate<void, ParamsType...> delegate;
			delegate.BindMember(pObject, FuncPtr, payload...);
			DelegateList.emplace_back(delegate);
		}
		//绑定类的静态函数和全局函数
		template<typename... ValType>
		inline void AddStatic(void(*FuncPtr)(ParamsType..., ValType...), ValType... payload)
		{
			Delegate<void, ParamsType...> delegate;
			delegate.BindStatic(FuncPtr, payload...);
			DelegateList.emplace_back(delegate);
		}

		// 绑定Lambda表达式
		template<class TLambda, typename... ValType>
		inline void AddLambda(TLambda Lambda, ValType... payload)
		{
			Delegate<void, ParamsType...> delegate;
			delegate.BindLambda(Lambda, payload...);
			DelegateList.emplace_back(delegate);
		}

		// 广播，顺序执行
		inline void Broadcast(ParamsType... args)
		{
			for (const auto& myDelegate : DelegateList)
			{
				myDelegate.ExecuteIfBound(args...);
			}
		}

	private:
		std::vector<Delegate<void, ParamsType...>> DelegateList; // 代理数组
	};
}