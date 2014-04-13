
class PlugIn;

//
//#ifdef PLUGIN_EXPORTS
//#define MAIN_DECL __declspec(dllimport)
//#else
//#define MAIN_DECL __declspec(dllexport)
//#endif


class PlugInManager
{
public:
//  std::map<std::string, PlugIn*> PlugIns;

  void AddPlugIn(PlugIn* plugIn);

};