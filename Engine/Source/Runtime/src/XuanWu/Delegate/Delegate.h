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

	// �󶨲�ͬ����ί�еľ���ʵ��

	/**
	* �����Ա������ί��
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
		MethodPtr Callback;	// ����ָ��
		C* m_p;
		std::tuple<ValType...> m_payload;
	};


	/**
	* ��ȫ�ֺ������ྲ̬����
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
	* ��Lambda���ʽ
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

	// ����ʹ�õ�ί����
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

		// ��������ָ����ø��ຯ������ָ��ָ���������������ຯ��
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

		// ����ľ�̬������ȫ�ֺ���
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

		// ��Lambda���ʽ������Lambda���ʽ�ļ�飬�Ƕ��ǿ������е�
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

		// pΪIDelegate*ָ�룬ָ���ʵ�ʶ���Ϊ��������࣬���ó�����IDelegate��Execute������ʵ���ϵ��õ��������Execute����
		inline Res Execute(ParamsType... args)
		{
			assert(("ί����ָ��Ϊ��", p != nullptr));
			return p->Execute(std::move(args)...);
		}
		inline Res ExecuteIfBound(ParamsType... args)
		{
			if (IsBound())
			{
				return Execute(args...);
			}
		}

		// ���
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

	// �ಥ����˳��ִ�У��ಥ����û�з���ֵ������void
	template<typename... ParamsType>
	class MulticastDelegate
	{
	public:
		MulticastDelegate() = default;
		~MulticastDelegate() = default;

		// ʹ��Delegate�İ󶨺������е�������İ�

		// ����ĳ�Ա����
		template<class C, typename... ValType>
		inline void AddMember(C* pObject, void(C::*FuncPtr)(ParamsType..., ValType...), ValType... payload)
		{
			Delegate<void, ParamsType...> delegate;
			delegate.BindMember(pObject, FuncPtr, payload...);
			DelegateList.emplace_back(delegate);
		}
		//����ľ�̬������ȫ�ֺ���
		template<typename... ValType>
		inline void AddStatic(void(*FuncPtr)(ParamsType..., ValType...), ValType... payload)
		{
			Delegate<void, ParamsType...> delegate;
			delegate.BindStatic(FuncPtr, payload...);
			DelegateList.emplace_back(delegate);
		}

		// ��Lambda���ʽ
		template<class TLambda, typename... ValType>
		inline void AddLambda(TLambda Lambda, ValType... payload)
		{
			Delegate<void, ParamsType...> delegate;
			delegate.BindLambda(Lambda, payload...);
			DelegateList.emplace_back(delegate);
		}

		// �㲥��˳��ִ��
		inline void Broadcast(ParamsType... args)
		{
			for (const auto& myDelegate : DelegateList)
			{
				myDelegate.ExecuteIfBound(args...);
			}
		}

	private:
		std::vector<Delegate<void, ParamsType...>> DelegateList; // ��������
	};
}