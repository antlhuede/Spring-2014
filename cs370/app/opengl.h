#pragma once
#include "GL\glew.h"
#include "GL\wglew.h"
#include <Windows.h>
#include <type_traits>
#include <cassert>

#define USE_FONTS 0
#define STRICT_OPENGL_4_0 1

namespace dit
{
  namespace opengl
  {
    
    template <class T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
    struct color_t
    {
      union
      {
        struct
        {
          const T r, g, b, a;
        };
        const T v[4];
      };

      color_t() = default;
      color_t(const color_t&) = default;
      color_t& operator=(const color_t&) = default;
      color_t(T r_, T g_, T b_, T a_) : r(r_), g(g_), b(b_), a(a_) {}
      color_t(std::initializer_list<T> values)
      {
        assert(values.size() == 4);
        T* walk = const_cast<T*>(v);
        for (auto it = values.begin(); it != values.end(); ++it, ++walk)
          *walk = *it;
      }
    };
    typedef color_t<unsigned char> coloruc;
    typedef color_t<float> colorf;
    
    struct palette
    {
      typedef coloruc color;

      static const color empty;
      static const color red;
      static const color green;
      static const color blue;
      static const color black;
      static const color white;
      static const color navy;
      static const color royal_blue;
      static const color slate_gray;
      static const color indian_red;
      static const color crimson;
      static const color pink;
      static const color hot_pink;
      static const color raspberry;
      static const color magenta;
      static const color purple;
      static const color indigo;
      static const color slate_blue;
      static const color steel_blue;
      static const color sky_blue;
      static const color sky_blue_darker;
      static const color dark_green;
      static const color lawn_green;
      static const color olive;
      static const color olive_drab;
      static const color yellow;
      static const color gold;
      static const color golden_rod;
      static const color orange;
      static const color brick;
      static const color dark_orange;
      static const color chocolate;
      static const color saddle_brown;
      static const color sienna;
      static const color burnt_sienna;
      static const color flesh;
      static const color ivory_black;
      static const color coral;
      static const color sepia;
      static const color salmon;

    protected:
      palette(void) = delete;
      palette& operator=(const palette& rhs) = delete;
      palette(const palette& rhs) = delete;
    };

    struct device 
    {
      device() = default;
      device(HWND hWnd_);
      ~device();
      device(const device&) = default;

      bool create(HWND hWnd_);
      void destroy();
      HWND hWnd = nullptr;
      HDC hdc = nullptr;
      HGLRC glrc = nullptr;
    };

    void BeginScene(device* pDevice, const colorf& clear_color);
    void EndScene(device* pDevice);

    void OrthoLens(GLsizei width, GLsizei height);
  }
}