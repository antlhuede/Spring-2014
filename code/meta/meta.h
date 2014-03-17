#pragma once

#include "meta\_type_definitions.h"

#include "meta\_type.h"
#include "meta\_field.h"
#include "meta\_registry.h"
#include "meta\_converter.h"
#include "meta\_operations.h"
#include "meta\_serializer.h"

#include "meta\_type_inl.h"
#include "meta\_registry_inl.h"
#include "meta\_converter_inl.h"
#include "meta\_operations_inl.h"


#define DECLARE_META_TYPE(TYPE) \
  const meta::type meta::meta_holder<TYPE>::t(meta::decl<TYPE>(), #TYPE, &meta::read_basic_types<TYPE>, &meta::write_basic_types<TYPE>, &meta::to_string_basic_types<TYPE>);

#define DECLARE_META_OBJECT(OBJECT) \
  const meta::type meta::meta_holder<OBJECT>::t(meta::decl<OBJECT>(), #OBJECT, &meta::read_objects<OBJECT>, &meta::write_objects<OBJECT>, &meta::to_string_objects<OBJECT>);
