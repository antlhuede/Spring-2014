#pragma once
#include "opengl.h"

using namespace dit::opengl;
namespace dit
{
  namespace cs370
  {
    class project1;
    
    class projects : public project1
    {
    public:
      projects(wxWindow* parent) 
        : project1(parent)
        , m_device(m_image->GetHandle()) {}

      // Virtual event handlers, overide them in your derived class
      virtual void OnDrawImage(wxPaintEvent& event) { BeginScene(&m_device, colorf(0.0f,0.0f,0.0f,0.0f)); EndScene(&m_device); event.Skip(); }
      virtual void OnResizeImage(wxSizeEvent& event) { OnResize(event.m_size.GetX(), event.m_size.GetY()); event.Skip(); }
      virtual void OnCommandEnter(wxCommandEvent& event) { event.Skip(); }
      virtual void OnLoadImage(wxCommandEvent& event) { event.Skip(); }
      virtual void OnSaveImage(wxCommandEvent& event) { event.Skip(); }
      virtual void OnEnableNearestNeighbor(wxCommandEvent& event) { event.Skip(); }
      virtual void OnEnableBilinear(wxCommandEvent& event) { event.Skip(); }

      device m_device;
      int image_width = 0;
      int image_height = 0;
    private:
      void OnResize(int width, int height) 
      {
        image_width = width; 
        image_height = height;
        OrthoLens(width, height);
      }
    };
  }
}