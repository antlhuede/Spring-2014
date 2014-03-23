#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <sstream>
#include <stack>
#include <memory>

#include "meta\meta.h"
#include "gmock\gmock.h"

void run_basic_test_code();

int main(int argc, char** argv)
{
  meta::initialize();

  ::testing::InitGoogleMock(&argc, argv);
  int ret_val = RUN_ALL_TESTS();

  std::cout << "\n\n";

  run_basic_test_code();

  meta::destroy();

  return ret_val;
}
