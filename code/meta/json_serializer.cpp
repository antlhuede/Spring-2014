#include "meta\meta.h"
#include "json\json.h"

namespace meta
{

void JSonSerializer::set(const hash_map<string, Json::Value>& variables)
{
  m_loadedVariables = variables;
}
bool JSonSerializer::add(const string& name, const meta::type& t, void* obj)
{
  if(m_loadedVariables.find(name) != m_loadedVariables.end())
    return false;

  Json::Value value = recursive_add(t, obj);
  m_root[name] = value;
  m_loadedVariables[name] = value;
  return true;
}
void JSonSerializer::write(ostream& stream)
{
  Json::StyledStreamWriter writer;
  writer.write(stream, m_root);
}
bool JSonSerializer::read(istream& stream)
{
  clear();

  Json::Reader reader;
  if (reader.parse(stream, m_root, false) == false)
    return false;

  Json::Value::Members members = m_root.getMemberNames();
  for (auto it = members.begin(); it != members.end(); ++it)
  {
    m_loadedVariables[*it] = m_root.get(*it, "");
  }
  return true;
}
bool JSonSerializer::has(const string& var)
{
  return (m_loadedVariables.find(var) != m_loadedVariables.end());
}
bool JSonSerializer::get(const string& var, const meta::type& t, void* obj)
{
  auto it = m_loadedVariables.find(var);
  if (it == m_loadedVariables.end())
    return false;

  recursive_read(it->second, t, obj);
  return true;
}
void JSonSerializer::clear()
{
  m_root = Json::nullValue;
  m_loadedVariables.clear();
}

void JSonSerializer::recursive_read(const Json::Value& node, const meta::type& type, void* obj)
{
  if (node.isObject())
  {
    Json::Value fields = node["fields"];
    for (auto it = fields.begin(); it != fields.end(); ++it)
    {
      const meta::field& field = type.field(it.memberName(), true);
      recursive_read(*it, field.type(), field.member_ptr(obj));
    }
  }
  else
  {
#define ASSERT_CORRECT_TYPE(TYPE) (assert(type.id() == meta::typeof<TYPE>().id()), true)

    if (node.isInt() && ASSERT_CORRECT_TYPE(int))
    {
      int result = node.asInt();
      type.copy(obj, &result);
    }
    else if (node.isBool() && ASSERT_CORRECT_TYPE(bool))
    {
      bool result = node.asBool();
      type.copy(obj, &result);
    }
    else if (node.isDouble() && type == meta::typeof<double>()) //let it fall back to converting to real otherwise
    {
      double result = node.asDouble();
      type.copy(obj, &result);
    }
    else if (node.isConvertibleTo(Json::ValueType::realValue) && ASSERT_CORRECT_TYPE(float))
    {
      float result = static_cast<float>(node.asDouble());
      type.copy(obj, &result);
    }
    else if (node.isString() && ASSERT_CORRECT_TYPE(string))
    {
      string result = node.asString();
      type.copy(obj, &result);
    }
    else
    {
      assert(false);
    }
#undef ASSERT_CORRECT_TYPE
  }
}

Json::Value JSonSerializer::recursive_add(const meta::type& t, void* obj)
{
  const vector<const meta::field*>& fields = t.fields();
  Json::Value value;

  if (fields.size())
  {
    value["type"] = t.name();
    for (size_t i = 0; i < fields.size(); ++i)
      value["fields"][fields[i]->name()] = recursive_add(fields[i]->type(), fields[i]->member_ptr(obj));
  }
  else
  {
    if (t.id() == meta::typeof<int>().id())
      value = meta::converter::toInt(t, obj);
    else if (t.id() == meta::typeof<bool>().id())
      value = meta::converter::toBool(t, obj);
    else if (t.id() == meta::typeof<float>().id())
      value = meta::converter::toFloat(t, obj);
    else if (t.id() == meta::typeof<double>().id())
      value = meta::converter::toDouble(t, obj);
    else if (t.id() == meta::typeof<string>().id())
      value = meta::converter::toString(t, obj);
    else
      value = t.to_string(obj);
  }

  return value;
}

}