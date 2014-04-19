#pragma once
#include "meta\meta.h"
#include "serializers\serializers.h"
#include "serializers\tinyxml2.h"
#include <fstream>
#include <iostream>

namespace serializers {

bool XMLSerializer::ReadFile(const string& file)
{
  m_doc = shared_ptr<xml::XMLDocument>(new xml::XMLDocument);
  xml::XMLError result = m_doc->LoadFile(file.c_str());
  
  if (result != xml::XML_SUCCESS)
    return false;

  m_fileName = file;
  m_current = m_doc->RootElement();
  m_state = e_Read;
  return true;
}
bool XMLSerializer::BeginWrite(const string& file)                                 
{
  std::ofstream stream(file.c_str());
  if (stream.is_open() == false)
    return false;
  stream.close();

  m_fileName = file;
  m_doc = shared_ptr<xml::XMLDocument>(new xml::XMLDocument);
  m_current = nullptr;

  m_state = e_Write;
  return true;
}
void XMLSerializer::EndWrite()                                                     
{
  std::ofstream stream(m_fileName.c_str());
  assert(stream.is_open());
  xml::XMLPrinter printer;
  m_doc->Print(&printer);
  stream << printer.CStr();
  stream.close();

  m_fileName = "";
  m_current = nullptr;
  m_doc.reset();
  m_state = e_None;
}
bool XMLSerializer::ReadBool(const string& name) const                             
{
  xml::XMLElement* node = m_current->FirstChildElement(name.c_str());
  assert(meta::typeof(node->Attribute("type")) == meta::typeof<bool>());
  return node->BoolAttribute("value");
}
int XMLSerializer::ReadInteger(const string& name) const                           
{
  xml::XMLElement* node = m_current->FirstChildElement(name.c_str());
  assert(meta::typeof(node->Attribute("type")) == meta::typeof<int>());
  return node->IntAttribute("value");
}
float XMLSerializer::ReadFloat(const string& name) const                           
{
  xml::XMLElement* node = m_current->FirstChildElement(name.c_str());
  assert(meta::typeof(node->Attribute("type")) == meta::typeof<float>());
  return node->FloatAttribute("value");
}
double XMLSerializer::ReadDouble(const string& name) const                         
{
  xml::XMLElement* node = m_current->FirstChildElement(name.c_str());
  assert(meta::typeof(node->Attribute("type")) == meta::typeof<double>());
  return node->DoubleAttribute("value");
}
const string XMLSerializer::ReadString(const string& name) const                   
{
  xml::XMLElement* node = m_current->FirstChildElement(name.c_str());
  assert(node);
  assert(meta::typeof(node->Attribute("type")) == meta::typeof<string>());
  return node->Attribute("value");
}
const meta::variant XMLSerializer::ReadVariable(const string& name) const          
{
  xml::XMLElement* node = m_current->FirstChildElement(name.c_str());
  meta::variant ret;

  if (meta::typeof(node->Attribute("type")) == meta::typeof<bool>())
    ret = node->BoolAttribute("value");
  else if (meta::typeof(node->Attribute("type")) == meta::typeof<int>())
    ret = node->IntAttribute("value");
  else if (meta::typeof(node->Attribute("type")) == meta::typeof<float>())
    ret = node->FloatAttribute("value");
  else if (meta::typeof(node->Attribute("type")) == meta::typeof<double>())
    ret = node->DoubleAttribute("value");
  else if (meta::typeof(node->Attribute("type")) == meta::typeof<string>())
    ret = node->Attribute("value");
  return ret;
}
template <class T>
void AddNewElement(xml::XMLDocument* doc, xml::XMLNode* parent, const string& name, T value)
{
  xml::XMLElement* element = doc->NewElement(name.c_str());

  element->SetAttribute("type", meta::typeof<T>()->name.c_str());
  element->SetAttribute("value", value);
  parent->InsertEndChild(element);
}

void XMLSerializer::WriteBool(const string& name, bool value)                      
{
  AddNewElement(m_doc.get(), m_current, name, value);
}
void XMLSerializer::WriteInteger(const string& name, int value)                    
{
  AddNewElement(m_doc.get(), m_current, name, value);
}
void XMLSerializer::WriteFloat(const string& name, float value)                    
{
  AddNewElement(m_doc.get(), m_current, name, value);
}
void XMLSerializer::WriteDouble(const string& name, double value)                  
{
  AddNewElement(m_doc.get(), m_current, name, value);
}
void XMLSerializer::WriteString(const string& name, const string& value)           
{
  //strings are handled differently to other types
  xml::XMLElement* element = m_doc->NewElement(name.c_str());

  element->SetAttribute("type", meta::typeof<string>()->name.c_str());
  element->SetAttribute("value", value.c_str());
  m_current->InsertEndChild(element);
}
void XMLSerializer::BeginObject(const string& name, const meta::type* type)        
{
  assert(m_state != e_None);
  if (m_state == e_Write)
  {
    xml::XMLElement* element = m_doc->NewElement(name.c_str());
    element->SetAttribute("type", type->name.c_str());

    if (m_current == nullptr)
      m_doc->InsertEndChild(element);
    else
      m_current->InsertEndChild(element);

    m_current = element;
  }
  else if (m_state == e_Read)
  {
    
  }
}
void XMLSerializer::EndObject()                                                    
{
  assert(m_state != e_None);
  if (m_state == e_Write)
  {
    assert(m_current);
    m_current = m_current->Parent();
    if (m_current == m_doc.get())
      m_current = nullptr;
  }
  else if (m_state == e_Read)
  {

  }
}
void XMLSerializer::BeginArray(const string& name)                                 
{
  assert(false);
}
void XMLSerializer::EndArray()                                                     
{
  assert(false);
}

const meta::variant XMLSerializer::ReadEnum(const string& name) const
{
  return meta::variant();
}
void XMLSerializer::WriteEnum(const string& name, const meta::variant_ref& value)
{

}



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
    assert(type == elementType);

    if (type == meta::typeof<int>())
    {
      int result = element->IntAttribute("value");
      type->assign(obj, &result);
    }
    else if (type == meta::typeof<bool>())
    {
      bool result = element->BoolAttribute("value");
      type->assign(obj, &result);
    }
    else if (type == meta::typeof<double>())
    {
      double result = element->DoubleAttribute("value");
      type->assign(obj, &result);
    }
    else if (type == meta::typeof<float>())
    {
      float result = element->FloatAttribute("value");
      type->assign(obj, &result);
    }
    else if (type == meta::typeof<string>())
    {
      string result = element->Attribute("value");

      type->assign(obj, &result);
    }
    else if (type->isEnum)
    {
      string name = element->Attribute("value");
      auto constant = type->getConstant(name);
      type->assign(obj, constant.value.data());
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
    else if (type->isEnum)
    {
      auto constant = type->getConstant(meta::converter::toInt(type, obj));
      node->SetAttribute("value", constant.name.c_str());
    }
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
