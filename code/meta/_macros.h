#pragma once

#define EXX2(a, b)  a##b
#define EXX(a, b) EXX2(a, b)

#define DECLARE_META_TYPE(class) \
  const ::meta::type* ::meta::internal::meta_holder<class>::s_pType = 0; namespace { \
  void EXX(CreateMeta, __LINE__)() { ::meta::internal::meta_holder<class>::s_pType = new ::meta::type(::meta::decl<class>(), #class, &::meta::read_basic_types<class>, &::meta::write_basic_types<class>, &::meta::to_string_basic_types<class>); } \
  void EXX(DeleteMeta, __LINE__)() { delete ::meta::internal::meta_holder<class>::s_pType; ::meta::internal::meta_holder<class>::s_pType = 0; }  \
  ::meta::internal::type_initializer EXX(g_typeInitializer, __LINE__)(&EXX(CreateMeta, __LINE__), &EXX(DeleteMeta, __LINE__), 0, 0); }

#define DECLARE_META_OBJECT(class) \
  const ::meta::type* ::meta::internal::meta_holder<class>::s_pType = 0; namespace { \
  void EXX(CreateMeta, __LINE__)() { ::meta::internal::meta_holder<class>::s_pType = new ::meta::type(::meta::decl<class>(), #class, &::meta::read_objects<class>, &::meta::write_objects<class>, &::meta::to_string_objects<class>); } \
  void EXX(DeleteMeta, __LINE__)() { delete ::meta::internal::meta_holder<class>::s_pType; ::meta::internal::meta_holder<class>::s_pType = 0; } \
  void EXX(ShutDownMeta, __LINE__)() { const_cast<::meta::type*>(::meta::internal::meta_holder<class>::s_pType)->destroy(); } \
  void EXX(InitializeMeta, __LINE__)(); \
  ::meta::internal::type_initializer EXX(g_typeInitializer, __LINE__)(&EXX(CreateMeta, __LINE__), &EXX(DeleteMeta, __LINE__), &EXX(InitializeMeta, __LINE__),  &EXX(ShutDownMeta, __LINE__)); \
  void EXX(InitializeMeta, __LINE__)() {  \
  const_cast<::meta::type&>(*::meta::typeof<class>())

#define END_META_OBJECT() ;}}
