#include "IToolInterface.hpp"

struct TranslationTool
{
  int mTranslation;
};

void* IToolCreate()
{
  TranslationTool* tool = new TranslationTool();
  tool->mTranslation = 9999;
  return tool;
}

void IToolDestroy(void* instance)
{
  TranslationTool* self = (TranslationTool*)instance;

  delete (TranslationTool*)self;
}

const char* IToolGetName(void* instance)
{
  return "Translation Tool";
}

int IToolComputeValue(void* instance, int x, int y)
{
  TranslationTool* self = (TranslationTool*)instance;

  return self->mTranslation;
}
