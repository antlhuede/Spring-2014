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
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  //_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  //_crtBreakAlloc = 145;

  meta::initialize();

  //::testing::InitGoogleMock(&argc, argv);
  int ret_val = 0; //RUN_ALL_TESTS();

  std::cout << "\n\n";

  run_basic_test_code();

  meta::destroy();
  _CrtDumpMemoryLeaks();
  return ret_val;
}
