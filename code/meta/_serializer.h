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

  virtual bool ReadFile(const string& file) = 0;
  virtual bool BeginWrite(const string& file) = 0;
  virtual void EndWrite() = 0;

  virtual bool ReadBool(const string& name) const = 0;
  virtual int ReadInteger(const string& name) const = 0;
  virtual float ReadFloat(const string& name) const = 0;
  virtual double ReadDouble(const string& name) const = 0;
  virtual const string ReadString(const string& name) const = 0;
  virtual const variant ReadVariable(const string& name) const = 0;
  
  virtual const variant ReadEnum(const string& name) const = 0;
  virtual void WriteEnum(const string& name, const variant_ref& value) = 0;

  virtual void WriteBool(const string& name, bool value) = 0;
  virtual void WriteInteger(const string& name, int value) = 0;
  virtual void WriteFloat(const string& name, float value) = 0;
  virtual void WriteDouble(const string& name, double value) = 0;
  virtual void WriteString(const string& name, const string& value) = 0;
  
  virtual void BeginObject(const string& name, const type* type) = 0;
  virtual void EndObject() = 0;

  virtual void BeginArray(const string& name) = 0;
  virtual void EndArray() = 0;
};
}

