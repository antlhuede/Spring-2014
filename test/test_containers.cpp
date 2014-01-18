#include "core/types.h"
#include "containers/hash_map.h"
#include "containers/slot_map.h"
#include "containers/string.h"
#include "containers/vector.h"
#include <utility>
#include <cstdio>
void print_string(const string& str)
{
  printf("%s : hash(%u) length(%u)\n", str.c_str(), str.hash(), str.length());
}
string return_string()
{
  return string("rvalue string");
}
void test_string()
{
  string default_construct;
  string charptr_construct("charptr construct");
  string copy_construct(charptr_construct);
  
  const string conststring("const string");
  string move_construct_const(conststring);

  string normalstring("normal string");

  string move_construct_nonconst(normalstring);

  string subscript_nonconst("normal string to be subscripted");
  const string subscript_const("const string to be subscripted");

  print_string(default_construct);
  print_string(charptr_construct);
  print_string(copy_construct);

  print_string(conststring);
  print_string(move_construct_const);

  print_string(normalstring);
  print_string(move_construct_nonconst);

  print_string(subscript_nonconst);
  print_string(subscript_const);

  //test subscripts
  auto test_subscript = [](string subscript) {
    printf("\ntesting subscript...\n");
    printf("%s\n", subscript.c_str());
    for (size i = 0; i < subscript.length(); ++i)
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
}
int main(void)
{
  test_string();
  return 0;
}