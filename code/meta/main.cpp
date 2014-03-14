#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <sstream>
#include <stack>
#include <memory>

#include "json\json.h"

template <class T> using vector = std::vector<T>;
template <class T> using shared_ptr = std::shared_ptr<T>;
template <class T, class U> using hash_map = std::unordered_map<T, U>;
typedef std::string string;
typedef std::istream istream;
typedef std::ostream ostream;
typedef std::ostringstream ostringstream;
typedef std::istringstream istringstream;

#include "meta\meta.h"

unsigned meta::type::S_ID_COUNTER = 0;
hash_map<string, const meta::type*> meta::meta_registry::m_registry;

DECLARE_META_TYPE(int);
DECLARE_META_TYPE(bool);
DECLARE_META_TYPE(float);
DECLARE_META_TYPE(double);
DECLARE_META_TYPE(string);

class test_class { public: test_class() = default; test_class(int ti, string ts) : test_int(ti), test_string(ts) {} ~test_class() = default; int test_int; string test_string; };

DECLARE_META_OBJECT(meta::nulltype);
DECLARE_META_OBJECT(test_class);

class serializer
{
public:
  virtual ~serializer() = 0 {};
  
  virtual void add(const string& name, const meta::type& t, void* obj) = 0;
  virtual void write(ostream& stream) = 0;
  virtual bool read(istream& stream) = 0;
  virtual bool get(const string& var, const meta::type& t, void* obj) = 0;
};

class JSonSerializer : public serializer
{
public:
  JSonSerializer() {}
  ~JSonSerializer() {}

  template <class T>
  void add(const string& name, const T& obj)
  {
    add(name, meta::typeof(obj), const_cast<T*>(&obj));
  }
  template <class T>
  bool get(const string& name, T& obj)
  {
    return get(name, meta::typeof(obj), &obj);
  }
  virtual void add(const string& name, const meta::type& t, void* obj)
  {
    m_root[name] = recursive_add(t, obj);
  }
  virtual void write(ostream& stream)
  {
    Json::StyledStreamWriter writer;
    writer.write(stream, m_root);
  }
  virtual bool read(istream& stream)
  {
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

  virtual bool get(const string& var, const meta::type& t, void* obj)
  {
    auto it = m_loadedVariables.find(var);
    if (it == m_loadedVariables.end())
      return false;
    
    recursive_read(it->second, t, obj);
    return true;
  }

private:
  void recursive_read(const Json::Value& node, const meta::type& type, void* obj)
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
    }
  }

  Json::Value recursive_add(const meta::type& t, void* obj)
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
  
  Json::Value m_root;
  hash_map<string, Json::Value> m_loadedVariables;
};

int main(void)
{
  int tttt = 5;
  int tttresult = meta::converter::toInt(meta::typeof(tttt), &tttt);
  
  float ffff = 12.5f;
  float fffresult = meta::converter::toFloat(meta::typeof(ffff), &ffff);

  meta::type& t = const_cast<meta::type&>(meta::meta_holder<test_class>::t);
  t.Field("test_string", &test_class::test_string);
  t.Field("test_int", &test_class::test_int);
  const vector<const meta::field*>& fields = t.fields();
  test_class tc(5, "test123");
  std::cout << t.to_string(&tc);
  

  std::ofstream writefile;
  writefile.open("test.json");
  JSonSerializer serializer;
  serializer.add("test_class1", tc);
  serializer.add("test_class2", tc);
  serializer.write(writefile);
  writefile.close();

  std::ifstream readfile;
  readfile.open("test.json");
  serializer.read(readfile);
  readfile.close();
  test_class read_test;
  serializer.get("test_class1", read_test);

  Json::Value root;
  
  for (size_t i = 0; i < fields.size(); ++i)
  {
    Json::Value value;
    value[fields[i]->name()] = fields[i]->to_string(&tc);
    root[t.to_string(&tc)].append(value);
    //std::cout << fields[i]->type().name() << " " << fields[i]->name() << " " << fields[i]->offset() << std::endl;
  }
  Json::StyledWriter writer;
  string output = writer.write(root);
  std::cout << output;


  float testint = 12.5;
  string teststr = "\nteststr";
  meta::type test = meta::typeof(testint);
  test.write(std::cout, &testint);
  test = meta::typeof(teststr);
  test.read(std::cin, &teststr);
  test.write(std::cout, &teststr);

  std::cout << test.name() << " " << test.size();
  return 0;
}