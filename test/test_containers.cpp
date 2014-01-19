#include "core/types.h"
#include "containers/hash_map.h"
#include "containers/slot_map.h"
#include "containers/string.h"
#include "containers/vector.h"
#include <utility>
#include <cstdio>
#include <cassert>

template <typename T> void print(const T&);
template <> void print(const string& str)
{
  printf("%s : hash(%u) length(%u)\n", str.c_str(), str.hash(), str.length());
}
template <template <class> class container, class T> void print(container<T>& vec)
{
  printf("vector - size(%u) capacity(%u)\n", vec.size(), vec.capacity());

  for (auto it = vec.begin(); it != vec.end(); ++it)
    print(*it);
  
  //test const version
  printf("test const version\n");
  const container<T>& const_vec = vec;
  for (auto it = const_vec.begin(); it != const_vec.end(); ++it)
    print(*it);
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

  auto test_all_subscript = [&test_subscript](initializer_list<string> list) {
    for (string s : list)
      test_subscript(s);
  };

  test_all_subscript({subscript_const, subscript_const});

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

  auto check_operators = [](const vector<string>& vec, int32 start, size_t length) {
    assert(vec.size() == length);
    bool error_found = false;
    for (size_t i = 0; i < vec.size(); ++i)
    {
      if (atoi(vec[i].c_str()) != start + i)
      {
        error_found = true;
        printf("Error in pushback or subscript operators, value: %u\n", i);
      }
    }
    if (error_found == false) printf("subscript and pushback operators successful\n");
  };
  auto itostr = [&check_operators](vector<string>& vec, int32 start, int32 length){
    char buffer[256];
    for (int32 i = start; i < start + length; ++i)
    {
      vec.push_back(_itoa(i, buffer, 10));
    }

    check_operators(vec, start, length);
  };

  int32 start = -10;
  int32 length = 45;
  vector<string> pushback_rvalue;
  itostr(pushback_rvalue, start, length);
  print(pushback_rvalue);

  //test copy construct

  vector<string> copy_normal(pushback_normal);
  vector<string> assign_normal;
  assign_normal = pushback_normal;
  printf("\ntest copy normal:\n");
  print(copy_normal);

  printf("\ntest assign normal:\n");
  print(assign_normal);

  auto run_test = [](vector<string>& vec, string test_name, void(*callback)(vector<string>&)) {
    printf("\ntest %s:\n", test_name.c_str());
    printf("before:\n");
    print(vec);
    callback(vec);
    printf("after:\n");
    print(vec);
  };

  auto clear = [](vector<string>& vec) { vec.clear(); };
  auto emplace_back = [](vector<string>& vec) { vec.emplace_back("emp1", "emp2", "emp3"); };
  auto reset = [](vector<string>& vec) { vec.reset(); };

  run_test(pushback_normal, "emplace back", emplace_back);
  run_test(pushback_rvalue, "emplace back", emplace_back);
  run_test(pushback_normal, "clear", clear);
  run_test(pushback_rvalue, "clear", clear);
  run_test(pushback_normal, "emplace back", emplace_back);
  run_test(pushback_rvalue, "emplace back", emplace_back);
  run_test(pushback_normal, "reset", reset);
  run_test(pushback_rvalue, "reset", reset);

  
}
int main(void)
{
  test_string();
  test_vector();
  return 0;
}