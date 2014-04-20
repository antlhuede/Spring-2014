#pragma once
#include "meta\meta.h"
#include "serializers\serializers.h"
#include <fstream>

namespace serializers {

void JSonSerializer::initialize(const string& file, state s)
{
  destroy();
  m_fileName = file;
  m_state = s;
  m_root = shared_ptr<Json::Value>(new Json::Value);
  m_tree.push_back(JsonValueHandle(nullptr, m_root.get()));
  m_current = m_root.get();
}
void JSonSerializer::destroy()
{
  m_tree.clear();
  m_root.reset();

  m_fileName = "";
  m_state = e_None;
  m_current = nullptr;
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
  m_tree.push_back(JsonValueHandle(m_current, node));
  m_current = node;
}
void JSonSerializer::EndObject()
{
  m_current = m_tree.back().parent;
  m_tree.pop_back();
}
void JSonSerializer::BeginArray(const string& name)
{

}
void JSonSerializer::EndArray()
{

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




template <class RealType, class JsonType>
inline void AssignJsonValue(const meta::type* type, void* obj, const Json::Value* value_ptr, JsonType(Json::Value::*getter)()const)
{
  //assign checktype the typeof<int> if type is enum, otherwise just assign its current type
  const meta::type* checkType = (type->isEnum == false) ? type : meta::typeof<int>();
  assert(meta::typeof<RealType>() == checkType);
  RealType result = static_cast<RealType>((value_ptr->*getter)());
  type->assign(obj, &result);
}


void JSonSerializer::construct_object(const Json::Value& value, const meta::type* type, void* obj) const
{
  if (type->isObject)
  {
    Json::Value jsonFields = value["fields"];
    for (auto field : type->fields())
    {
      Json::Value& fieldValue = jsonFields[field.name];
      if (fieldValue.isNull())
        continue;

      construct_object(fieldValue, field.type, field.member_ptr(obj));
    }

    Json::Value jsonProperties = value["properties"];
    for (auto property : type->properties())
    {
      if (property.hasSetter)
      {
        Json::Value& propertyValue = jsonProperties[property.name];
        if (propertyValue.isNull())
          continue;

        meta::variant propertyData(property.type);
        construct_object(propertyValue, property.type, propertyData.data());
        property.set(type, obj, property.type, propertyData.data());
      }
    }

    Json::Value jsonConstants = value["constants"];
    for (auto constant : type->constants())
    {
      Json::Value& constantValue = jsonFields[constant.name];
      if (constantValue.isNull())
        continue;

      meta::variant& valueRef = const_cast<meta::variant&>(constant.value);
      construct_object(constantValue, constant.type, valueRef.data());
    }
  }
  else
  {
    if (value.isInt())
      AssignJsonValue<int>(type, obj, &value, &Json::Value::asInt);
    else if (value.isBool())
      AssignJsonValue<bool>(type, obj, &value, &Json::Value::asBool);
    else if (value.isDouble() && type == meta::typeof<double>()) //let it fall back to converting to real otherwise
      AssignJsonValue<double>(type, obj, &value, &Json::Value::asDouble);
    else if (value.isConvertibleTo(Json::ValueType::realValue))
      AssignJsonValue<float>(type, obj, &value, &Json::Value::asDouble);
    else if (value.isString())
    {
      if (type->isEnum == false)
        AssignJsonValue<string>(type, obj, &value, &Json::Value::asString);
      else
      {
        string name = value.asString();
        auto constant = type->getConstant(name);
        type->assign(obj, constant.value.data());
      }
    }
    else
    {
      assert(false);
    }
  }
}

Json::Value JSonSerializer::construct_json_value(const meta::type* type, const void* obj) const
{
  Json::Value value;
  auto fields = type->fields();
  auto properties = type->properties();
  auto constants = type->constants();

  value["type"] = type->name;

  if (type->isObject)
  {
    for (auto field : fields)
      value["fields"][field.name] = construct_json_value(field.type, field.member_ptr(obj));

    for (auto property : properties)
    {
      if (property.hasGetter)
      {
        meta::variant property_data = property.get(obj);
        value["properties"][property.name] = construct_json_value(property.type, property_data.data());
      }
    }

    for (auto constant : constants)
    {
      value["constants"][constant.name] = construct_json_value(constant.type, constant.value.data());
    }
  }
  else
  {
    if (type == meta::typeof<int>())
      value = meta::converter::toInt(type, obj);
    else if (type == meta::typeof<bool>())
      value = meta::converter::toBool(type, obj);
    else if (type == meta::typeof<float>())
      value = meta::converter::toFloat(type, obj);
    else if (type == meta::typeof<double>())
      value = meta::converter::toDouble(type, obj);
    else if (type == meta::typeof<string>())
      value = meta::converter::toString(type, obj);
    else if (type->isEnum)
    {
      auto constant = type->getConstant(meta::converter::toInt(type, obj));
      value = constant.name;
    }
    else
      value = type->toString(obj);
  }
  
  return value;
}
Json::Value JSonSerializer::build_json_tree() const
{
  Json::Value root;
  
  for (auto value : m_names)
  {
    const meta::variant& variant = m_objects[value.second];
    root[value.first] = construct_json_value(variant.type(), variant.data());
  }

  return root;
}
void JSonSerializer::write(const string& file) const
{
  std::ofstream stream(file.c_str());
  Json::StyledStreamWriter writer;
  writer.write(stream, build_json_tree());
}
bool JSonSerializer::read(const string& file)
{
  std::ifstream stream(file.c_str());
  m_objects.clear();

  Json::Value root;
  Json::Reader reader;

  if (reader.parse(stream, root, false) == false)
    return false;
  
  for (auto member : root.getMemberNames())
  {
    Json::Value value = root.get(member, "");
    m_names[member] = m_objects.size();
    m_objects.push_back(meta::variant(meta::typeof(value["type"].asString().c_str())));
    meta::variant& var = m_objects.back();
    construct_object(value, var.type(), var.data());
  }
  return true;
}
}
