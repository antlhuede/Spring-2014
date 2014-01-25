///////////////////////////////////////////////////////////////////////////////
///
/// \file Application.hpp
/// Feel free to reorganize your application's logic
/// This is nothing more than an incredibly simple framework to get you started
/// 
/// Authors: Trevor Sundberg
/// Copyright 2013, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once

// Includes
#include <stdlib.h>
#include "OpenGL/glut.h"
#include "AntTweakBar/AntTweakBar.h"
#include "Math/Math.hpp"
#include "Math/Utilities.hpp"

// Using directives
using Math::Vector3;
using Math::Vector4;

// The main flow behind your application
class Application
{
public:
  // Constructor
  Application();

  // Destructor
  ~Application();

  // This function gets called every frame 
  void Update(float frameTime);

  // Draw the application in its current state
  void Draw();

  // Reshape our projection matrix since the window size changed
  void Reshape(int width, int height, float aspectRatio);

  // Occurs when the user presses the 'reload shaders' button
  static void TW_CALL OnReloadShaders(void* selfPointer);

private:

  // The ui that represents our application
  TwBar* mBar;

  float mRotation;

  Vector3 mLightPosition;
  Vector4 mLightAmbient;
  Vector4 mLightDiffuse;
  Vector4 mLightSpecular;
};
