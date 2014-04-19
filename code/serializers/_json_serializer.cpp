#pragma once
#include "meta\meta.h"
#include "serializers\serializers.h"
#include <fstream>

namespace serializers {

bool JSonSerializer::ReadFile(const string& file)
{
  return false;
}
bool JSonSerializer::BeginWrite(const string& file)
{
  return false;
}
void JSonSerializer::EndWrite()
{

}
bool JSonSerializer::ReadBool(const string& name) const
{
  return false;
}
int JSonSerializer::ReadInteger(const string& name) const
{
  return 0;
}
float JSonSerializer::ReadFloat(const string& name) const
{
  return 0.0f;
}
double JSonSerializer::ReadDouble(const string& name) const
{
  return 0.0;
}
const string JSonSerializer::ReadString(const string& name) const
{
  return string();
}
const meta::variant JSonSerializer::ReadVariable(const string& name) const
{
  return meta::variant();
}
void JSonSerializer::WriteBool(const string& name, bool value)
{

}
void JSonSerializer::WriteInteger(const string& name, int value)
{

}
void JSonSerializer::WriteFloat(const string& name, float value)
{

}
void JSonSerializer::WriteDouble(const string& name, double value)
{

}
void JSonSerializer::WriteString(const string& name, const string& value)
{

}
void JSonSerializer::BeginObject(const string& name, const meta::type* type)
{

}
void JSonSerializer::EndObject()
{

}
void JSonSerializer::BeginArray(const string& name)
{

}
void JSonSerializer::EndArray()
{

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
