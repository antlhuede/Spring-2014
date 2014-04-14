#pragma once
//#include "meta\meta.h"
//#include "serializers\tinyxml2.h"
//#include <fstream>
//#include <iostream>

namespace serializers {
template <class CP>
void XMLSerializer<CP>::construct_object(const xml::XMLElement* element, const meta::type* type, void* obj) const
{
  if (element->NoChildren() == false)
  {
    //Json::Value json_fields = value["fields"];
    const xml::XMLElement* xml_fields = element->FirstChildElement("fields");

    for (auto field : type->fields())
    {
      construct_object(xml_fields->FirstChildElement(field.name().c_str()), field.type(), field.member_ptr(obj));
    }
  }
  else
  {
#define ASSERT_CORRECT_TYPE(TYPE) (assert(type == meta::typeof<TYPE>()), true)
#define CHECK_ELEMENT_TYPE(TYPE) (meta::typeof(element->Attribute("type")) == meta::typeof<TYPE>() && ASSERT_CORRECT_TYPE(TYPE))

    if (CHECK_ELEMENT_TYPE(int))
    {
      int result = element->IntAttribute("value");
      type->assign(obj, &result);
    }
    else if (CHECK_ELEMENT_TYPE(bool))
    {
      bool result = element->BoolAttribute("value");
      type->assign(obj, &result);
    }
    else if (CHECK_ELEMENT_TYPE(double)) //let it fall back to converting to real otherwise
    {
      double result = element->DoubleAttribute("value");
      type->assign(obj, &result);
    }
    else if (CHECK_ELEMENT_TYPE(float))
    {
      float result = element->FloatAttribute("value");
      type->assign(obj, &result);
    }
    else if (CHECK_ELEMENT_TYPE(string))
    {
      string result = element->Attribute("value");
      type->assign(obj, &result);
    }
    else
    {
      assert(false);
    }
#undef CHECK_ELEMENT_TYPE
#undef ASSERT_CORRECT_TYPE
  }
}
template <class CP>
xml::XMLElement* XMLSerializer<CP>::construct_xml_element(xml::XMLDocument* doc, const string& name, const meta::type* type, const void* obj) const
{
  xml::XMLElement* node = doc->NewElement(name.c_str());
  node->SetAttribute("type", type->name().c_str());

  auto fields = type->fields();
  if (fields.size())
  {
    xml::XMLElement* xml_fields = doc->NewElement("fields");

    for (size_t i = 0; i < fields.size(); ++i)
      xml_fields->InsertEndChild(construct_xml_element(doc, fields[i].name(), fields[i].type(), fields[i].member_ptr(obj))); 

    node->InsertEndChild(xml_fields);
  }
  else
  {
    if (type == meta::typeof<int>())
      node->SetAttribute("value", meta::converter::toInt(type, obj));
    else if (type == meta::typeof<bool>())
      node->SetAttribute("value", meta::converter::toBool(type, obj));
    else if (type == meta::typeof<float>())
      node->SetAttribute("value", meta::converter::toFloat(type, obj));
    else if (type == meta::typeof<double>())
      node->SetAttribute("value", meta::converter::toDouble(type, obj));
    else if (type == meta::typeof<string>())
      node->SetAttribute("value", meta::converter::toString(type, obj).c_str());
    else
      node->SetAttribute("value", type->to_string(obj).c_str());
  }

  return node;
}
template <class CP>
shared_ptr<xml::XMLDocument> XMLSerializer<CP>::build_xml_document() const
{
  shared_ptr<xml::XMLDocument> doc(new xml::XMLDocument);

  for (auto value : m_names)
  {
    const meta::variant& variant = m_objects[value.second];
    doc->InsertEndChild(construct_xml_element(doc.get(), value.first, variant.type(), variant.data()));
  }

  return doc;
}
template <class CP>
void XMLSerializer<CP>::write(const string& file) const
{
  std::ofstream stream(file.c_str());
  xml::XMLPrinter printer;
  shared_ptr<xml::XMLDocument> doc = build_xml_document();
  doc->Print(&printer);
  stream << printer.CStr();
}
template <class CP>
bool XMLSerializer<CP>::read(const string& file)
{
  xml::XMLDocument doc;
  doc.LoadFile(file.c_str());
  xml::XMLElement* root = doc.RootElement();
  while (root)
  {
    m_names[root->Name()] = m_objects.size();
    const meta::type* type = meta::typeof(root->Attribute("type"));
    m_objects.push_back(meta::variant(type));
    meta::variant& var = m_objects.back();
    construct_object(root, var.type(), var.data());

    root = root->NextSiblingElement();
  }
  return true;
}
}
