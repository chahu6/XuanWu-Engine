#ifndef __GENERATOR_H__
#define __GENERATOR_H__

class Class;
class SchemaModule;

namespace Generator
{
	class GeneratorInterface
	{
	public:
		GeneratorInterface(std::string out_path, std::string root_path, std::function<std::string(std::string)> get_include_func);
		virtual ~GeneratorInterface() = default;

		virtual int Generate(std::string path, SchemaModule schema) = 0;

		virtual void Finish() {}

	protected:
		virtual void PrepareStatus(std::string path);
		virtual void GenClassRenderData(std::shared_ptr<Class> class_temp, Mustache::data& class_def);
		virtual void GenClassFieldRenderData(std::shared_ptr<Class> class_temp, Mustache::data& field_defs);

	protected:
		std::string m_out_path{ "gen_src" };
		std::string m_root_path;
		std::function<std::string(std::string)> m_get_include_func;
	};
}

#endif // !__GENERATOR_H__
