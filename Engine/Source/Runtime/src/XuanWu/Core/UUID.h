#pragma once

namespace XuanWu
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};
}

// ʹ��map��key�����Զ������UUID����
namespace std
{
	template<typename T> struct hash;

	template<>
	struct hash<XuanWu::UUID>
	{
		std::size_t operator()(const XuanWu::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};
}
