solution "CS370"
  language "C++"
  location ("build/")
 
  defines { "_CRT_SECURE_NO_WARNINGS", "_WINDOWS", "WIN32", "GLEW_STATIC" }

  configurations
  { 
    "Release",
    "Debug"
  }

configuration "Release"
  defines { "NDEBUG" }
  optimize "Speed"
  floatingpoint "fast"
  vectorextensions "SSE2"
  flags 
  {
    "WinMain",
    "Symbols",
    "NoMinimalRebuild",
    "NoEditAndContinue",
    "No64BitChecks",
    "Unicode"
  }

configuration "Debug"
  defines { "_DEBUG" }
  optimize "Off"
  floatingpoint "fast"
  vectorextensions "SSE2"
  flags 
  {
    "WinMain",
    "Symbols",
    "NoMinimalRebuild",
    "NoEditAndContinue",
    "No64BitChecks",
    "Unicode"
  }

  configuration { "vs2013" }
    debugdir "assets/"  
    buildoptions { "-wd4201 -wd4505" } -- nonstandard struct union


project "App"
  kind("WindowedApp")
  files { "app/**.*" }

  targetdir("bin/")

  includedirs { "./", "app/", 
                "extern/glew-1.10.0/include/", 
                "extern/wxWidgets-3.0.0/include/", "extern/wxWidgets-3.0.0/include/msvc/" }
  libdirs { "extern/wxWidgets-3.0.0/lib/vc_lib/", "extern/glew-1.10.0/lib/" }
  --linkoptions{"/ENTRY:WinMainCRTStartup"}

  links { "Project1" }
  links { "gdi32", "opengl32" }
  links { "winmm", "comctl32", "rpcrt4" }

  vpaths({ ["source"] = {"**.cpp", "**.c", "**.h"} })

  configuration "Release"
    targetname( "main_r" )
    links {"glew32s"}
    links {"wxbase30u", --"wxbase30u_net",-- "wxbase30u_xml", 
          --"wxmsw30u_adv",-- "wxmsw30u_aui", "wxmsw30u_gl", 
          --"wxmsw30u_media", "wxmsw30u_qa", "wxmsw30u_ribbon", 
          --"wxmsw30u_richtext", "wxmsw30u_stc", "wxmsw30u_webview",
           --"wxmsw30u_xrc", "wxmsw30u_propgrid", "wxmsw30u_html", 
           "wxmsw30u_core", "wxzlib",--"wxtiff", "wxjpeg", 
           "wxpng", --"wxexpat","wxregexu",  -- "wxscintilla"
           }

  configuration "Debug"
    targetname( "main_d" )
    links {"glew32sd"}
    links {"wxbase30ud", --"wxbase30ud_net",-- "wxbase30ud_xml", 
          --"wxmsw30ud_adv",-- "wxmsw30ud_aui", "wxmsw30ud_gl", 
          --"wxmsw30ud_media", "wxmsw30ud_qa", "wxmsw30ud_ribbon", 
          --"wxmsw30ud_richtext", "wxmsw30ud_stc", "wxmsw30ud_webview",
          -- "wxmsw30ud_xrc", "wxmsw30ud_propgrid", "wxmsw30ud_html", 
           "wxmsw30ud_core", "wxzlibd", --"wxtiffd", "wxjpegd", 
           "wxpngd", --"wxexpatd","wxregexud",  -- "wxscintillad"
           }

project "Project1"
  kind ("StaticLib")
  files{"project1/**.*"}

  vpaths({ ["source"] = {"**.cpp", "**.c", "**.h"} })

  targetdir("bin/projects/")

  includedirs{"project1/", "extern/wxWidgets-3.0.0/include/", "extern/wxWidgets-3.0.0/include/msvc/"}

  configuration "Release"
    targetname( "project1_r" )

  configuration "Debug"
    targetname( "project1_d" )
