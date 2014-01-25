#pragma once
#include "form.h"

namespace dit { namespace cs300 {

class project1_wxui;

class project1 : public project1_wxui
{
public:
  project1(wxWindow* parent);

  bool CompileShaders(const std::string& vertex, const std::string& fragment);

  // Virtual event handlers, overide them in your derived class
  virtual void OnDrawImage(wxPaintEvent& event);
  virtual void OnResizeImage(wxSizeEvent& event);
  virtual void OnCommandEnter(wxCommandEvent& event);
  virtual void OnLoadImage(wxCommandEvent& event);
  virtual void OnSaveImage(wxCommandEvent& event);
  virtual void OnEnableNearestNeighbor(wxCommandEvent& event);
  virtual void OnEnableBilinear(wxCommandEvent& event);

  opengl::device m_device;
  int image_width = 0;
  int image_height = 0;

  GLuint fragment_shader = 0;
  GLuint vertex_shader = 0;
  GLuint program = 0;

private:
  void OnResize(int width, int height);
};
}}