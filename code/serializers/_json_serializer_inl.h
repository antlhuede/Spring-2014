#pragma once
//#include "meta\meta.h"
//#include "serializers\json\json.h"
//#include "serializers\serializers.h"
//#include <fstream>

namespace serializers {

template <class RealType, class JsonType>
inline void assign_json_value(const meta::type* type, void* obj, const Json::Value* value_ptr, JsonType(Json::Value::*getter)()const)
{
  assert(type == meta::typeof<RealType>());
  RealType result = static_cast<RealType>((value_ptr->*getter)());
  type->assign(obj, &result);
}
template <class CP>
void JSonSerializer<CP>::construct_object(const Json::Value& value, const meta::type* type, void* obj) const
{
  if (type->isObject())
  {
    Json::Value json_fields = value["fields"];
    Json::Value json_properties = value["properties"];

    for (auto field : type->fields())
    {
      construct_object(json_fields[field.name()], field.type(), field.member_ptr(obj));
    }

    for (auto property : type->properties())
    {
      if (property.has_setter())
      {
        const meta::type* property_type = property.type();

        Json::Value property_value(json_properties[property.name()]);
        if (property_value.isNull())
          return;

        if (property_value.isInt() && property_type == meta::typeof<int>())
          property.set(obj, property_value.asInt());
        else if (property_value.isBool() && property_type == meta::typeof<bool>())
          property.set(obj, property_value.asBool());
        else if (property_value.isDouble() && property_type == meta::typeof<double>())
          property.set(obj, property_value.asDouble());
        else if (property_value.isString() && property_type == meta::typeof<string>())
          property.set(obj, property_value.asString());
        else if (property_value.isConvertibleTo(Json::ValueType::realValue) && property_type == meta::typeof<float>())
          property.set(obj, static_cast<float>(property_value.asDouble()));
        else
        {
          assert(false);
        }
      }
    }
  }
  else
  {
    if (value.isInt())
      assign_json_value<int>(type, obj, &value, &Json::Value::asInt);
    else if (value.isBool())
      assign_json_value<bool>(type, obj, &value, &Json::Value::asBool);
    else if (value.isDouble() && type == meta::typeof<double>()) //let it fall back to converting to real otherwise
      assign_json_value<double>(type, obj, &value, &Json::Value::asDouble);
    else if (value.isConvertibleTo(Json::ValueType::realValue))
      assign_json_value<float>(type, obj, &value, &Json::Value::asDouble);
    else if (value.isString())
      assign_json_value<string>(type, obj, &value, &Json::Value::asString);
    else
    {
      assert(false);
    }
  }
}

template <class CP>
Json::Value JSonSerializer<CP>::construct_json_value(const meta::type* type, const void* obj) const
{
  Json::Value value;
  const vector<const meta::field>& fields = type->fields();
  const vector<const meta::property>& properties = type->properties();
  value["type"] = type->name();

  if (type->isObject())
  {
    for (size_t i = 0; i < fields.size(); ++i)
      value["fields"][fields[i].name()] = construct_json_value(fields[i].type(), fields[i].member_ptr(obj));

    for (size_t i = 0; i < properties.size(); ++i)
    {
      if (properties[i].has_getter())
      {
        meta::variant property_data = properties[i].get(obj);
        value["properties"][properties[i].name()] = construct_json_value(properties[i].type(), property_data.data());
      }
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
    else
      value = type->to_string(obj);
  }

  return value;
}
template <class CP>
Json::Value JSonSerializer<CP>::build_json_tree() const
{
  Json::Value root;
  
  for (auto value : m_names)
  {
    const meta::variant& variant = m_objects[value.second];
    root[value.first] = construct_json_value(variant.type(), variant.data());
  }

  return root;
}
template <class CP>
void JSonSerializer<CP>::write(const string& file) const
{
  std::ofstream stream(file.c_str());
  Json::StyledStreamWriter writer;
  writer.write(stream, build_json_tree());
}
template <class CP>
bool JSonSerializer<CP>::read(const string& file)
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
