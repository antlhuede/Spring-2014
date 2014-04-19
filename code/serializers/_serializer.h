#pragma once

namespace serializers
{
//container policy necessary for dependency inversion
class serializer : public meta::object_container
{
public:
  virtual ~serializer() = 0 {};

  virtual void write(const string& file) const = 0;
  virtual bool read(const string& file) = 0;

};

typedef serializer base_serializer;
typedef base_serializer* base_serializer_ptr;
}

