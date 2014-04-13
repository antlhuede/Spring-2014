#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#define Map unordered_map 

#define DeclareSingleton(type)    \
	static type* Instance; \
	static type* GetInstance(){if(Instance == NULL) Instance = new type(); return Instance;}

#define DefineSingleton(type) \
	type* type::Instance = nullptr;

class MetaType;

class Variant
{
public:
	MetaType* Type;

	template<typename type>
	Variant(const type& value)
	{
		Type = GetMetaTypeOf<type>();
		memcpy(data, &value, sizeof(value));
	}

	template<typename type>
	bool Is()
	{
		return Type == GetMetaTypeOf<type>();
	}

	template<typename type>
	type& As()
	{
		return *(type*)data;
	}

	char data[16];
};





class MetaManager
{
public:

	DeclareSingleton(MetaManager);

	Map<string, MetaType*> Types;
};


template<typename type>
void SafeInsert(Map<string, type*>& map, string key, type* value)
{
	auto it = map.find(key);
	if(it == map.end() )
	{
		map[key] = value;
	}
	else
	{
		throw exception("NO STAHP");
	}



}

typedef void Instance;

typedef void (*PropertyFunc)(Instance* instance, Variant& var);
 

class Property
{
public:
	string Name;

	void GetValue(Instance* instance, Variant& var)
	{
		mGet(instance, var);
	}

	void SetValue(Instance* instance, Variant var)
	{
		mSet(instance, var);
	}

	PropertyFunc mGet;
	PropertyFunc mSet;

};

class Method
{
public:
	string Name;
};

class MetaType
{
public:
	MetaType()
		:Setup(false)
	{
	}

	string TypeName;
	int TypeSize;
	bool Setup;

	void Register()
	{
		MetaManager* manager = MetaManager::GetInstance();
		SafeInsert(manager->Types, this->TypeName, this);
		Setup = true;
	}

	Map<string, Property*> Properties;
	Map<string, Method*> Methods;
};




#define DeclareMeta(type) \
	MetaType* GetType(); \
	typedef type SelfType; \
	static void SetupMeta(MetaType* metaType);

#define DefineMeta(type) \
	void type::SetupMeta(MetaType* metaType)

template<typename type>
void Setup(MetaType* metaType) {type::SetupMeta(metaType);};


#define DefineExternal(type)        \
	template<>                       \
	void Setup<type>(MetaType* metaType)

template<typename type>
MetaType* GetMetaTypeOf()
{
	static MetaType metaType;
	if(!metaType.Setup)
	{
		Setup<type>(&metaType);
		metaType.Register();
	}

	return &metaType;
}




