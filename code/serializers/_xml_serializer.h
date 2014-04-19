#pragma once

#include "serializers\tinyxml2.h"

namespace serializers {
namespace xml = ::tinyxml2;

class XMLSerializer : public serializer
{
public:
  XMLSerializer() = default;
  ~XMLSerializer() = default;
  XMLSerializer(XMLSerializer&&) = delete;
  XMLSerializer(const XMLSerializer&) = delete;
  XMLSerializer& operator=(const XMLSerializer&) = delete;

  virtual void write(const string& file) const;
  virtual bool read(const string& file);

private:
  shared_ptr<xml::XMLDocument> build_xml_document() const;
  xml::XMLElement* construct_xml_element(xml::XMLDocument* doc, const string& name, const meta::type* type, const void* obj) const;
  void construct_object(const xml::XMLElement* element, const meta::type* type, void* obj) const;
};
}

namespace xml {
typedef serializers::XMLSerializer serializer;
typedef serializer* serializer_ptr;
}