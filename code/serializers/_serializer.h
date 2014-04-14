#pragma once

namespace serializers
{
template <class ContainerPolicy = meta::object_container>
class serializer : public ContainerPolicy
{
public:
  virtual ~serializer() = 0 {};

  virtual void write(const string& file) const = 0;
  virtual bool read(const string& file) = 0;
};

typedef serializer<meta::object_container> base_serializer;
typedef base_serializer* base_serializer_ptr;
}

