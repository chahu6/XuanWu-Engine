#ifndef __REFLECTION_GENERATOR_H__
#define __REFLECTION_GENERATOR_H__

#include "generator.h"

namespace Generator
{
	class ReflectionGenerator : public GeneratorInterface
	{
	public:
		ReflectionGenerator(std::string source_directory, std::function<std::string(std::string)> get_include_function);
		
		virtual int Generate(std::string path, SchemaModule schema) override;
		virtual void Finish() override;
	protected:
		virtual void PrepareStatus(std::string path) override;

	private:
		std::string ProcessFileName(std::string path);

	private:
		std::vector<std::string> m_head_file_list;
		std::vector<std::string> m_type_list;
	};
}
#endif // !__REFLECTION_GENERATOR_H__
