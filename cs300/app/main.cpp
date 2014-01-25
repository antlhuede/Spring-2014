#include "bind_projects.h"

template <class Project>
class App : public wxApp
{
public:
  virtual bool OnInit()
  {
    if (!wxApp::OnInit())
      return false;

    wxInitAllImageHandlers();

    Project* projects = new Project(nullptr);
    projects->Show();
    
    return true;
  }
};

IMPLEMENT_APP(App<dit::cs300::project1>)
