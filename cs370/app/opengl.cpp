#include "opengl.h"
#include <cassert>
#include <cstdio>
#include <vector>

namespace dit
{
  namespace opengl
  {
  #if USE_FONTS
    namespace { GLuint font_texture = 0; }
  #endif
    const coloruc palette::empty(0, 0, 0, 0);
    const coloruc palette::red(255, 0, 0, 255);
    const coloruc palette::green(0, 255, 0, 255);
    const coloruc palette::blue(0, 0, 255, 255);
    const coloruc palette::black(0, 0, 0, 255);
    const coloruc palette::white(255, 255, 255, 255);
    const coloruc palette::navy(0, 0, 128, 255);
    const coloruc palette::royal_blue(65, 105, 225, 255);
    const coloruc palette::slate_gray(112, 128, 144, 225);
    const coloruc palette::indian_red(176, 23, 31, 255);
    const coloruc palette::crimson(220, 20, 60, 255);
    const coloruc palette::pink(255, 192, 203, 255);
    const coloruc palette::hot_pink(255, 105, 180, 255);
    const coloruc palette::raspberry(135, 38, 87, 255);
    const coloruc palette::magenta(255, 0, 255, 255);
    const coloruc palette::purple(128, 0, 128, 255);
    const coloruc palette::indigo(75, 0, 30, 255);
    const coloruc palette::slate_blue(106, 90, 205, 255);
    const coloruc palette::steel_blue(70, 30, 180, 255);
    const coloruc palette::sky_blue(135, 206, 235, 255);
    const coloruc palette::sky_blue_darker(108, 166, 205, 255);
    const coloruc palette::dark_green(0, 100, 0, 255);
    const coloruc palette::lawn_green(124, 252, 0, 255);
    const coloruc palette::olive(128, 128, 0, 255);
    const coloruc palette::olive_drab(107, 142, 35, 255);
    const coloruc palette::yellow(255, 255, 0, 255);
    const coloruc palette::gold(255, 215, 0, 255);
    const coloruc palette::golden_rod(218, 165, 32, 255);
    const coloruc palette::orange(255, 165, 0, 255);
    const coloruc palette::brick(156, 102, 31, 255);
    const coloruc palette::dark_orange(255, 127, 0, 255);
    const coloruc palette::chocolate(210, 105, 30, 255);
    const coloruc palette::saddle_brown(139, 69, 19, 255);
    const coloruc palette::sienna(160, 82, 45, 255);
    const coloruc palette::burnt_sienna(138, 54, 15, 255);
    const coloruc palette::flesh(255, 125, 64, 255);
    const coloruc palette::ivory_black(41, 36, 33, 255);
    const coloruc palette::coral(255, 127, 80, 255);
    const coloruc palette::sepia(94, 38, 18, 255);
    const coloruc palette::salmon(250, 128, 114, 255);

    bool CompileShader(const std::string& source_file, GLuint* shader_id)
    {
      FILE* pFile = fopen(source_file.c_str(), "rt");
      char line_buffer[256] = {0};
      std::string source;
      while (std::fgets(line_buffer, 256, pFile))
      {
        source += line_buffer;
      }
      fclose(pFile);
      
      GLenum shaderType = GL_FRAGMENT_SHADER;
      if (source_file.find(".v") != std::string::npos)
        shaderType = GL_VERTEX_SHADER;
      GLuint& id = *shader_id;
      id = glCreateShader(shaderType);
      const GLchar* source_array[] = { source.c_str() };
      GLsizei array_len = sizeof(source_array) / sizeof(GLchar*);
      glShaderSource(id, array_len, source_array, nullptr);
      glCompileShader(id);

      //source retrieved from http://www.opengl.org/wiki/Shader_Compilation
      GLint isCompiled = 0;
      glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
      if (isCompiled == GL_FALSE)
      {
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);

        //Use the infoLog as you see fit.
        
        FILE* pErr = fopen("shader.log", "wt");
        assert(pErr);

        fprintf(pErr, "%s", &infoLog[0]);
        fclose(pErr);
        pErr = nullptr;

        //Exit with failure.
        glDeleteShader(id); //Don't leak the shader.
        id = 0;

        return false;
      }

      return true;
    }
    bool CreateProgram(GLuint vertex_shader, GLuint fragment_shader, GLuint* program_id)
    {
      //source retrieved from http://www.opengl.org/wiki/Shader_Compilation
      //Vertex and fragment shaders are successfully compiled.
      //Now time to link them together into a program.
      //Get a program object.
      GLuint id = glCreateProgram();

      //Attach our shaders to our program
      glAttachShader(id, vertex_shader);
      glAttachShader(id, fragment_shader);

      //Link our program
      glLinkProgram(id);

      //Note the different functions here: glGetProgram* instead of glGetShader*.
      GLint isLinked = 0;
      glGetProgramiv(id, GL_LINK_STATUS, &isLinked);
      if (isLinked == GL_FALSE)
      {
        GLint maxLength = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(id, maxLength, &maxLength, &infoLog[0]);

        //Always detach shaders after a successful link.
        glDetachShader(id, vertex_shader);
        glDetachShader(id, fragment_shader);

        //We don't need the program anymore.
        glDeleteProgram(id);
        //Don't leak shaders either.
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        //Use the infoLog as you see fit.
        assert(false);
        //In this simple program, we'll just leave
        return false;
      }

      //Always detach shaders after a successful link.
      glDetachShader(id, vertex_shader);
      glDetachShader(id, fragment_shader);

      return true;
    }
    device::device(HWND hWnd_) : hWnd(nullptr), hdc(nullptr), glrc(nullptr)
    {
      assert(create(hWnd_));
    }
    device::~device()
    {
      destroy();
    }
    bool device::create(HWND hWnd_)
    {
      hWnd = hWnd_;
      hdc = GetDC(hWnd_);
      assert(hWnd != nullptr);
      assert(hdc != nullptr);
      assert(glrc == nullptr);
      // get the device context
      //if( ( deviceContext_ = GetDC( hWnd ) ) == NULL ) return;

      PIXELFORMATDESCRIPTOR pfd;
      memset(&pfd, 0, sizeof(pfd));

      pfd.nSize = sizeof(pfd);
      pfd.nVersion = 1;
      pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
      pfd.iPixelType = PFD_TYPE_RGBA;
      pfd.cColorBits = 32;
      pfd.cDepthBits = 24;
      pfd.iLayerType = PFD_MAIN_PLANE;
      pfd.cStencilBits = 8;

      int iFormat = ChoosePixelFormat(hdc, &pfd);
      SetPixelFormat(hdc, iFormat, &pfd);

      // create OpenGL context
      if ((glrc = wglCreateContext(hdc)) == nullptr)
      {
        ReleaseDC(hWnd, hdc);
        hWnd = nullptr;
        hdc = nullptr;
        glrc = nullptr;
        return false;
      }

      // set the current openGL render context
      wglMakeCurrent(hdc, glrc);

      // make sure GLEW initialized correctly
      assert(glewInit() == GLEW_OK);

      // make sure that there is OpenGL 4.0 support
      // * need at LEAST 4.0 for shaders
      assert(GLEW_VERSION_4_0);

      // create an openGL render context
      if (wglCreateContextAttribsARB)
      {
        int contextAttrs[] =
        {
          // request for v4.0 context
          WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
          WGL_CONTEXT_MINOR_VERSION_ARB, 0,

          WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
          0
        };

        HGLRC prevRenderContext = glrc;

        // create an openGL render context
        if ((glrc = wglCreateContextAttribsARB(hdc, 0, contextAttrs)) == nullptr)
        {
        #if STRICT_OPENGL_4_0
          assert(false);
        #endif
          // revert to the previous one
          glrc = prevRenderContext;
        }
      }

      // set the current openGL render context
      wglMakeCurrent(hdc, glrc);
      assert(glrc != nullptr);

    #if USE_FONTS
      //generate font texture, 
      //http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01
      glActiveTexture(GL_TEXTURE0);
      glGenTextures(1, &font_texture);
      glBindTexture(GL_TEXTURE_2D, font_texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    #endif

      return true;
    }
    void device::destroy()
    {
      assert(hdc != nullptr);
      assert(glrc != nullptr);
      assert(hWnd != nullptr);

    #if USE_FONTS
      glDeleteTextures(1, &font_texture);
      font_texture = 0;
    #endif

      if (glrc) wglDeleteContext(glrc);
      if (hdc) ReleaseDC(hWnd, hdc);

      glrc = nullptr;
      hdc = nullptr;
      hWnd = nullptr;
    }

    void BeginScene(device* pDevice, const colorf& clear_color)
    {
      assert(pDevice);
      wglMakeCurrent(pDevice->hdc, pDevice->glrc);

      // push all current attributes to the stack
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

      glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
      glClear(GL_COLOR_BUFFER_BIT);
      glColor4ub(palette::white.r, palette::white.g, palette::white.b, palette::white.a);
    }
    void EndScene(device* pDevice)
    {
      glDisable(GL_BLEND);
      glDisable(GL_LINE_SMOOTH);

      glPopClientAttrib();
      glPopAttrib();

      glFinish();
      SwapBuffers(pDevice->hdc);
    }
    void OrthoLens(GLsizei width, GLsizei height)
    {
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      //left right bottom top znear zfar
      glOrtho(0, (GLdouble)width, (GLdouble)height, 0, -1, 1);
      glViewport(0, 0, width, height);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glMatrixMode(GL_TEXTURE);
      glLoadIdentity();

      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glDisable(GL_DEPTH_TEST);
      glDisable(GL_STENCIL_TEST);
    }
  }
}