#pragma once
//#include "meta\meta.h"
//#include "serializers\json\json.h"
//#include "serializers\serializers.h"
//#include <fstream>

namespace serializers {
template <class CP>
void JSonSerializer<CP>::construct_object(const Json::Value& value, const meta::type* type, void* obj) const
{
  if (value.isObject())
  {
    Json::Value json_fields = value["fields"];

    for (auto field : type->fields())
    {
      construct_object(json_fields[field.name()], field.type(), field.member_ptr(obj));
    }
  }
  else
  {
#define ASSERT_CORRECT_TYPE(TYPE) (assert(type == meta::typeof<TYPE>()), true)
    if (value.isInt() && ASSERT_CORRECT_TYPE(int))
    {
      int result = value.asInt();
      type->assign(obj, &result);
    }
    else if (value.isBool() && ASSERT_CORRECT_TYPE(bool))
    {
      bool result = value.asBool();
      type->assign(obj, &result);
    }
    else if (value.isDouble() && type == meta::typeof<double>()) //let it fall back to converting to real otherwise
    {
      double result = value.asDouble();
      type->assign(obj, &result);
    }
    else if (value.isConvertibleTo(Json::ValueType::realValue) && ASSERT_CORRECT_TYPE(float))
    {
      float result = static_cast<float>(value.asDouble());
      type->assign(obj, &result);
    }
    else if (value.isString() && ASSERT_CORRECT_TYPE(string))
    {
      string result = value.asString();
      type->assign(obj, &result);
    }
    else
    {
      assert(false);
    }
#undef ASSERT_CORRECT_TYPE
  }
}
template <class CP>
Json::Value JSonSerializer<CP>::construct_json_value(const meta::type* type, const void* obj) const
{
  Json::Value value;
  const vector<const meta::field>& fields = type->fields();

  value["type"] = type->name();

  if (fields.size())
  {
    for (size_t i = 0; i < fields.size(); ++i)
      value["fields"][fields[i].name()] = construct_json_value(fields[i].type(), fields[i].member_ptr(obj));
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
