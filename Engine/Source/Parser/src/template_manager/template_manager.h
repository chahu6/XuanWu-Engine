#ifndef __TEMPLATE_MANAGER_H__
#define __TEMPLATE_MANAGER_H__

class TemplateManager
{
public:
	TemplateManager() = default;
	TemplateManager(const TemplateManager&) = default;
	TemplateManager& operator=(const TemplateManager&) = default;

	void LoadTemplates(std::string path, std::string template_name);

	std::string RenderByTemplate(std::string template_name, Mustache::data& template_data);

	static TemplateManager* GetInstance()
	{
		return m_instance;
	}

private:
	static TemplateManager* m_instance;

	std::unordered_map <std::string, std::string> m_template_pool;
};

#endif // !__TEMPLATE_MANAGER_H__
