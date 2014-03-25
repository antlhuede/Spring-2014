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

#include <stdlib.h>
#include <crtdbg.h>

int main(int argc, char** argv)
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _crtBreakAlloc = 1740;
  meta::initialize();

  ::testing::InitGoogleMock(&argc, argv);
  int ret_val = RUN_ALL_TESTS();
  
  std::cout << "\n\n";
  
  run_basic_test_code();

  meta::destroy();

  return ret_val;
}
