#pragma once
#include "xml\tinyxml2.h"

namespace meta {
namespace xml = ::tinyxml2;

class XmlSerializer : public serializer
{
public:
  XmlSerializer();
  ~XmlSerializer() {}

  virtual void write(const string& file) const;
  virtual bool read(const string& file);

private:
  shared_ptr<xml::XMLDocument> build_xml_document() const;
  xml::XMLElement* construct_xml_element(xml::XMLDocument* doc, const string& name, const type* type, const void* obj) const;
  void construct_object(const xml::XMLElement* element, const type* type, void* obj) const;
};
}
