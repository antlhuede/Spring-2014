#pragma once

#include "serializers\tinyxml2.h"

namespace serializers {
namespace xml = ::tinyxml2;

class XMLSerializer : public meta::serializer
{
public:
  XMLSerializer() = default;
  ~XMLSerializer() = default;
  XMLSerializer(XMLSerializer&&) = delete;
  XMLSerializer(const XMLSerializer&) = delete;
  XMLSerializer& operator=(const XMLSerializer&) = delete;

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

  virtual const meta::variant ReadEnum(const string& name) const;
  virtual void WriteEnum(const string& name, const meta::variant_ref& value);

private:
  shared_ptr<xml::XMLDocument> m_doc = nullptr;
  xml::XMLNode* m_current = nullptr;
  string m_fileName = "";
  enum state { e_None, e_Read, e_Write };
  state m_state;
  shared_ptr<xml::XMLDocument> build_xml_document() const;
  xml::XMLElement* construct_xml_element(xml::XMLDocument* doc, const string& name, const meta::type* type, const void* obj) const;
  void construct_object(const xml::XMLElement* element, const meta::type* type, void* obj) const;
};
}
