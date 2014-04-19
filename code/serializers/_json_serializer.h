#pragma once

#include "serializers\json\json.h"
#include "serializers\json\value.h"

namespace serializers {
class JSonSerializer : public serializer<meta::object_container>
{
public:
  typedef serializer<meta::object_container> base_serializer;

  JSonSerializer() = default;
  ~JSonSerializer() = default;
  JSonSerializer(JSonSerializer&&) = delete;
  JSonSerializer(const JSonSerializer&) = delete;
  JSonSerializer& operator=(const JSonSerializer&) = delete;

  virtual void write(const string& file) const;
  virtual bool read(const string& file);

private:
  Json::Value build_json_tree() const;
  Json::Value construct_json_value(const meta::type* type, const void* obj) const;
  void construct_object(const Json::Value& value, const meta::type* type, void* obj) const;
};
}

namespace json {
typedef serializers::JSonSerializer serializer;
typedef serializer* serializer_ptr;
}