#pragma once

#include "serializers\json\json.h"
#include "serializers\json\value.h"

namespace serializers {
class JSonSerializer : public meta::serializer
{
public:
  JSonSerializer() = default;
  ~JSonSerializer() = default;
  JSonSerializer(JSonSerializer&&) = delete;
  JSonSerializer(const JSonSerializer&) = delete;
  JSonSerializer& operator=(const JSonSerializer&) = delete;

  virtual void write(const string& file) const;
  virtual bool read(const string& file);

  virtual bool ReadFile(const string& file);
  virtual bool BeginWrite(const string& file);
  virtual void EndWrite();

  virtual bool ReadBool(const string& name) const;
  virtual int ReadInteger(const string& name) const;
  virtual float ReadFloat(const string& name) const;
  virtual double ReadDouble(const string& name) const;
  virtual const string ReadString(const string& name) const;
  virtual const meta::variant ReadVariable(const string& name) const;

  virtual void WriteBool(const string& name, bool value);
  virtual void WriteInteger(const string& name, int value);
  virtual void WriteFloat(const string& name, float value);
  virtual void WriteDouble(const string& name, double value);
  virtual void WriteString(const string& name, const string& value);

  virtual void BeginObject(const string& name, const meta::type* type);
  virtual void EndObject();

  virtual void BeginArray(const string& name);
  virtual void EndArray();

  virtual const string ReadEnum(const string& name) const;
  virtual void WriteEnum(const string& name, const meta::type* type, const string& value);

private:
  void initialize(const string& file, state s);
  void destroy();

  string m_fileName = "";
  state m_state;

  shared_ptr<Json::Value> m_root;
  std::vector<Json::Value*> m_parents;
  Json::Value* m_current;
};
}
