#pragma once

namespace meta
{
class serializer : public object_container
{
public:
  virtual ~serializer() = 0 {};

  virtual void write(const string& file) const = 0;
  virtual bool read(const string& file) = 0;
};
}
