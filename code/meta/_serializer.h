#pragma once

namespace meta
{
//container policy necessary for dependency inversion
class serializer : public meta::object_container
{
public:
  virtual ~serializer() = 0 {};

  virtual void write(const string& file) const = 0;
  virtual bool read(const string& file) = 0;

};
}

