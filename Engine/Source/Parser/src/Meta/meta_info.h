#ifndef __META_INFO_H__
#define __META_INFO_H__

class Cursor;

class MetaInfo
{
	typedef std::pair<std::string, std::string> Property;
public:
	MetaInfo(const Cursor& cursor);

	bool GetFlag(const std::string_view key) const;

	std::string GetProperty(const std::string_view key) const;
private:
	std::vector<Property> ExtractProperties(const Cursor& cursor) const;

private:
	std::unordered_map<std::string, std::string> m_properties;
};
#endif // !__META_INFO_H__