#include "xwpch.h"
#include "meta_info.h"

#include "Cursor/cursor.h"

MetaInfo::MetaInfo(const Cursor& cursor)
{
	for (auto& child : cursor.GetChildren())
	{
		if (child.GetKind() != CXCursor_AnnotateAttr)
			continue;

		for (auto& prop : ExtractProperties(child))
		{
			m_properties[prop.first] = prop.second;
		}
	}
}

bool MetaInfo::GetFlag(const std::string_view key) const
{
	return m_properties.find(key.data()) != m_properties.end();
}

std::string MetaInfo::GetProperty(const std::string_view key) const
{
	auto search = m_properties.find(key.data());

	return search == m_properties.end() ? "" : search->second;
}

std::vector<MetaInfo::Property> MetaInfo::ExtractProperties(const Cursor& cursor) const
{
	std::vector<Property> ret_list;

	auto propertyList = cursor.GetDisplayName();

	auto&& properties = Utils::Split(propertyList, ",");

	static const std::string white_space_string = "\t\r\n";

	for (auto& property_item : properties)
	{
		auto&& item_details = Utils::Split(property_item, ":");
		auto&& temp_string = Utils::Trim(item_details[0], white_space_string);
		if (temp_string.empty())
		{
			continue;
		}

		ret_list.emplace_back(temp_string, item_details.size() > 1 ? Utils::Trim(item_details[1], white_space_string) : "");
		return ret_list;
	}
}
