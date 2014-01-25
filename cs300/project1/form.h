///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __FORM_H__
#define __FORM_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

namespace dit
{
	namespace cs300
	{
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class project1_wxui
		///////////////////////////////////////////////////////////////////////////////
		class project1_wxui : public wxFrame 
		{
			private:
			
			protected:
				wxTextCtrl* m_command;
				wxButton* m_enter;
				wxMenuBar* m_menubar1;
				wxMenu* m_imageMenu;
				wxMenu* m_interpolationMenu;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnDrawImage( wxPaintEvent& event ) { event.Skip(); }
				virtual void OnResizeImage( wxSizeEvent& event ) { event.Skip(); }
				virtual void OnCommandEnter( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnLoadImage( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnSaveImage( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnEnableNearestNeighbor( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnEnableBilinear( wxCommandEvent& event ) { event.Skip(); }
				
			
			public:
				wxPanel* m_image;
				
				project1_wxui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("CS370 Project 1 - Image Interpolation"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 557,447 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
				
				~project1_wxui();
			
		};
		
	} // namespace cs300
} // namespace dit

#endif //__FORM_H__
