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
    
    if (dit::opengl::CompileShader(std::string("shaders/test.frag"), &frag) == false)
      return false;
    if (dit::opengl::CompileShader(std::string("shaders/test.vert"), &vert) == false)
      return false;
    if (dit::opengl::CreateProgram(vert, frag, &program) == false)
      return false;

    return true;
  }

  GLuint frag = 0;
  GLuint vert = 0;
  GLuint program = 0;
private:
};

IMPLEMENT_APP(app)
