///////////////////////////////////////////////////////////////////////////////
///
/// \file Application.cpp
/// Feel free to reorganize your application's logic
/// This is nothing more than an incredibly simple framework to get you started
/// 
/// Authors: Trevor Sundberg
/// Copyright 2013, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////

// Includes
#include "Application.hpp"
#include "Main/Support.hpp"

//-----------------------------------------------------------------------------
Application::Application()
{
  mRotation = 0.0f;
  mLightPosition = Vector3(0, 5, 0);
  mLightAmbient  = Vector4(1, 1, 1, 1);
  mLightDiffuse  = Vector4(1, 1, 1, 1);
  mLightSpecular = Vector4(1, 1, 1, 1);


  mBar = TwNewBar("Application");

  TwAddVarRW(mBar, "Rotation", TW_TYPE_FLOAT, &mRotation, "");
  TwAddVarRW(mBar, "LightPosition.X", TW_TYPE_FLOAT, &mLightPosition.x, "");
  TwAddVarRW(mBar, "LightPosition.Y", TW_TYPE_FLOAT, &mLightPosition.y, "");
  TwAddVarRW(mBar, "LightPosition.Z", TW_TYPE_FLOAT, &mLightPosition.z, "");
  TwAddVarRW(mBar, "LightAmbient", TW_TYPE_COLOR4F, &mLightAmbient, "");
  TwAddVarRW(mBar, "LightDiffuse", TW_TYPE_COLOR4F, &mLightDiffuse, "");
  TwAddVarRW(mBar, "LightSpecular", TW_TYPE_COLOR4F, &mLightSpecular, "");
  TwAddButton(mBar, "Reload Shaders", OnReloadShaders, NULL, "");
}

//-----------------------------------------------------------------------------
Application::~Application()
{
}

//-----------------------------------------------------------------------------
void Application::Update(float frameTime)
{
}

//-----------------------------------------------------------------------------
void Application::Draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  glLightfv(GL_LIGHT0, GL_POSITION, mLightPosition.ToFloats());
  glLightfv(GL_LIGHT0, GL_AMBIENT,  mLightAmbient.ToFloats());
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  mLightDiffuse.ToFloats());
  glLightfv(GL_LIGHT0, GL_SPECULAR, mLightSpecular.ToFloats());
  glEnable(GL_LIGHT0);

  glLoadIdentity();
  gluLookAt(0.0f, 0.0f, 5.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f);

  glRotatef(mRotation, 0.0f, 1.0f, 0.0f);

  glutSolidTeapot(1.25f);
  //glutSolidTorus(1.0, 2.0, 30, 30);

  PrintOpenGLErrors();
}

//-----------------------------------------------------------------------------
void Application::Reshape(int width, int height, float aspectRatio)
{
  // Use the Projection Matrix
  glMatrixMode(GL_PROJECTION);

  // Load the identity matrix over the top of the current projection
  glLoadIdentity();
  
  // Set the correct perspective
  gluPerspective(45.0f, aspectRatio, 0.1f, 1000.0f);
  
  // Get back to the Modelview
  glMatrixMode(GL_MODELVIEW);
}

//-----------------------------------------------------------------------------
void Application::OnReloadShaders(void* selfPointer)
{
  // Get our application (it's basically our this pointer!)
  Application* self = (Application*)selfPointer;

  Error("The OnReloadShader function must be implemented by you");
}