#include "MetaType.h"

DefineSingleton(MetaManager);

DefineExternal(int){metaType->TypeName = "int"; metaType->TypeSize = sizeof(int);};
DefineExternal(float){metaType->TypeName = "float"; metaType->TypeSize = sizeof(float);};

template<typename type, typename setType, setType type::* Member>
void TypeGetter(Instance* instance, Variant& var)
{
	type* typeInstance = (type*)(instance);
	var = typeInstance->*Member;
}

template<typename type, typename setType, setType type::* Member>
void TypeSetter(Instance* instance, Variant& var)
{
	type* typeInstance = (type*)(instance);
	typeInstance->*Member = var.As<setType>();
}

#define BindMemberProperty(type, name) 									 \
		{auto prop = new Property();							     \
		prop->Name = #name;											 \
		prop->mGet = TypeGetter<SelfType, type, &SelfType::name>;	 \
		prop->mSet = TypeSetter<SelfType, type, &SelfType::name>; \
		SafeInsert(metaType->Properties, prop->Name, prop); }  \



class Animal
{
public:

	DeclareMeta(Animal);


	int Lives;
	int Guns;

	Animal()
		:Lives(9)
	{
	}
};

DefineMeta(Animal)
{			
	metaType->TypeName = "Animal";
	BindMemberProperty(int, Lives);
	BindMemberProperty(int, Guns);


/*		auto prop = new Property();
		prop->Name = "Lives";
		prop->mGet = TypeGetter<Animal, int, &Animal::Lives>;
		prop->mSet = TypeSetter<Animal, int, &Animal::Lives>*/
}

void TakeVariant(Variant var)
{
	if( var.Is<int>() )
		printf("Is an int");
}

int main()
{
	MetaManager* manager = MetaManager::GetInstance();

	MetaType* intType = GetMetaTypeOf<int>();

	MetaType* animalType = GetMetaTypeOf<Animal>();

	Animal* animal = new Animal();

	Property* prop = animalType->Properties["Lives"];

	prop->SetValue(animal, 5);


	//Variant v( 5 );

	TakeVariant( 5 );

	

}