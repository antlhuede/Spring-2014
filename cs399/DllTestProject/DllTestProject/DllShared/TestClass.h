#ifdef TEST_EXPORTS
#define TEST_DECL __declspec(dllexport)
#else
#define TEST_DECL __declspec(dllimport)
#endif

//#define TEST_DECL


class TEST_DECL TestClass
{
public:

  TestClass();
  ~TestClass();

  int GetValue();
  void SetValue();

  int value;

};