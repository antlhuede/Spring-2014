#pragma once

namespace meta {
class property
{
public:
  property(const string& name, const function& get, const function& set)
    : m_name(name)//, m_get(get), m_set(set) 
  {
    assert(get.traits().classType == set.traits().classType);
    
    assert(get.traits().numArguments == 1);
    assert(set.traits().numArguments == 1);
    assert(get.traits().args[0].type == set.traits().args[0].type);

    assert(get.traits().args[0].isPointer == true);
    assert(set.traits().args[0].isReference == true);

    m_classType = get.traits().classType;
    m_type = get.traits().args[0].type;
  }

  const string& name() const { return m_name; }
  const type* type() const { return m_type; }

  //const function& get() const { return m_get; }
  //const function& set() const { return m_set; }
private:
  string m_name = "uninitialized_property";
  const ::meta::type* m_type;
  const ::meta::type* m_classType;
  //const function m_get;
  //const function m_set;
};
}