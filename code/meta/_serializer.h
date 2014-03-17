#pragma once

namespace meta
{
class serializer
{
public:
  virtual ~serializer() = 0 {};

  virtual bool add(const string& name, const meta::type& t, void* obj) = 0;
  virtual void write(ostream& stream) = 0;
  virtual bool read(istream& stream) = 0;
  virtual bool get(const string& var, const meta::type& t, void* obj) = 0;
  virtual bool has(const string& var) = 0;
  virtual void clear() = 0;
};
}

#include "_json_serializer.h"
