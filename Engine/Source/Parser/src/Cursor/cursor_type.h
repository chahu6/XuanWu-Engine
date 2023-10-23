#pragma once

class CursorType
{
public:
	CursorType(const CXType& handle);

	std::string GetDisplayName() const;

	bool IsConst() const;

private:
	CXType m_handle;
};