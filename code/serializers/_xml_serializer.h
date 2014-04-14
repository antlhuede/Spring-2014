#pragma once

#include "serializers\tinyxml2.h"

namespace serializers {
namespace xml = ::tinyxml2;

template <class ContainerPolicy>
class XMLSerializer : public serializer<ContainerPolicy>
{
public:
  XMLSerializer() = default;
  ~XMLSerializer() = default;

  virtual void write(const string& file) const;
  virtual bool read(const string& file);

private:
  shared_ptr<xml::XMLDocument> build_xml_document() const;
  xml::XMLElement* construct_xml_element(xml::XMLDocument* doc, const string& name, const meta::type* type, const void* obj) const;
  void construct_object(const xml::XMLElement* element, const meta::type* type, void* obj) const;
};
}

namespace xml {
typedef serializers::XMLSerializer<meta::object_container> serializer;
typedef serializer* serializer_ptr;
}