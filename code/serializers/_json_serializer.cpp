#pragma once
#include "meta\meta.h"
#include "serializers\serializers.h"
#include <fstream>

namespace serializers {

void JSonSerializer::write(const string& file) const {}
bool JSonSerializer::read(const string& file) { return false; }

void JSonSerializer::initialize(const string& file, state s)
{
  destroy();
  m_fileName = file;
  m_state = s;
  m_root = shared_ptr<Json::Value>(new Json::Value);
  m_parents.push_back(nullptr);
  m_current = m_root.get();
}
void JSonSerializer::destroy()
{
  m_current = nullptr;
  m_parents.clear();
  m_root.reset();

  m_fileName = "";
  m_state = e_None;
}
bool JSonSerializer::ReadFile(const string& file)
{
  std::ifstream stream(file.c_str());
  if (stream.is_open() == false)
    return false;

  initialize(file, e_Read);
  Json::Reader reader;

  if (reader.parse(stream, *m_root, false) == false)
  {
    destroy();
    return false;
  }

  return true;
}
bool JSonSerializer::BeginWrite(const string& file)
{
  std::ofstream stream(file.c_str());
  if (stream.is_open() == false)
    return false;

  initialize(file, e_Write);
  return true;
}
void JSonSerializer::EndWrite()
{
  std::ofstream stream(m_fileName.c_str());
  assert(stream.is_open());
  
  Json::StyledStreamWriter writer;
  writer.write(stream, *m_root);

  destroy();
}
void JSonSerializer::BeginObject(const string& name, const meta::type* type)
{
  assert(m_state != e_None);

  Json::Value* node = &(*m_current)[name];
  assert(node);

  if (m_state == e_Write)
  {
    (*node)["type"] = type->name;
  }
  else if (m_state == e_Read)
  {
    const meta::type* storedType = meta::typeof((*node)["type"].asString().c_str());
    assert(type == storedType);
  }
  //m_tree.push_back(JsonValueHandle(m_current, node));
  m_parents.push_back(m_current);
  m_current = node;
}
void JSonSerializer::EndObject()
{
  m_current = m_parents.back();
  m_parents.pop_back();
}
void JSonSerializer::BeginArray(const string& name)
{
  assert(false);
}
void JSonSerializer::EndArray()
{
  assert(false);
}
template <class T>
T ExtractValue(Json::Value* node, const string& name, T(Json::Value::*getter)()const, bool(Json::Value::*checker)()const = nullptr)
{
  Json::Value* value = &(*node)[name];
  if (value->isNull())
    return T();

  if (checker)
    assert((value->*checker)());

  return (value->*getter)();
}
bool JSonSerializer::ReadBool(const string& name) const
{
  return ExtractValue(m_current, name, &Json::Value::asBool, &Json::Value::isBool);
}
int JSonSerializer::ReadInteger(const string& name) const
{
  return ExtractValue(m_current, name, &Json::Value::asInt, &Json::Value::isInt);
}
float JSonSerializer::ReadFloat(const string& name) const
{
  assert((*m_current)[name].isConvertibleTo(Json::ValueType::realValue));
  return static_cast<float>(ExtractValue(m_current, name, &Json::Value::asDouble));
}
double JSonSerializer::ReadDouble(const string& name) const
{
  return ExtractValue(m_current, name, &Json::Value::asDouble, &Json::Value::isDouble);
}
const string JSonSerializer::ReadString(const string& name) const
{
  return ExtractValue(m_current, name, &Json::Value::asString, &Json::Value::isString);
}
const string JSonSerializer::ReadEnum(const string& name) const
{
  return ExtractValue(m_current, name, &Json::Value::asString);
}
const meta::variant JSonSerializer::ReadVariable(const string& name) const
{
  assert(false);
  return meta::variant();
}
void JSonSerializer::WriteBool(const string& name, bool value)
{
  (*m_current)[name] = value;
}
void JSonSerializer::WriteInteger(const string& name, int value)
{
  (*m_current)[name] = value;
}
void JSonSerializer::WriteFloat(const string& name, float value)
{
  (*m_current)[name] = value;
}
void JSonSerializer::WriteDouble(const string& name, double value)
{
  (*m_current)[name] = value;
}
void JSonSerializer::WriteString(const string& name, const string& value)
{
  (*m_current)[name] = value;
}
void JSonSerializer::WriteEnum(const string& name, const meta::type*, const string& value)
{
  (*m_current)[name] = value;
}
}
