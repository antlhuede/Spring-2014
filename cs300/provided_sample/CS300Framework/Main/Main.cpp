///////////////////////////////////////////////////////////////////////////////
///
/// \file Main.cpp
/// Feel free to reorganize your application's logic
/// This is nothing more than an incredibly simple framework to get you started
/// 
/// Authors: Trevor Sundberg
/// Copyright 2013, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////

// Includes
#include <time.h>
#include "Application.hpp"
#include "AntTweakBar/AntTweakBar.h"
#include "OpenGL/glut.h"
#include "Support.hpp"

//-----------------------------------------------------------------------------
// Glut assumes everthing is global (no userdata for callbacks! ;-;)
Application* gApplication;
clock_t gLastTime = clock();

//-----------------------------------------------------------------------------
// Glut idle function where we update (glut will call this for us)
void Idle()
{
  // Determine the new clock ticks
  clock_t newTime = clock();

  // Get the difference between the new clock tick and the last clock tick
  clock_t ticks = newTime - gLastTime;

  // Divide out by tick frequency to get the frame time passed
  float frameTime = ticks / (float)CLOCKS_PER_SEC;

  // Update the last tick count
  gLastTime = newTime;

  // Now update the application with the frame time
  gApplication->Update(frameTime);

  // Tell glut to redisplay the last frame buffer
  glutPostRedisplay();
}

//-----------------------------------------------------------------------------
// Glut display function where we draw (glut will call this for us)
void Display()
{
  // Let the application draw and present to the screen
  gApplication->Draw();

  // Draw all the AntTweakBar windows / ui
  TwDraw();

  // Finally, present the back buffer to the screen
  glutSwapBuffers();
}

//-----------------------------------------------------------------------------
// Glut window resize/reshape function (glut will call this for us)
void Reshape(int width, int height)
{
  // Tell AntTweakBar that our window has been resized
  TwWindowSize(width, height);

  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if (height <= 0)
  {
    height = 1;
  }
  
  // Compute the aspect ratio as w/h
  float aspectRatio = width / (float)height;

  // Set the viewport to be the entire window
  glViewport(0, 0, width, height);
  
  // Let the application handle it's own resizing
  gApplication->Reshape(width, height, aspectRatio);
}

//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  // Starting width / height of the window
  const int WindowWidth = 800;
  const int WindowHeight = 600;

  // Initialize GLUT
  glutInit(&argc, argv);

  // Get how big our screen is that we're displaying the window on
  int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
  int screneHeight = glutGet(GLUT_SCREEN_HEIGHT);
  
  // Initialize the display mode to utilize double buffering, 4-channel framebuffer and depth buffer
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
  
  // Setup the window
  glutInitWindowSize(WindowWidth, WindowHeight);
  glutInitWindowPosition((screenWidth - WindowWidth) / 2, (screneHeight - WindowHeight) / 2);
  glutCreateWindow("CS300 Framework");
  
  // Assign callback functions
  glutDisplayFunc(Display);
  glutIdleFunc(Idle); 
  glutReshapeFunc(Reshape);

  // Setup all the open-gl states we want to use (ones that don't change in the lifetime of the application)
  // Note: These can be changed anywhere, but generally we don't change the back buffer color
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glCullFace(GL_FRONT);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  
  // Show the window that we just initailized
  glutShowWindow();

  // Initialize AntTweakBar
  TwInit(TW_OPENGL, NULL);
  TwWindowSize(WindowWidth, WindowHeight);

  // We redirect all GLUT functions to point at AntTweakBar's input
  // If we want to do our own input, we need to make our own callbacks
  // Where we can intercept the input and remove these
  glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
  glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
  glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
 
  // Send the 'glutGetModifers' function pointer to AntTweakBar
  TwGLUTModifiersFunc(glutGetModifiers);

  // Check to see if we had any errors
  PrintOpenGLErrors();

  // Create the application
  gApplication = new Application();

  // We leave the console up since it's a useful debugging tool
  printf("OpenGL Info:\n", glGetString(GL_VERSION));
  printf("   Version: %s\n", glGetString(GL_VERSION));
  printf("    Vendor: %s\n", glGetString(GL_VENDOR));
  printf("  Renderer: %s\n", glGetString(GL_RENDERER));

  // Now let glut takeover the main loop (we'll get callbacks in our display / idle functions)
  glutMainLoop();

  // Glut never returns here, so I'm not sure what we need to do to be proper...
  delete gApplication;
  return 0;
}
