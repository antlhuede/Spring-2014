#pragma once

#include "json\json\value.h"

namespace meta {

class JSonSerializer : public serializer
{
public:
  JSonSerializer() {}
  ~JSonSerializer() {}

  virtual bool add(const string& name, const meta::type& t, void* obj);
  virtual void write(ostream& stream);
  virtual bool read(istream& stream);
  virtual bool has(const string& var);
  virtual bool get(const string& var, const meta::type& t, void* obj);
  virtual void clear();

  template <class T>
  bool add(const string& name, const T& obj);
  template <class T>
  bool get(const string& name, T& obj);

  void set(const hash_map<string, Json::Value>& variables);

  const Json::Value& get_root() const { return m_root; }
private:
  void recursive_read(const Json::Value& node, const meta::type& type, void* obj);
  Json::Value recursive_add(const meta::type& t, void* obj);

  Json::Value m_root;
  hash_map<string, Json::Value> m_loadedVariables;
};

template <class T>
bool JSonSerializer::add(const string& name, const T& obj)
{
  return add(name, meta::typeof(obj), const_cast<T*>(&obj));
}
template <class T>
bool JSonSerializer::get(const string& name, T& obj)
{
  return get(name, meta::typeof(obj), &obj);
}
}
