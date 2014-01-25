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

  GLuint frag = 0;
  GLuint vert = 0;
  GLuint program = 0;
private:
};

IMPLEMENT_APP(App<dit::cs370::project1>)
