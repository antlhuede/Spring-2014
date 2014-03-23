#pragma once
#include "xml\tinyxml2.h"

namespace meta {
  namespace xml = ::tinyxml2;

  class XmlSerializer : public serializer
  {
  public:
    XmlSerializer() {}
    ~XmlSerializer() {}

    virtual bool add(const string& name, const meta::type& t, const void* obj);
    virtual void write(ostream& stream);
    virtual bool read(istream& stream);
    virtual bool has(const string& var);
    virtual bool get(const string& var, const meta::type& t, void* obj);
    virtual void clear();

    template <class T>
    bool add(const string& name, const T& obj);
    template <class T>
    bool get(const string& name, T& obj);

    void set(const hash_map<string, xml::XMLNode*>& variables);

    const xml::XMLDocument& get_doc() const { return m_doc; }
  private:
    void recursive_read(const xml::XMLNode* node, const meta::type& type, void* obj);
    xml::XMLElement* recursive_add(xml::XMLElement* node, const meta::type& t, const void* obj);

    xml::XMLDocument m_doc;
    hash_map<string, xml::XMLNode*> m_loadedVariables;
  };

  template <class T>
  bool XmlSerializer::add(const string& name, const T& obj)
  {
    return add(name, meta::typeof(obj), const_cast<T*>(&obj));
  }
  template <class T>
  bool XmlSerializer::get(const string& name, T& obj)
  {
    return get(name, meta::typeof(obj), &obj);
  }
}
