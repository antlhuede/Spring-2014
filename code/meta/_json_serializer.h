#pragma once

#include "json\json\value.h"

namespace meta {
class JSonSerializer : public serializer
{
public:
  JSonSerializer();
  ~JSonSerializer() {}

  virtual void write(const string& file) const;
  virtual bool read(const string& file);

private:
  Json::Value build_json_tree() const;
  Json::Value construct_json_value(const type& type, const void* obj) const;
  void construct_object(const Json::Value& value, const type& type, void* obj) const;
};
}