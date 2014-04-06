#include "meta\meta.h"
#include "xml\tinyxml2.h"
#include <fstream>
#include <iostream>

namespace meta
{
using namespace xml;
XmlSerializer::XmlSerializer()
{
  if (m_names.size() > 0)
  {

  }

  if (m_objects.size() > 0)
  {

  }
}
void XmlSerializer::construct_object(const XMLElement* element, const type& type, void* obj) const
{
  if (element->NoChildren() == false)
  {
    //Json::Value json_fields = value["fields"];
    const XMLElement* xml_fields = element->FirstChildElement("fields");

    for (auto field : type.fields())
    {
      construct_object(xml_fields->FirstChildElement(field.name().c_str()), field.type(), field.member_ptr(obj));
    }
  }
  else
  {
#define ASSERT_CORRECT_TYPE(TYPE) (assert(type.id() == meta::typeof<TYPE>().id()), true)
#define CHECK_ELEMENT_TYPE(TYPE) (typeof(element->Attribute("type")) == typeof<TYPE>() && ASSERT_CORRECT_TYPE(TYPE))

    if (CHECK_ELEMENT_TYPE(int))
    {
      int result = element->IntAttribute("value");
      type.assign(obj, &result);
    }
    else if (CHECK_ELEMENT_TYPE(bool))
    {
      bool result = element->BoolAttribute("value");
      type.assign(obj, &result);
    }
    else if (CHECK_ELEMENT_TYPE(double)) //let it fall back to converting to real otherwise
    {
      double result = element->DoubleAttribute("value");
      type.assign(obj, &result);
    }
    else if (CHECK_ELEMENT_TYPE(float))
    {
      float result = element->FloatAttribute("value");
      type.assign(obj, &result);
    }
    else if (CHECK_ELEMENT_TYPE(string))
    {
      string result = element->Attribute("value");
      type.assign(obj, &result);
    }
    else
    {
      assert(false);
    }
#undef CHECK_ELEMENT_TYPE
#undef ASSERT_CORRECT_TYPE
  }
}
XMLElement* XmlSerializer::construct_xml_element(XMLDocument* doc, const string& name, const type& type, const void* obj) const
{
  XMLElement* node = doc->NewElement(name.c_str());
  node->SetAttribute("type", type.name().c_str());

  auto fields = type.fields();
  if (fields.size())
  {
    xml::XMLElement* xml_fields = doc->NewElement("fields");

    for (size_t i = 0; i < fields.size(); ++i)
      xml_fields->InsertEndChild(construct_xml_element(doc, fields[i].name(), fields[i].type(), fields[i].member_ptr(obj))); 

    node->InsertEndChild(xml_fields);
  }
  else
  {
    if (type.id() == meta::typeof<int>().id())
      node->SetAttribute("value", meta::converter::toInt(type, obj));
    else if (type.id() == meta::typeof<bool>().id())
      node->SetAttribute("value", meta::converter::toBool(type, obj));
    else if (type.id() == meta::typeof<float>().id())
      node->SetAttribute("value", meta::converter::toFloat(type, obj));
    else if (type.id() == meta::typeof<double>().id())
      node->SetAttribute("value", meta::converter::toDouble(type, obj));
    else if (type.id() == meta::typeof<string>().id())
      node->SetAttribute("value", meta::converter::toString(type, obj).c_str());
    else
      node->SetAttribute("value", type.to_string(obj).c_str());
  }

  return node;
}
shared_ptr<XMLDocument> XmlSerializer::build_xml_document() const
{
  shared_ptr<XMLDocument> doc(new XMLDocument);

  for (auto value : m_names)
  {
    const variant& variant = m_objects[value.second];
    doc->InsertEndChild(construct_xml_element(doc.get(), value.first, variant.type(), variant.data()));
  }

  return doc;
}
void XmlSerializer::write(const string& file) const
{
  std::ofstream stream(file.c_str());
  XMLPrinter printer;
  shared_ptr<XMLDocument> doc = build_xml_document();
  doc->Print(&printer);
  stream << printer.CStr();
}
bool XmlSerializer::read(const string& file)
{
  XMLDocument doc;
  doc.LoadFile(file.c_str());
  XMLElement* root = doc.RootElement();
  while (root)
  {
    m_names[root->Name()] = m_objects.size();
    const type& type = typeof(root->Attribute("type"));
    std::cout << "xml serializing member: " << root->Name() << std::endl;
    std::cout << "xml reading type: " << type.name() << std::endl;
    m_objects.push_back(variant(type));
    variant& var = m_objects.back();
    construct_object(root, var.type(), var.data());

    root = root->NextSiblingElement();
  }
  return true;
}
}
