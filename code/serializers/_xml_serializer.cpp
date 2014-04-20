#pragma once
#include "meta\meta.h"
#include "serializers\serializers.h"
#include "serializers\tinyxml2.h"
#include <fstream>
#include <iostream>

namespace serializers {

void XMLSerializer::write(const string& file) const {}
bool XMLSerializer::read(const string& file) { return false; }

bool XMLSerializer::ReadFile(const string& file)
{
  m_doc = shared_ptr<xml::XMLDocument>(new xml::XMLDocument);
  xml::XMLError result = m_doc->LoadFile(file.c_str());
  
  if (result != xml::XML_SUCCESS)
    return false;

  m_fileName = file;
  m_current = m_doc.get();
  m_state = e_Read;
  return true;
}
bool XMLSerializer::BeginWrite(const string& file)                                 
{
  if (m_state != e_None)
  {
    m_doc.reset();
    m_state = e_None;
  }

  std::ofstream stream(file.c_str());
  if (stream.is_open() == false)
    return false;

  m_fileName = file;
  m_doc = shared_ptr<xml::XMLDocument>(new xml::XMLDocument);
  m_current = m_doc.get();

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
void XMLSerializer::BeginObject(const string& name, const meta::type* type)
{
  assert(m_state != e_None);

  if (m_state == e_Write)
  {
    xml::XMLElement* element = m_doc->NewElement(name.c_str());
    element->SetAttribute("type", type->name.c_str());

    m_current->InsertEndChild(element);
    m_current = element;
  }
  else if (m_state == e_Read)
  {
    xml::XMLElement* object = m_current->FirstChildElement(name.c_str());
    assert(object);
    const meta::type* storedType = meta::typeof(object->Attribute("type"));
    assert(type == storedType);
    m_current = object;
  }
}
void XMLSerializer::EndObject()
{
  assert(m_state != e_None);
  assert(m_current);
  assert(m_current != m_doc.get()); //ensure we dont go too far up the tree
  m_current = m_current->Parent();
}
void XMLSerializer::BeginArray(const string& name)
{
  assert(false);
}
void XMLSerializer::EndArray()
{
  assert(false);
}

template <class T>
T ExtractNodeValue(xml::XMLNode* node, const string& name, T(xml::XMLElement::*getter)(const char*)const)
{
  xml::XMLElement* value = node->FirstChildElement(name.c_str());
  if (node == nullptr)
    return T();

  assert(meta::typeof(value->Attribute("type")) == meta::typeof<T>());
  return (value->*getter)("value");
}
bool XMLSerializer::ReadBool(const string& name) const                             
{
  return ExtractNodeValue(m_current, name, &xml::XMLElement::BoolAttribute);
}
int XMLSerializer::ReadInteger(const string& name) const                           
{
  return ExtractNodeValue(m_current, name, &xml::XMLElement::IntAttribute);
}
float XMLSerializer::ReadFloat(const string& name) const                           
{
  return ExtractNodeValue(m_current, name, &xml::XMLElement::FloatAttribute);
}
double XMLSerializer::ReadDouble(const string& name) const                         
{
  return ExtractNodeValue(m_current, name, &xml::XMLElement::DoubleAttribute);
}
const string XMLSerializer::ReadString(const string& name) const                   
{
  //attribute function has different signature to others
  //must duplicate code
  xml::XMLElement* node = m_current->FirstChildElement(name.c_str());
  if (node == nullptr)
    return string();

  const meta::type* type = meta::typeof(node->Attribute("type"));
  assert(type == meta::typeof<string>());
  return node->Attribute("value");
}
const string XMLSerializer::ReadEnum(const string& name) const
{
  xml::XMLElement* node = m_current->FirstChildElement(name.c_str());
  if (node == nullptr)
    return string();

  const meta::type* type = meta::typeof(node->Attribute("type"));
  assert(type->isEnum);
  return node->Attribute("value");
}
const meta::variant XMLSerializer::ReadVariable(const string& name) const          
{
  xml::XMLElement* node = m_current->FirstChildElement(name.c_str());

  if (node == nullptr)
    return meta::variant();

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
void XMLSerializer::WriteEnum(const string& name, const meta::type* type, const string& value)
{
  //strings are handled differently to other types
  xml::XMLElement* element = m_doc->NewElement(name.c_str());

  element->SetAttribute("type", type->name.c_str());
  element->SetAttribute("value", value.c_str());
  m_current->InsertEndChild(element);
}
}
