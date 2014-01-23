#include "wx/wx.h"
#include "project1/form.h"
#include "bind_projects.h"
#include "opengl.h"

class app : public wxApp
{
public:
  virtual bool OnInit()
  {
    if (!wxApp::OnInit())
      return false;

    wxInitAllImageHandlers();

    dit::cs370::projects* projects = new dit::cs370::projects(nullptr);
    projects->Show();
    
    return true;
  }

private:
};

IMPLEMENT_APP(app)
