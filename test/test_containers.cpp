#include "core/types.h"
#include "containers/hash_map.h"
#include "containers/slot_map.h"
#include "containers/string.h"
#include "containers/vector.h"
#include <utility>
#include <cstdio>

template <typename T> void print(const T&);
template <> void print(const string& str)
{
  printf("%s : hash(%u) length(%u)\n", str.c_str(), str.hash(), str.length());
}
template <typename T> void print(const vector<T>& vec)
{
  printf("vector - size(%u) capacity(%u)\n", vec.size(), vec.capacity());
  for (size_t i = 0; i < vec.size(); ++i)
    print(vec[i]);
}
void test_string()
{
  string default_construct;
  string charptr_construct("charptr construct");
  string copy_construct(charptr_construct);
  
  const string conststring("const string");
  string move_construct_const(conststring);

  string normalstring("normal string");

  string moveable_string("movable string");
  string move_construct_nonconst = std::move(moveable_string);
  print(move_construct_nonconst);

  string subscript_nonconst("normal string to be subscripted");
  const string subscript_const("const string to be subscripted");

  print(default_construct);
  print(charptr_construct);
  print(copy_construct);

  print(conststring);
  print(move_construct_const);

  print(normalstring);
  print(move_construct_nonconst);

  print(subscript_nonconst);
  print(subscript_const);

  //test subscripts
  auto test_subscript = [](string subscript) {
    printf("\ntesting subscript...\n");
    printf("%s\n", subscript.c_str());
    for (size_t i = 0; i < subscript.length(); ++i)
      printf("%c", subscript[i]);
    printf("\n");
  };
  test_subscript(subscript_const);
  test_subscript(subscript_nonconst);

  auto test_assignment = [](string left, string right) {
    printf("\ntesting assignment...\n");
    printf("left: %s \tright: %s\n", left.c_str(), right.c_str());
    left = right;
    printf("left: %s \tright: %s\n", left.c_str(), right.c_str());
  };
  test_assignment(charptr_construct, subscript_const);

  string init_list({ "hello", " world", " test" });
  print(init_list);
}

void test_vector()
{
  vector<string> default_constructor;
  vector<string> size_constructor(20);
  vector<string> list_constructor({ string("hello"), string(" world"), string(" second "), string("test") });

  print(default_constructor);
  print(size_constructor);
  print(list_constructor);


  string pushback_str("this string is being pushed back");
  vector<string> pushback_normal;
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);
  pushback_normal.push_back(pushback_str);

  print(pushback_normal);

  vector<string> pushback_rvalue;
  pushback_rvalue.push_back("0");
  pushback_rvalue.push_back("1");
  pushback_rvalue.push_back("2");
  pushback_rvalue.push_back("3");
  pushback_rvalue.push_back("4");
  pushback_rvalue.push_back("5");
  pushback_rvalue.push_back("6");
  pushback_rvalue.push_back("7");
  pushback_rvalue.push_back("8");
  pushback_rvalue.push_back("9");
  pushback_rvalue.push_back("10");
  pushback_rvalue.push_back("11");
  pushback_rvalue.push_back("12");
  pushback_rvalue.push_back("13");
  pushback_rvalue.push_back("14");
  pushback_rvalue.push_back("15");
  pushback_rvalue.push_back("16");
  pushback_rvalue.push_back("17");
  pushback_rvalue.push_back("18");
  pushback_rvalue.push_back("19");
  pushback_rvalue.push_back("20");
  pushback_rvalue.push_back("21");
  pushback_rvalue.push_back("22");
  pushback_rvalue.push_back("23");
  pushback_rvalue.push_back("24");
  pushback_rvalue.push_back("25");
  pushback_rvalue.push_back("26");
  pushback_rvalue.push_back("27");
  pushback_rvalue.push_back("28");
  pushback_rvalue.push_back("29");
  pushback_rvalue.push_back("30");
  pushback_rvalue.push_back("31");
  pushback_rvalue.push_back("32");
  pushback_rvalue.push_back("33");
  pushback_rvalue.push_back("34");

  print(pushback_rvalue);

  auto check_operators = [](const vector<string>& vec) {
    for (size_t i = 0; i < vec.size(); ++i)
    {
      if (i != atoi(vec[i].c_str()))
        printf("Error in pushback or subscript operators, value: %u\n", i);
    }
  };
  
  check_operators(pushback_rvalue);
}
int main(void)
{
  test_string();
  test_vector();
  return 0;
}