#include "meta\meta.h"
#include "xml\tinyxml2.h"

namespace meta
{

void XmlSerializer::set(const hash_map<string, xml::XMLNode*>& variables)
{
  m_loadedVariables = variables;
}
bool XmlSerializer::add(const string& name, const meta::type& t, const void* obj)
{
  if (has(name) == true)
    return false;

  xml::XMLElement* node = recursive_add(m_doc.NewElement(name.c_str()), t, obj);
  m_doc.InsertEndChild(node);
  m_loadedVariables[name] = node;

  return true;
}
void XmlSerializer::write(ostream& stream)
{
  xml::XMLPrinter printer;
  m_doc.Print(&printer);
  stream << printer.CStr();
  //m_root->Accept(&printer);
  //stream << printer.CStr();
}
bool XmlSerializer::read(istream& stream)
{
    
  return false;
}
bool XmlSerializer::has(const string& var)
{
  return (m_loadedVariables.find(var) != m_loadedVariables.end());
}
bool XmlSerializer::get(const string& var, const meta::type& t, void* obj)
{
  return false;
}
void XmlSerializer::clear()
{
}

void XmlSerializer::recursive_read(const xml::XMLNode* node, const meta::type& type, void* obj)
{
}

xml::XMLElement* XmlSerializer::recursive_add(xml::XMLElement* node, const meta::type& t, const void* obj)
{
  const vector<const meta::field*>& fields = t.fields();

  node->SetAttribute("type", t.name().c_str());

  if (fields.size())
  {
    xml::XMLElement* xml_fields = m_doc.NewElement("fields");
    
    for (size_t i = 0; i < fields.size(); ++i)
      xml_fields->InsertEndChild(recursive_add(m_doc.NewElement(fields[i]->name().c_str()), fields[i]->type(), fields[i]->member_ptr(obj))); //value["fields"][fields[i]->name()] = recursive_add(fields[i]->type(), fields[i]->member_ptr(obj));
    node->InsertEndChild(xml_fields);
  }
  else
  {
    if (t.id() == meta::typeof<int>().id())
      node->SetAttribute("value", meta::converter::toInt(t, obj));
    else if (t.id() == meta::typeof<bool>().id())
      node->SetAttribute("value", meta::converter::toBool(t, obj));// value = meta::converter::toBool(t, obj);
    else if (t.id() == meta::typeof<float>().id())
      node->SetAttribute("value", meta::converter::toFloat(t, obj));//value = meta::converter::toFloat(t, obj);
    else if (t.id() == meta::typeof<double>().id())
      node->SetAttribute("value", meta::converter::toDouble(t, obj));//value = meta::converter::toDouble(t, obj);
    else if (t.id() == meta::typeof<string>().id())
      node->SetAttribute("value", meta::converter::toString(t, obj).c_str());//value = meta::converter::toString(t, obj);
    else
      node->SetAttribute("value", t.to_string(obj).c_str());//value = t.to_string(obj);
  }

  return node;
}

}