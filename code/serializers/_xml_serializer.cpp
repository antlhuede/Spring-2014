#pragma once
#include "meta\meta.h"
#include "serializers\serializers.h"
#include "serializers\tinyxml2.h"
#include <fstream>
#include <iostream>

namespace serializers {
void XMLSerializer::construct_object(const xml::XMLElement* element, const meta::type* type, void* obj) const
{
  if (type->isObject)
  {
    //Json::Value json_fields = value["fields"];
    const xml::XMLElement* xmlFields = element->FirstChildElement("fields");

    for (auto field : type->fields())
    {
      const xml::XMLElement* fieldValue = xmlFields->FirstChildElement(field.name.c_str());
      if (fieldValue == nullptr)
        continue;

      construct_object(fieldValue, field.type, field.member_ptr(obj));
    }

    const xml::XMLElement* xmlProperties = element->FirstChildElement("properties");
    for (auto property : type->properties())
    {
      if (property.hasSetter)
      {
        const xml::XMLElement* propertyValue = xmlProperties->FirstChildElement(property.name.c_str());
        
        if (propertyValue == nullptr)
          continue;

        meta::variant propertyData(property.type);
        construct_object(propertyValue, property.type, propertyData.data());
        property.set(type, obj, property.type, propertyData.data());
      }
    }

    const xml::XMLElement* xmlConstants = element->FirstChildElement("constants");
    for (auto constant : type->constants())
    {
      const xml::XMLElement* constantValue = xmlConstants->FirstChildElement(constant.name.c_str());
      if (constantValue == nullptr)
        continue;

      meta::variant& valueRef = const_cast<meta::variant&>(constant.value);
      construct_object(constantValue, constant.type, valueRef.data());
    }
  }
  else
  {
    const meta::type* elementType = meta::typeof(element->Attribute("type"));
    assert(elementType == type);

    if (elementType == meta::typeof<int>() || elementType->isEnum)
    {
      int result = element->IntAttribute("value");
      type->assign(obj, &result);
    }
    else if (elementType == meta::typeof<bool>())
    {
      bool result = element->BoolAttribute("value");
      type->assign(obj, &result);
    }
    else if (elementType == meta::typeof<double>()) //let it fall back to converting to real otherwise
    {
      double result = element->DoubleAttribute("value");
      type->assign(obj, &result);
    }
    else if (elementType == meta::typeof<float>())
    {
      float result = element->FloatAttribute("value");
      type->assign(obj, &result);
    }
    else if (elementType == meta::typeof<string>())
    {
      string result = element->Attribute("value");
      type->assign(obj, &result);
    }
    else
    {
      assert(false);
    }
  }
}
xml::XMLElement* XMLSerializer::construct_xml_element(xml::XMLDocument* doc, const string& name, const meta::type* type, const void* obj) const
{
  xml::XMLElement* node = doc->NewElement(name.c_str());
  node->SetAttribute("type", type->name.c_str());

  auto fields = type->fields();
  auto properties = type->properties();
  auto constants = type->constants();

  if (type->isObject)
  {
    if (fields.size())
    {
      xml::XMLElement* xmlFields = doc->NewElement("fields");
      for (auto field : fields)
        xmlFields->InsertEndChild(construct_xml_element(doc, field.name, field.type, field.member_ptr(obj)));
      node->InsertEndChild(xmlFields);
    }
    if (properties.size())
    {
      xml::XMLElement* xmlProperties = doc->NewElement("properties");
      for (auto property : properties)
      {
        if (property.hasGetter)
        {
          meta::variant propertyData = property.get(obj);
          xmlProperties->InsertEndChild(construct_xml_element(doc, property.name, property.type, propertyData.data()));
        }
      }
      node->InsertEndChild(xmlProperties);
    }
    if (constants.size())
    {
      xml::XMLElement* xmlConstants = doc->NewElement("constants");
      for (auto constant : constants)
      {
        xmlConstants->InsertEndChild(construct_xml_element(doc, constant.name, constant.type, constant.value.data()));
      }
      node->InsertEndChild(xmlConstants);
    }
  }
  else
  {
    if (type == meta::typeof<int>() || type->isEnum)
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
      node->SetAttribute("value", type->toString(obj).c_str());
  }

  return node;
}
shared_ptr<xml::XMLDocument> XMLSerializer::build_xml_document() const
{
  shared_ptr<xml::XMLDocument> doc(new xml::XMLDocument);

  for (auto value : m_names)
  {
    const meta::variant& variant = m_objects[value.second];
    doc->InsertEndChild(construct_xml_element(doc.get(), value.first, variant.type(), variant.data()));
  }

  return doc;
}
void XMLSerializer::write(const string& file) const
{
  std::ofstream stream(file.c_str());
  xml::XMLPrinter printer;
  shared_ptr<xml::XMLDocument> doc = build_xml_document();
  doc->Print(&printer);
  stream << printer.CStr();
}
bool XMLSerializer::read(const string& file)
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
