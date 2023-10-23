#include "xwpch.h"
#include "cursor.h"

#include "cursor_type.h"

Cursor::Cursor(const CXCursor& handle)
	:m_handle(handle)
{
}

CXCursorKind Cursor::GetKind() const
{
	return m_handle.kind;
}

std::string Cursor::GetSpelling() const
{
	std::string spelling;
	Utils::ToString(clang_getCursorSpelling(m_handle), spelling);
	return spelling;
}

// 获取全名称，Test()
std::string Cursor::GetDisplayName() const
{
	std::string display_name;

	Utils::ToString(clang_getCursorDisplayName(m_handle), display_name);

	return display_name;
}

std::string Cursor::GetSourceFile() const
{
	auto range = clang_Cursor_getSpellingNameRange(m_handle, 0, 0);

	auto start = clang_getRangeStart(range);

	CXFile file;
	unsigned line, column, offset;
	clang_getFileLocation(start, &file, &line, &column, &offset);

	std::string filename;

	Utils::ToString(clang_getFileName(file), filename);

	return filename;
}

bool Cursor::IsDefinition() const
{
	return clang_isCursorDefinition(m_handle);
}

CursorType Cursor::GetType() const
{
	return clang_getCursorType(m_handle);
}

Cursor::List Cursor::GetChildren() const
{
	List children;

	auto visitor = [](CXCursor cursor, CXCursor parent, CXClientData data)
	{
		auto container = static_cast<List*>(data);

		container->emplace_back(cursor);
		if (cursor.kind == CXCursor_LastPreprocessing)
		{
			return CXChildVisit_Break;
		}

		return CXChildVisit_Continue;
	};

	clang_visitChildren(m_handle, visitor, &children);

	return children;
}

void Cursor::VisitChildren(Visitor visitor, void* data)
{
}
