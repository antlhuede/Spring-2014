#include "meta\meta.h"
#include "gmock\gmock.h"
#include "serializers\serializers.h"

#include <fstream>

void run_basic_test_code();

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

  bool operator==(const test_class& rhs) const
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

  string name;
  void set_property(const string& test) { m_property = test; }
  const string& get_property() const { return m_property; }

  const string test_event(const string& arg1, float arg2, int arg3, bool arg4)
  {
    std::cout << "test event: " << arg1 << " " << arg2 << " " << arg3 << " " << arg4 << std::endl;
    return arg1 + " event was processed";
  }
private:
  string m_property = "default property value";
};
struct global_message_table
{
  static meta::messenger* m_table;
  template <class... Args>
  static bool DispatchMessage(const string& name, Args... args)
  {
    return m_table->DispatchMessage(name, std::forward<Args>(args)...);
  }
  static bool AddListener(const string& name, const meta::function& func)
  {
    return m_table->AddListener(name, func);
  }
};
meta::messenger* global_message_table::m_table = nullptr;

DECLARE_META_OBJECT(test_class)
  .Field("name", &test_class::name)
  .Field("test_string", &test_class::test_string)
  .Field("test_int", &test_class::test_int)
  .Field("test_float", &test_class::test_float)
  .Field("test_double", &test_class::test_double)
  .Field("test_bool", &test_class::test_bool)
  .Property("test_property", &test_class::get_property, &test_class::set_property)
  .Property("test_getter_only", &test_class::get_property)
  .Property("test_setter_only", &test_class::set_property)
  .Event("test_event", &test_class::test_event)
END_META_OBJECT()

int test_int1 = 5;
int test_int2 = 12;
float test_float1 = 13.5f;
float test_float2 = 332.5f;
string* test_string1 = nullptr;
string* test_string2 = nullptr;
double test_double1 = 0.0928;
double test_double2 = 10293.9382;
bool test_bool1 = true;
bool test_bool2 = false;

test_class* test_class1 = nullptr;
test_class* test_class2 = nullptr;
xml::serializer_ptr xml_serializer = nullptr;
json::serializer_ptr json_serializer = nullptr;
meta::function* pFunc = nullptr;

void make_test_memory()
{
  global_message_table::m_table = new meta::messenger;
  test_string1 = new string("test123");
  test_string2 = new string("123 boogaloo");
  test_class1 = new test_class(test_int1, *test_string1, test_float1, test_double1, test_bool1);
  test_class2 = new test_class(test_int2, *test_string2, test_float2, test_double2, test_bool2);

  xml_serializer = new xml::serializer;
  json_serializer = new json::serializer;
  auto lambda = [](int a, float b, double c, const string& d) {
    std::cout << a << " " << b << " " << c << " " << d << std::endl;
  };

  pFunc = new meta::function(lambda);
}
void delete_test_memory()
{
  delete global_message_table::m_table;
  delete pFunc;
  delete json_serializer;
  delete xml_serializer;

  delete test_string1;
  delete test_string2;
  delete test_class1;
  delete test_class2;
}

//TEST(SampleTest, TestThatItWorked)
//{
//  EXPECT_TRUE(true);
//  EXPECT_FALSE(false);
//}
//
//TEST(Meta_CORE, typeof)
//{
//#define TEST_TYPEOF(TYPE, NUM_FIELDS) \
//  const meta::type TYPE##_ = meta::typeof<TYPE>();        \
//  EXPECT_TRUE(TYPE##_.name() == #TYPE);                   \
//  EXPECT_TRUE(TYPE##_.size() == sizeof(TYPE));            \
//  EXPECT_TRUE(TYPE##_.fields().size() == NUM_FIELDS)
//#define TEST_TYPEOF_BASIC_TYPES(TYPE) TEST_TYPEOF(TYPE, 0)
//
//  TEST_TYPEOF_BASIC_TYPES(nulltype);
//  TEST_TYPEOF_BASIC_TYPES(int);
//  TEST_TYPEOF_BASIC_TYPES(bool);
//  TEST_TYPEOF_BASIC_TYPES(float);
//  TEST_TYPEOF_BASIC_TYPES(double);
//  TEST_TYPEOF_BASIC_TYPES(string);
//
//  TEST_TYPEOF(test_class, 5);
//
//#undef TEST_TYPEOF_BASIC_TYPES
//#undef TEST_TYPEOF
//}

void run_serializer_test(serializers::base_serializer_ptr serializer, const string& file1, const string& file2)
{
  string file_path("documents/");
  string first_file = file_path + file1;
  string second_file = file_path + file2;

  test_class tc(test_int1, *test_string1, test_float1, test_double1, test_bool1);
  test_class tc2(test_int2, *test_string2, test_float2, test_double2, test_bool2);
  
  serializer->add("test_class1", tc);
  serializer->add("test_class2", tc2);
  serializer->write(first_file);
  serializer->clear();
  
  test_class read_test1;
  test_class read_test2;
  
  serializer->read(first_file);
  serializer->get("test_class1", read_test1);
  serializer->get("test_class2", read_test2);
  serializer->clear();
  read_test1.set_property(string("read_test1: ") + read_test1.get_property() + " property was successfully read and edited");
  read_test2.set_property(string("read_test2: ") + read_test2.get_property() + " property was successfully read and edited");
  serializer->add("read_test1", read_test1);
  serializer->add("read_test2", read_test2);
  serializer->write(second_file);
  serializer->clear();
}

class test_funcs_class
{
public:
  string func_test1() { return "fuck return values"; }
  void func_test2(int a) { std::cout << a << std::endl; }
  void func_test3(int a, float b) { std::cout << a << " " << b << std::endl; }
  void func_test4(int a, float b, double c) { std::cout << a << " " << b << " " << c << std::endl; }
  void func_test5(int a, float b, double c, const string& d) const {
    std::cout << a << " " << b << " " << c << " " << d << std::endl;
  }
  void func_test6(const test_class& d) const
  {
    std::cout << d.test_int << " " << d.test_float << " " << d.test_double << " " << d.test_string << std::endl;
  }
};
class test_message_class : public meta::messenger
{
public:
  test_message_class()
  {
    AddListener("Player Dead", meta::function(&test_message_class::PlayerDeadFunc, this));
  }
  void PlayerDeadFunc(int lives_left)
  {
    std::cout << "Player Dead Func: lives_left(" << lives_left << ")" << std::endl;
  }
private:
};
DECLARE_META_OBJECT(test_funcs_class) END_META_OBJECT()
DECLARE_META_OBJECT(test_message_class) END_META_OBJECT()
void func_test1() {}
void func_test2(int a) {}
void func_test3(int a, float b) {}
void func_test4(int a, float b, double c) {}
void func_test5(int a, float b, double c, const string& d) 
{
  std::cout << a << " " << b << " " << c << " " << d << std::endl;
}
void func_test6(const test_class& d)
{
  std::cout << d.test_int << " " << d.test_float << " " << d.test_double << " " << d.test_string << std::endl;
}
void GlobalPlayerDeadFunc(int lives_left)
{
  std::cout << "In global player dead func: lives left(" << lives_left << ")" << std::endl;
}
void test_properties()
{
  test_class tc;
  const meta::type* tc_type = meta::typeof(tc);
  const meta::property p1 = tc_type->property("test_property");
  string value("initial test");
  p1.set(&tc, value);
  std::cout << p1.get_as<string>(&tc) << std::endl;

  const meta::property p2 = tc_type->property("test_getter_only");
  std::cout << p2.get_as<string>(&tc) << std::endl;

  const meta::event e1 = tc_type->event("test_event");
  std::cout << e1.trigger(&tc, string("hello world"), 13.5f, 255, true).get_as<string>() << std::endl;
}
void run_basic_test_code()
{
  test_properties();

  std::cout << "start run basic test" << std::endl;
  make_test_memory();

  test_message_class tm;
  //tm.DispatchMessage("Player Dead", 2);
  global_message_table::AddListener("Player Dead", meta::function(&test_message_class::PlayerDeadFunc, &tm));
  global_message_table::AddListener("Player Dead", meta::function(&GlobalPlayerDeadFunc));
  global_message_table::DispatchMessage("Player Dead", 2);

  auto lambda_func = [](const string& name)->void {
    std::cout << "IN LAMBDA: " << name <<  std::endl;
  };
  //store_lambda<decltype(lambda_func)> store(lambda_func);
  //store();
  static const int NUM_ITERATIONS = 1;
  for (int i = 0; i < NUM_ITERATIONS; ++i)
    (*pFunc)(i, 3.5f, 123.0, string("fuck you"));

  meta::variant variant;
  variant = 5;
  assert(variant.is_type<int>());
  std::cout << "variant is int:" << variant.get_as<int>() << "\n";
  variant = 15.32413;
  assert(variant.is_type<double>());
  std::cout << "variant is double:" << variant.get_as<double>() << "\n";
  
  variant = 2.32f;
  assert(variant.is_type<float>());
  std::cout << "variant is float:" << variant.get_as<float>() << "\n";
  
  variant = false;
  assert(variant.is_type<bool>());
  std::cout << "variant is bool:" << variant.get_as<bool>() << "\n";
  
  variant = "hello world";
  assert(variant.is_type<string>());
  std::cout << "variant is string:" << variant.get_as<string>() << "\n";
  
  const meta::type* type = meta::typeof<test_class>();
  
  
  test_class tc(test_int1, *test_string1, test_float1, test_double1, test_bool1);
  test_class tc2(test_int2, *test_string2, test_float2, test_double2, test_bool2);
  
  run_serializer_test(xml_serializer, "test.xml", "test2.xml");
  run_serializer_test(json_serializer, "test.json", "test2.json");
  
  Json::Value root;
  const vector<const meta::field>& fields = type->fields();
  
  for (size_t i = 0; i < fields.size(); ++i)
  {
    Json::Value value;
    value[fields[i].name()] = fields[i].to_string(&tc);
    root[type->to_string(&tc)].append(value);
    //std::cout << fields[i]->type().name() << " " << fields[i]->name() << " " << fields[i]->offset() << std::endl;
  }
  Json::StyledWriter writer;
  string output = writer.write(root);
  std::cout << output;
  
  
  float testint = 12.5;
  string teststr = "\nteststr";
  const meta::type* test = meta::typeof(testint);
  test->write(std::cout, &testint);
  test = meta::typeof(teststr);
  
  std::cout << test->name() << " " << test->size() << "\n\n";

  test_funcs_class tfc;
  string hello_world("Hello World");

  meta::function func1(func_test1);
  meta::function func2(func_test2);
  meta::function func3(func_test3);
  meta::function func4(func_test4);
  meta::function func5(func_test5);
  meta::function func6(func_test6);
  
  meta::function funcl(lambda_func);
  funcl(hello_world);

  meta::function mfunc1(&test_funcs_class::func_test1, &tfc);
  meta::function mfunc2(&test_funcs_class::func_test2, &tfc);
  meta::function mfunc3(&test_funcs_class::func_test3, &tfc);
  meta::function mfunc4(&test_funcs_class::func_test4, &tfc);
  meta::function mfunc5(&test_funcs_class::func_test5, &tfc);
  meta::function mfunc6(&test_funcs_class::func_test6, &tfc);

  meta::variant fuck_returns = mfunc1.call(&tfc);
  std::cout << fuck_returns.get_as<string>() << std::endl;
  mfunc2.call(&tfc, 3);
  mfunc3.call(&tfc, 3, 3.5f);
  mfunc4.call(&tfc, 3, 3.5f, 123.0);
  mfunc5.call(&tfc, 3, 3.5f, 123.0, hello_world);

  auto print_args = [](const string& name, meta::function& func) -> void {
    std::cout << name << " test: " << std::endl;
    for (size_t i = 0; i < func.traits().numArguments; ++i)
      std::cout << "  arg " << i << ": " << func.traits().args[i].type->name() << std::endl;
    std::cout << "  member func: " << func.traits().isMemberFunction << std::endl;
  };

  print_args("function1", func1);
  print_args("function2", func2);
  print_args("function3", func3);
  print_args("function4", func4);
  print_args("function5", func5);
  print_args("member-function1", mfunc1);
  print_args("member-function2", mfunc2);
  print_args("member-function3", mfunc3);
  print_args("member-function4", mfunc4);
  print_args("member-function5", mfunc5);

  //print_args("member function1", mfunc1);
  //print_args("member function2", mfunc2);
  //print_args("member function3", mfunc3);
  //print_args("member function4", mfunc4);
  //print_args("member function5", mfunc5);
  func1();
  func2(3);
  func3(3, 3.5f);
  func4(3, 3.5f, 123.0);
  func5(3, 3.5f, 123.0, hello_world);
  mfunc1();
  mfunc2(3);
  mfunc3(3, 3.5f);
  mfunc4(3, 3.5f, 123.0);
  mfunc5(3, 3.5f, 123.0, hello_world);

  func6(tc);
  mfunc6(tc2);


  delete_test_memory();
}

//
//hash_map<string, Json::Value> BuildVariableMap()
//{
//  hash_map<string, Json::Value> variables;
//
//  variables["test_bool1"] = test_bool1;
//  variables["test_bool2"] = test_bool2;
//  variables["test_double1"] = test_double1;
//  variables["test_double2"] = test_double2;
//  variables["test_float1"] = test_float1;
//  variables["test_float2"] = test_float2;
//  variables["test_int1"] = test_int1;
//  variables["test_int2"] = test_int2;
//  variables["test_string1"] = test_string1;
//  variables["test_string2"] = test_string2;
//
//#define BUILD_TEST_CLASS_AS_JSON_VALUE(SUFFIX)  \
//  Json::Value json_test_class##SUFFIX;                                      \
//  json_test_class##SUFFIX["fields"]["test_bool"] = test_bool##SUFFIX;       \
//  json_test_class##SUFFIX["fields"]["test_double"] = test_double##SUFFIX;   \
//  json_test_class##SUFFIX["fields"]["test_float"] = test_float##SUFFIX;     \
//  json_test_class##SUFFIX["fields"]["test_int"] = test_int##SUFFIX;         \
//  json_test_class##SUFFIX["fields"]["test_string"] = test_string##SUFFIX;   \
//  json_test_class##SUFFIX["type"] = "test_class";
//
//  BUILD_TEST_CLASS_AS_JSON_VALUE(1)
//  BUILD_TEST_CLASS_AS_JSON_VALUE(2)
//#undef BUILD_TEST_CLASS_AS_JSON_VALUE
//
//  variables["test_class1"] = json_test_class1;
//  variables["test_class2"] = json_test_class2;
//
//  return variables;
//}
//
//TEST(Meta_JsonSerializer, has)
//{
//  meta::JSonSerializer serializer;
//  serializer.set(BuildVariableMap());
//
//  EXPECT_FALSE(serializer.has(""));
//  EXPECT_TRUE(serializer.has("test_bool1"));
//  EXPECT_TRUE(serializer.has("test_bool2"));
//  EXPECT_TRUE(serializer.has("test_double1"));
//  EXPECT_TRUE(serializer.has("test_double2"));
//  EXPECT_TRUE(serializer.has("test_float1"));
//  EXPECT_TRUE(serializer.has("test_float2"));
//  EXPECT_TRUE(serializer.has("test_int1"));
//  EXPECT_TRUE(serializer.has("test_int2"));
//  EXPECT_TRUE(serializer.has("test_string1"));
//  EXPECT_TRUE(serializer.has("test_string2"));
//  EXPECT_TRUE(serializer.has("test_class1"));
//  EXPECT_TRUE(serializer.has("test_class2"));
//}
//
//template <class T>
//void Meta_JsonSerializer_Add_Test_Basic_Types(const T& orig, const Json::Value& value);
//template <>
//void Meta_JsonSerializer_Add_Test_Basic_Types<bool>(const bool& orig, const Json::Value& value)
//{
//  EXPECT_TRUE(orig == value.asBool());
//}
//
////ensures that variables are added to the root with the correct types and values
//TEST(Meta_JsonSerializer, add)
//{
//  meta::JSonSerializer serializer;
//
//  //adding variables to serializer
//  serializer.add("test_class1", test_class1);
//  serializer.add("test_class2", test_class2);
//  serializer.add("test_bool1", test_bool1);
//  serializer.add("test_bool2", test_bool2);
//  serializer.add("test_double1", test_double1);
//  serializer.add("test_double2", test_double2);
//  serializer.add("test_float1", test_float1);
//  serializer.add("test_float2", test_float2);
//  serializer.add("test_int1", test_int1);
//  serializer.add("test_int2", test_int2);
//  serializer.add("test_string1", test_string1);
//  serializer.add("test_string2", test_string2);
//
//  //get json root to check if variables were correctly added to the root object
//  Json::Value root = serializer.get_root();
//  Json::Value json_test_class1 = root.get("test_class1", 0);
//  Json::Value json_test_class2 = root.get("test_class2", 0);
//  Json::Value json_test_bool1 = root.get("test_bool1", false);
//  Json::Value json_test_bool2 = root.get("test_bool2", false);
//  Json::Value json_test_double1 = root.get("test_double1", 0.0);
//  Json::Value json_test_double2 = root.get("test_double2", 0.0);
//  Json::Value json_test_float1 = root.get("test_float1", 0.0f);
//  Json::Value json_test_float2 = root.get("test_float2", 0.0f);
//  Json::Value json_test_int1 = root.get("test_int1", 0);
//  Json::Value json_test_int2 = root.get("test_int2", 0);
//  Json::Value json_test_string1 = root.get("test_string1", "");
//  Json::Value json_test_string2 = root.get("test_string2", "");
//
//  //function to check complex object test_class
//  auto test_class_check = [](const test_class& orig, const Json::Value& value) -> void
//  {
//    Json::Value fields = value["fields"];
//    Json::Value test_float = fields["test_float"];
//
//    EXPECT_TRUE(orig.test_bool == fields.get("test_bool", false).asBool());
//    EXPECT_TRUE(orig.test_double == fields.get("test_double", 0.0).asDouble());
//    EXPECT_TRUE(test_float.isConvertibleTo(Json::realValue));
//    EXPECT_TRUE(orig.test_float == static_cast<float>(test_float.asDouble()));
//    EXPECT_TRUE(orig.test_int == fields.get("test_int", 0).asInt());
//    EXPECT_TRUE(orig.test_string == fields.get("test_string", "").asString());
//  };
//
//  //testing both versions of test_class
//  test_class_check(test_class1, json_test_class1);
//  test_class_check(test_class2, json_test_class2);
//
//  //test all versions of basic types added to serializer
//  EXPECT_TRUE(test_bool1 == json_test_bool1.asBool());
//  EXPECT_TRUE(test_bool2 == json_test_bool2.asBool());
//  EXPECT_TRUE(test_double1 == json_test_double1.asDouble());
//  EXPECT_TRUE(test_double2 == json_test_double2.asDouble());
//
//  EXPECT_TRUE(json_test_float1.isConvertibleTo(Json::realValue));
//  EXPECT_TRUE(json_test_float2.isConvertibleTo(Json::realValue));
//  EXPECT_TRUE(test_float1 == static_cast<float>(json_test_float1.asDouble()));
//  EXPECT_TRUE(test_float2 == static_cast<float>(json_test_float2.asDouble()));
//
//  EXPECT_TRUE(test_int1 == json_test_int1.asInt());
//  EXPECT_TRUE(test_int2 == json_test_int2.asInt());
//  EXPECT_TRUE(test_string1 == json_test_string1.asString());
//  EXPECT_TRUE(test_string2 == json_test_string2.asString());
//}
//
////ensures that variables are added to the root with the correct types and values
//TEST(Meta_JsonSerializer, get)
//{
//  meta::JSonSerializer serializer;
//  serializer.set(BuildVariableMap());
//
//  test_class get_test_class1;
//  test_class get_test_class2;
//  bool get_test_bool1;
//  bool get_test_bool2;
//  float get_test_float1;
//  float get_test_float2;
//  double get_test_double1;
//  double get_test_double2;
//  int get_test_int1;
//  int get_test_int2;
//  string get_test_string1;
//  string get_test_string2;
//
//  EXPECT_TRUE(serializer.get("test_class1", get_test_class1));
//  EXPECT_TRUE(serializer.get("test_class2", get_test_class2));
//
//  EXPECT_TRUE(serializer.get("test_bool1", get_test_bool1));
//  EXPECT_TRUE(serializer.get("test_bool2", get_test_bool2));
//
//  EXPECT_TRUE(serializer.get("test_float1", get_test_float1));
//  EXPECT_TRUE(serializer.get("test_float2", get_test_float2));
//
//  EXPECT_TRUE(serializer.get("test_double1", get_test_double1));
//  EXPECT_TRUE(serializer.get("test_double2", get_test_double2));
//
//  EXPECT_TRUE(serializer.get("test_int1", get_test_int1));
//  EXPECT_TRUE(serializer.get("test_int2", get_test_int2));
//
//  EXPECT_TRUE(serializer.get("test_string1", get_test_string1));
//  EXPECT_TRUE(serializer.get("test_string2", get_test_string2));
//
//  EXPECT_TRUE(get_test_class1 == test_class1);
//  EXPECT_TRUE(get_test_class2 == test_class2);
//
//  EXPECT_TRUE(get_test_bool1 == test_bool1);
//  EXPECT_TRUE(get_test_bool2 == test_bool2);
//
//  EXPECT_TRUE(get_test_float1 == test_float1);
//  EXPECT_TRUE(get_test_float2 == test_float2);
//
//  EXPECT_TRUE(get_test_double1 == test_double1);
//  EXPECT_TRUE(get_test_double2 == test_double2);
//
//  EXPECT_TRUE(get_test_int1 == test_int1);
//  EXPECT_TRUE(get_test_int2 == test_int2);
//
//  EXPECT_TRUE(get_test_string1 == test_string1);
//  EXPECT_TRUE(get_test_string2 == test_string2);
//}

