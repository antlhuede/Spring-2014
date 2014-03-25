#include "meta\meta.h"
#include "json\json.h"
#include <fstream>

namespace meta
{
void JSonSerializer::construct_object(const Json::Value& value, const type& type, void* obj) const
{
  if (value.isObject())
  {
    Json::Value json_fields = value["fields"];

    for (auto field : type.fields())
    {
      construct_object(json_fields[field->name()], field->type(), field->member_ptr(obj));
    }
  }
  else
  {
#define ASSERT_CORRECT_TYPE(TYPE) (assert(type.id() == meta::typeof<TYPE>().id()), true)
    if (value.isInt() && ASSERT_CORRECT_TYPE(int))
    {
      int result = value.asInt();
      type.copy(obj, &result);
    }
    else if (value.isBool() && ASSERT_CORRECT_TYPE(bool))
    {
      bool result = value.asBool();
      type.copy(obj, &result);
    }
    else if (value.isDouble() && type == meta::typeof<double>()) //let it fall back to converting to real otherwise
    {
      double result = value.asDouble();
      type.copy(obj, &result);
    }
    else if (value.isConvertibleTo(Json::ValueType::realValue) && ASSERT_CORRECT_TYPE(float))
    {
      float result = static_cast<float>(value.asDouble());
      type.copy(obj, &result);
    }
    else if (value.isString() && ASSERT_CORRECT_TYPE(string))
    {
      string result = value.asString();
      type.copy(obj, &result);
    }
    else
    {
      assert(false);
    }
#undef ASSERT_CORRECT_TYPE
  }
}
Json::Value JSonSerializer::construct_json_value(const type& type, const void* obj) const
{
  Json::Value value;
  auto fields = type.fields();

  value["type"] = type.name();

  if (fields.size())
  {
    for (size_t i = 0; i < fields.size(); ++i)
      value["fields"][fields[i]->name()] = construct_json_value(fields[i]->type(), fields[i]->member_ptr(obj));
  }
  else
  {
    if (type.id() == meta::typeof<int>().id())
      value = meta::converter::toInt(type, obj);
    else if (type.id() == meta::typeof<bool>().id())
      value = meta::converter::toBool(type, obj);
    else if (type.id() == meta::typeof<float>().id())
      value = meta::converter::toFloat(type, obj);
    else if (type.id() == meta::typeof<double>().id())
      value = meta::converter::toDouble(type, obj);
    else if (type.id() == meta::typeof<string>().id())
      value = meta::converter::toString(type, obj);
    else
      value = type.to_string(obj);
  }

  return value;
}
Json::Value JSonSerializer::build_json_tree() const
{
  Json::Value root;
  
  auto obj = objects();
  for (auto value : lexicon())
  {
    const variant& variant = obj[value.second];
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
  clear();

  std::ifstream stream(file.c_str());

  Json::Value root;
  Json::Reader reader;

  if (reader.parse(stream, root, false) == false)
    return false;
  
  for (auto member : root.getMemberNames())
  {
    Json::Value value = root.get(member, "");
    //variant object;//(typeof(value["type"].asString().c_str()));
    type type = typeof(value["type"].asString().c_str());
    void* memory = type.make_new();
    construct_object(value, type, memory);
    add(member, variant(type, memory, true));
    //type.delete_ptr(memory);
  }
  return true;
}
}
