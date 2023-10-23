#include "xwpch.h"
#include "template_manager.h"

TemplateManager* TemplateManager::m_instance = new TemplateManager;

void TemplateManager::LoadTemplates(std::string path, std::string template_name)
{
	m_template_pool.insert_or_assign(template_name, Utils::LoadFile(path + TEMPLATE_PATH + template_name + ".mustache"));
}

std::string TemplateManager::RenderByTemplate(std::string template_name, Mustache::data& template_data)
{
	if (m_template_pool.end() == m_template_pool.find(template_name))
	{
		return "";
	}
	Mustache::mustache tmpl(m_template_pool[template_name]);
	return tmpl.render(template_data);
}
