#ifndef __CURSOR_H__
#define __CURSOR_H__

class CursorType;

class Cursor
{
	typedef CXCursorVisitor Visitor;
	typedef std::vector<Cursor> List;
public:
	Cursor(const CXCursor& handle);

	CXCursorKind GetKind() const;

	std::string GetSpelling() const;
	std::string GetDisplayName() const; // 获取全名称，Test()

	std::string GetSourceFile() const;

	bool IsDefinition() const;

	CursorType GetType() const;

	List GetChildren() const;
	void VisitChildren(Visitor visitor, void* data = nullptr);

private:
	CXCursor m_handle;
};

#endif // !__CURSOR_H__
