#ifndef __SCHEMA_MODULE_H__
#define __SCHEMA_MODULE_H__

class Class;

struct SchemaModule
{
	std::string name;

	std::vector<std::shared_ptr<Class>> classes;
};


#endif // !__SCHEMA_MODULE_H__
