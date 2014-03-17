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

#include "meta\meta.h"
#include "gmock\gmock.h"

unsigned meta::type::S_ID_COUNTER = 0;
hash_map<string, const meta::type*> meta::meta_registry::m_registry;

DECLARE_META_TYPE(int);
DECLARE_META_TYPE(bool);
DECLARE_META_TYPE(float);
DECLARE_META_TYPE(double);
DECLARE_META_TYPE(string);

DECLARE_META_OBJECT(nulltype);

class test_class
{
public:
  test_class() = default;
  ~test_class() = default;

  test_class(int ti, string ts, float tf, double td, bool tb)
    : test_int(ti)
    , test_string(ts)
    , test_float(tf)
    , test_double(td)
    , test_bool(tb) {}

  bool operator==(const test_class& rhs)
  {
    return (test_int == rhs.test_int &&
           test_bool == rhs.test_bool &&
           test_float == rhs.test_float &&
           test_double == rhs.test_double &&
           test_string == rhs.test_string);
  }
  int test_int = 0;
  string test_string = "";
  float test_float = 0.0f;
  double test_double = 0.0;
  bool test_bool = false;
};

int main(int argc, char** argv)
{
  meta::type& type = const_cast<meta::type&>(meta::typeof<test_class>());
  type.Field("test_string", &test_class::test_string);
  type.Field("test_int", &test_class::test_int);
  type.Field("test_float", &test_class::test_float);
  type.Field("test_double", &test_class::test_double);
  type.Field("test_bool", &test_class::test_bool);


  ::testing::InitGoogleMock(&argc, argv);
  int ret_val = RUN_ALL_TESTS();

  std::cout << "\n\n";


  std::ofstream writefile;
  std::ifstream readfile;
  meta::JSonSerializer serializer;

  test_class tc(5, "test123", 13.25f, 0.0928, true);
  test_class tc2(12, "123 boogaloo", 103.484f, 10293.92820, false);

  serializer.add("test_class1", tc);
  serializer.add("test_class2", tc2);

  writefile.open("test.json");
  serializer.write(writefile);
  writefile.close();

  readfile.open("test.json");
  serializer.read(readfile);
  readfile.close();

  test_class read_test1;
  test_class read_test2;
  serializer.get("test_class1", read_test1);
  serializer.get("test_class2", read_test2);
  
  serializer.clear();
  serializer.add("read_test1", read_test1);
  serializer.add("read_test2", read_test2);

  writefile.open("test2.json");
  serializer.write(writefile);
  writefile.close();

  Json::Value root;
  const vector<const meta::field*>& fields = type.fields();
  
  for (size_t i = 0; i < fields.size(); ++i)
  {
    Json::Value value;
    value[fields[i]->name()] = fields[i]->to_string(&tc);
    root[type.to_string(&tc)].append(value);
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
  //test.read(std::cin, &teststr);
  //test.write(std::cout, &teststr);

  std::cout << test.name() << " " << test.size() << "\n\n";

  return ret_val;
}

TEST(SampleTest, TestThatItWorked)
{
  EXPECT_TRUE(true);
  EXPECT_FALSE(false);

}

DECLARE_META_OBJECT(test_class);

int test_int1 = 5;
int test_int2 = 12;
float test_float1 = 13.5f;
float test_float2 = 332.5f;
string test_string1 = "test123";
string test_string2 = "123 boogaloo";
double test_double1 = 0.0928;
double test_double2 = 10293.9382;
bool test_bool1 = true;
bool test_bool2 = false;

test_class test_class1(test_int1, test_string1, test_float1, test_double1, test_bool1);
test_class test_class2(test_int2, test_string2, test_float2, test_double2, test_bool2);

TEST(Meta_CORE, typeof)
{
#define TEST_TYPEOF(TYPE, NUM_FIELDS) \
  const meta::type TYPE##_ = meta::typeof<TYPE>();        \
  EXPECT_TRUE(TYPE##_.name() == #TYPE);                   \
  EXPECT_TRUE(TYPE##_.size() == sizeof(TYPE));            \
  EXPECT_TRUE(TYPE##_.fields().size() == NUM_FIELDS)
#define TEST_TYPEOF_BASIC_TYPES(TYPE) TEST_TYPEOF(TYPE, 0)

  TEST_TYPEOF_BASIC_TYPES(nulltype);
  TEST_TYPEOF_BASIC_TYPES(int);
  TEST_TYPEOF_BASIC_TYPES(bool);
  TEST_TYPEOF_BASIC_TYPES(float);
  TEST_TYPEOF_BASIC_TYPES(double);
  TEST_TYPEOF_BASIC_TYPES(string);

  TEST_TYPEOF(test_class, 5);

#undef TEST_TYPEOF_BASIC_TYPES
#undef TEST_TYPEOF
}

hash_map<string, Json::Value> BuildVariableMap()
{
  hash_map<string, Json::Value> variables;

  variables["test_bool1"] = test_bool1;
  variables["test_bool2"] = test_bool2;
  variables["test_double1"] = test_double1;
  variables["test_double2"] = test_double2;
  variables["test_float1"] = test_float1;
  variables["test_float2"] = test_float2;
  variables["test_int1"] = test_int1;
  variables["test_int2"] = test_int2;
  variables["test_string1"] = test_string1;
  variables["test_string2"] = test_string2;

#define BUILD_TEST_CLASS_AS_JSON_VALUE(SUFFIX)  \
  Json::Value json_test_class##SUFFIX;                                      \
  json_test_class##SUFFIX["fields"]["test_bool"] = test_bool##SUFFIX;       \
  json_test_class##SUFFIX["fields"]["test_double"] = test_double##SUFFIX;   \
  json_test_class##SUFFIX["fields"]["test_float"] = test_float##SUFFIX;     \
  json_test_class##SUFFIX["fields"]["test_int"] = test_int##SUFFIX;         \
  json_test_class##SUFFIX["fields"]["test_string"] = test_string##SUFFIX;   \
  json_test_class##SUFFIX["type"] = "test_class";

  BUILD_TEST_CLASS_AS_JSON_VALUE(1)
  BUILD_TEST_CLASS_AS_JSON_VALUE(2)
#undef BUILD_TEST_CLASS_AS_JSON_VALUE

  variables["test_class1"] = json_test_class1;
  variables["test_class2"] = json_test_class2;

  return variables;
}
TEST(Meta_JsonSerializer, has)
{
  meta::JSonSerializer serializer;
  serializer.set(BuildVariableMap());
  
  EXPECT_FALSE(serializer.has(""));
  EXPECT_TRUE(serializer.has("test_bool1"));
  EXPECT_TRUE(serializer.has("test_bool2"));
  EXPECT_TRUE(serializer.has("test_double1"));
  EXPECT_TRUE(serializer.has("test_double2"));
  EXPECT_TRUE(serializer.has("test_float1"));
  EXPECT_TRUE(serializer.has("test_float2"));
  EXPECT_TRUE(serializer.has("test_int1"));
  EXPECT_TRUE(serializer.has("test_int2"));
  EXPECT_TRUE(serializer.has("test_string1"));
  EXPECT_TRUE(serializer.has("test_string2"));
  EXPECT_TRUE(serializer.has("test_class1"));
  EXPECT_TRUE(serializer.has("test_class2"));
}

template <class T> 
void Meta_JsonSerializer_Add_Test_Basic_Types(const T& orig, const Json::Value& value);
template <>
void Meta_JsonSerializer_Add_Test_Basic_Types<bool>(const bool& orig, const Json::Value& value)
{
  EXPECT_TRUE(orig == value.asBool());
}

//ensures that variables are added to the root with the correct types and values
TEST(Meta_JsonSerializer, add)
{
  meta::JSonSerializer serializer;

  //adding variables to serializer
  serializer.add("test_class1", test_class1);
  serializer.add("test_class2", test_class2);
  serializer.add("test_bool1", test_bool1);
  serializer.add("test_bool2", test_bool2);
  serializer.add("test_double1", test_double1);
  serializer.add("test_double2", test_double2);
  serializer.add("test_float1", test_float1);
  serializer.add("test_float2", test_float2);
  serializer.add("test_int1", test_int1);
  serializer.add("test_int2", test_int2);
  serializer.add("test_string1", test_string1);
  serializer.add("test_string2", test_string2);

  //get json root to check if variables were correctly added to the root object
  Json::Value root = serializer.get_root();
  Json::Value json_test_class1 = root.get("test_class1", 0);
  Json::Value json_test_class2 = root.get("test_class2", 0);
  Json::Value json_test_bool1 = root.get("test_bool1", false);
  Json::Value json_test_bool2 = root.get("test_bool2", false);
  Json::Value json_test_double1 = root.get("test_double1", 0.0);
  Json::Value json_test_double2 = root.get("test_double2", 0.0);
  Json::Value json_test_float1 = root.get("test_float1", 0.0f);
  Json::Value json_test_float2 = root.get("test_float2", 0.0f);
  Json::Value json_test_int1 = root.get("test_int1", 0);
  Json::Value json_test_int2 = root.get("test_int2", 0);
  Json::Value json_test_string1 = root.get("test_string1", "");
  Json::Value json_test_string2 = root.get("test_string2", "");

  //function to check complex object test_class
  auto test_class_check = [](const test_class& orig, const Json::Value& value) -> void
  {
    Json::Value fields = value["fields"];
    Json::Value test_float = fields["test_float"];

    EXPECT_TRUE(orig.test_bool == fields.get("test_bool", false).asBool());
    EXPECT_TRUE(orig.test_double == fields.get("test_double", 0.0).asDouble());
    EXPECT_TRUE(test_float.isConvertibleTo(Json::realValue));
    EXPECT_TRUE(orig.test_float == static_cast<float>(test_float.asDouble()));
    EXPECT_TRUE(orig.test_int == fields.get("test_int", 0).asInt());
    EXPECT_TRUE(orig.test_string == fields.get("test_string", "").asString());
  };

  //testing both versions of test_class
  test_class_check(test_class1, json_test_class1);
  test_class_check(test_class2, json_test_class2);

  //test all versions of basic types added to serializer
  EXPECT_TRUE(test_bool1 == json_test_bool1.asBool());
  EXPECT_TRUE(test_bool2 == json_test_bool2.asBool());
  EXPECT_TRUE(test_double1 == json_test_double1.asDouble());
  EXPECT_TRUE(test_double2 == json_test_double2.asDouble());

  EXPECT_TRUE(json_test_float1.isConvertibleTo(Json::realValue));
  EXPECT_TRUE(json_test_float2.isConvertibleTo(Json::realValue));
  EXPECT_TRUE(test_float1 == static_cast<float>(json_test_float1.asDouble()));
  EXPECT_TRUE(test_float2 == static_cast<float>(json_test_float2.asDouble()));

  EXPECT_TRUE(test_int1 == json_test_int1.asInt());
  EXPECT_TRUE(test_int2 == json_test_int2.asInt());
  EXPECT_TRUE(test_string1 == json_test_string1.asString());
  EXPECT_TRUE(test_string2 == json_test_string2.asString());
}

//ensures that variables are added to the root with the correct types and values
TEST(Meta_JsonSerializer, get)
{
  meta::JSonSerializer serializer;
  serializer.set(BuildVariableMap());

  test_class get_test_class1;
  test_class get_test_class2;
  bool get_test_bool1;
  bool get_test_bool2;
  float get_test_float1;
  float get_test_float2;
  double get_test_double1;
  double get_test_double2;
  int get_test_int1;
  int get_test_int2;
  string get_test_string1;
  string get_test_string2;

  EXPECT_TRUE(serializer.get("test_class1", get_test_class1));
  EXPECT_TRUE(serializer.get("test_class2", get_test_class2));

  EXPECT_TRUE(serializer.get("test_bool1", get_test_bool1));
  EXPECT_TRUE(serializer.get("test_bool2", get_test_bool2));

  EXPECT_TRUE(serializer.get("test_float1", get_test_float1));
  EXPECT_TRUE(serializer.get("test_float2", get_test_float2));

  EXPECT_TRUE(serializer.get("test_double1", get_test_double1));
  EXPECT_TRUE(serializer.get("test_double2", get_test_double2));

  EXPECT_TRUE(serializer.get("test_int1", get_test_int1));
  EXPECT_TRUE(serializer.get("test_int2", get_test_int2));

  EXPECT_TRUE(serializer.get("test_string1", get_test_string1));
  EXPECT_TRUE(serializer.get("test_string2", get_test_string2));

  EXPECT_TRUE(get_test_class1 == test_class1);
  EXPECT_TRUE(get_test_class2 == test_class2);

  EXPECT_TRUE(get_test_bool1 == test_bool1);
  EXPECT_TRUE(get_test_bool2 == test_bool2);

  EXPECT_TRUE(get_test_float1 == test_float1);
  EXPECT_TRUE(get_test_float2 == test_float2);

  EXPECT_TRUE(get_test_double1 == test_double1);
  EXPECT_TRUE(get_test_double2 == test_double2);

  EXPECT_TRUE(get_test_int1 == test_int1);
  EXPECT_TRUE(get_test_int2 == test_int2);

  EXPECT_TRUE(get_test_string1 == test_string1);
  EXPECT_TRUE(get_test_string2 == test_string2);
}