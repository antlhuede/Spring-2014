///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form.h"

///////////////////////////////////////////////////////////////////////////
using namespace dit::cs370;

project1_wxui::project1_wxui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_image = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer1->Add( m_image, 1, wxEXPAND | wxALL, 0 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_command = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_command, 1, wxALL, 1 );
	
	m_enter = new wxButton( this, wxID_ANY, wxT("Enter"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer2->Add( m_enter, 0, wxALL, 0 );
	
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	m_menubar1 = new wxMenuBar( 0 );
	m_imageMenu = new wxMenu();
	wxMenuItem* m_loadImage;
	m_loadImage = new wxMenuItem( m_imageMenu, wxID_ANY, wxString( wxT("Load") ) + wxT('\t') + wxT("CTRL+L"), wxT("Load a .ppm format image"), wxITEM_NORMAL );
	m_imageMenu->Append( m_loadImage );
	
	wxMenuItem* m_saveImage;
	m_saveImage = new wxMenuItem( m_imageMenu, wxID_ANY, wxString( wxT("Save") ) + wxT('\t') + wxT("CTRL+S"), wxT("Save image to .ppm file"), wxITEM_NORMAL );
	m_imageMenu->Append( m_saveImage );
	
	m_menubar1->Append( m_imageMenu, wxT("Image") ); 
	
	m_interpolationMenu = new wxMenu();
	wxMenuItem* m_nearestNeighbor;
	m_nearestNeighbor = new wxMenuItem( m_interpolationMenu, wxID_ANY, wxString( wxT("Nearest Neighbor") ) + wxT('\t') + wxT("CTRL+N"), wxT("Enable Nearest Neighbor interpolation"), wxITEM_NORMAL );
	m_interpolationMenu->Append( m_nearestNeighbor );
	
	wxMenuItem* m_bilinear;
	m_bilinear = new wxMenuItem( m_interpolationMenu, wxID_ANY, wxString( wxT("Bilinear") ) + wxT('\t') + wxT("CTRL+B"), wxT("Enable Bilinear interpolation"), wxITEM_NORMAL );
	m_interpolationMenu->Append( m_bilinear );
	
	m_menubar1->Append( m_interpolationMenu, wxT("Interpolation") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_image->Connect( wxEVT_PAINT, wxPaintEventHandler( project1_wxui::OnDrawImage ), NULL, this );
	m_image->Connect( wxEVT_SIZE, wxSizeEventHandler( project1_wxui::OnResizeImage ), NULL, this );
	m_command->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( project1_wxui::OnCommandEnter ), NULL, this );
	this->Connect( m_loadImage->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( project1_wxui::OnLoadImage ) );
	this->Connect( m_saveImage->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( project1_wxui::OnSaveImage ) );
	this->Connect( m_nearestNeighbor->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( project1_wxui::OnEnableNearestNeighbor ) );
	this->Connect( m_bilinear->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( project1_wxui::OnEnableBilinear ) );
}

project1_wxui::~project1_wxui()
{
	// Disconnect Events
	m_image->Disconnect( wxEVT_PAINT, wxPaintEventHandler( project1_wxui::OnDrawImage ), NULL, this );
	m_image->Disconnect( wxEVT_SIZE, wxSizeEventHandler( project1_wxui::OnResizeImage ), NULL, this );
	m_command->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( project1_wxui::OnCommandEnter ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( project1_wxui::OnLoadImage ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( project1_wxui::OnSaveImage ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( project1_wxui::OnEnableNearestNeighbor ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( project1_wxui::OnEnableBilinear ) );
	
}
