#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <sstream>
#include <stack>

#include "json\json.h"

template <class T> using vector = std::vector<T>;
typedef std::string string;

#include "meta\meta.h"

unsigned meta::type::S_ID_COUNTER = 0;

DECLARE_META_TYPE(int);
DECLARE_META_TYPE(bool);
DECLARE_META_TYPE(float);
DECLARE_META_TYPE(double);
DECLARE_META_TYPE(string);

class test_class { public: test_class(int ti, string ts) : test_int(ti), test_string(ts) {} int test_int; string test_string; };

DECLARE_META_OBJECT(test_class);

class serializer
{
public:
  virtual ~serializer() = 0 {};
  virtual void add(const meta::type& t, void* obj) = 0;
  virtual void write(std::ostream& stream) = 0;
  virtual bool read(std::istream& stream) = 0;
  virtual bool get(const string& var, void* obj) = 0;
};

class JSonSerializer : public serializer
{
public:
  JSonSerializer() {}
  ~JSonSerializer() {}

  template <class T>
  void add(const T& obj)
  {
    add(meta::typeof(obj), const_cast<T*>(&obj));
  }
  virtual void add(const meta::type& t, void* obj)
  {
    m_root.append(recursive_add(t, obj));
  }
  virtual void write(std::ostream& stream)
  {
    Json::StyledStreamWriter writer;
    writer.write(stream, m_root);
  }
  virtual bool read(std::istream& stream)
  {
    Json::Reader reader;
    if (reader.parse(stream, m_root, false) == false)
      return false;

    recursive_read(m_root);
    return true;
  }
  virtual bool get(const string& var, void* obj)
  {
    
    return false;
  }

private:
  void recursive_read(Json::Value node)
  {
    Json::Value::Members members = node.getMemberNames();
    
    for (size_t i = 0; i < node.size(); ++i)
    {
      m_loadedVariables[members[i]] = node[i];
      recursive_read(node[i]);
    }
  }
  Json::Value recursive_add(const meta::type& t, void* obj)
  {
    const vector<const meta::field*>& fields = t.fields();
    Json::Value value;
    
    if (fields.size())
    {
      for (size_t i = 0; i < fields.size(); ++i)
        value[t.name()][fields[i]->name()] = recursive_add(fields[i]->type(), fields[i]->member_ptr(obj));
    }
    else
    {
      if (t.id() == meta::typeof<int>().id())
        value[t.name()] = meta::converter::toInt(t, obj);
      else if (t.id() == meta::typeof<bool>().id())
        value[t.name()] = meta::converter::toBool(t, obj);
      else if (t.id() == meta::typeof<float>().id())
        value[t.name()] = meta::converter::toFloat(t, obj);
      else if (t.id() == meta::typeof<double>().id())
        value[t.name()] = meta::converter::toDouble(t, obj);
      else if (t.id() == meta::typeof<string>().id())
        value[t.name()] = meta::converter::toString(t, obj);
      else
        value[t.name()] = t.to_string(obj);
    }

    return value;
  }
  
  Json::Value m_root;
  std::unordered_map<string, Json::Value> m_loadedVariables;
};

int main(void)
{
  int tttt = 5;
  int tttresult = meta::converter::toInt(meta::typeof(tttt), &tttt);
  
  float ffff = 12.5f;
  float fffresult = meta::converter::toFloat(meta::typeof(ffff), &ffff);

  meta::type& t = meta::meta_holder<test_class>::t;
  t.Field("test_string", &test_class::test_string);
  t.Field("test_int", &test_class::test_int);
  const vector<const meta::field*>& fields = t.fields();
  test_class tc(5, "test123");
  std::cout << t.to_string(&tc);
  
  std::ofstream writefile;
  writefile.open("test.json");
  JSonSerializer serializer;
  serializer.add(tc);
  serializer.add(tc);
  serializer.write(writefile);
  writefile.close();

  std::ifstream readfile;
  readfile.open("test.json");
  serializer.read(readfile);
  readfile.close();

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