#include <wx/wx.h>
#include "opengl.h"
#include "project1.h"

namespace dit { namespace cs370 {

project1::project1(wxWindow* parent)
  : project1_wxui(parent)
{
  m_device.create(m_image->GetHandle());
}

bool project1::CompileShaders(const std::string& vertex, const std::string& fragment)
{
  if (m_device.hWnd == nullptr && 
      m_device.create(m_image->GetHandle()) == false)
    return false;

  if (opengl::CompileShader(vertex, &vertex_shader) == false)
    return false;
  if (opengl::CompileShader(fragment, &fragment_shader) == false)
    return false;
  if (opengl::CreateProgram(vertex_shader, fragment_shader, &program) == false)
    return false;

  return true;
}

// Virtual event handlers, overide them in your derived class
void project1::OnDrawImage(wxPaintEvent& event) 
{ 
  BeginScene(&m_device, opengl::colorf(0.0f, 0.0f, 0.0f, 0.0f)); 
  EndScene(&m_device); 
  
  //event.Skip(); 
}
void project1::OnResizeImage(wxSizeEvent& event) 
{ 
  OnResize(event.m_size.GetX(), event.m_size.GetY()); 
  
  //event.Skip(); 
}
void project1::OnCommandEnter(wxCommandEvent& event) { event.Skip(); }
void project1::OnLoadImage(wxCommandEvent& event) { event.Skip(); }
void project1::OnSaveImage(wxCommandEvent& event) { event.Skip(); }
void project1::OnEnableNearestNeighbor(wxCommandEvent& event) { event.Skip(); }
void project1::OnEnableBilinear(wxCommandEvent& event) { event.Skip(); }

void project1::OnResize(int width, int height)
{
  image_width = width;
  image_height = height;
  opengl::OrthoLens(width, height);
}

}}