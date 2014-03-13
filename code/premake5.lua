solution "Programming"
  language "C++"
  location ("build/")
 
  defines { "_CRT_SECURE_NO_WARNINGS", "_WINDOWS", "WIN32", "GLEW_STATIC" }

  configurations
  { 
    "Release",
    "Debug"
  }

configuration "Release"
  defines { "NDEBUG", "JSON_LESS_MEMORY" }
  optimize "Speed"
  floatingpoint "fast"
  vectorextensions "SSE2"
  flags 
  {
    --"WinMain",
    "Symbols",
    "NoMinimalRebuild",
    "NoEditAndContinue",
    "No64BitChecks",
    "Unicode"
  }

configuration "Debug"
  defines { "_DEBUG", "JSON_DEBUG" }
  optimize "Off"
  floatingpoint "fast"
  vectorextensions "SSE2"
  flags 
  {
    --"WinMain",
    "Symbols",
    "NoMinimalRebuild",
    "NoEditAndContinue",
    "No64BitChecks",
    "Unicode"
  }

  configuration { "vs2013" }
    debugdir "assets/"  
    targetdir("bin/")
    buildoptions { "-wd4201 -wd4505" } -- nonstandard struct union


project "Meta"
  kind("ConsoleApp")
  files { "meta/**.*" }

  includedirs { "./", "meta/", "json/" }
  libdirs { "bin/" }
  --linkoptions{"/ENTRY:WinMainCRTStartup"}

  links { "JSON" }
  links { "gdi32", "opengl32" }
  links { "winmm", "comctl32", "rpcrt4" }

  vpaths({ ["source"] = {"**.cpp", "**.c", "**.h"} })

  configuration "Release"
    targetname( "meta_r" )
    
  configuration "Debug"
    targetname( "meta_d" )


project "JSON"
  kind("StaticLib")
  files { "json/**.*" }
  includedirs { "json/" }

  vpaths({ ["source"] = {"**.cpp", "**.c", "**.h"} })

  configuration "Release"
    targetname( "json_r" )
    
  configuration "Debug"
    targetname( "json_d" )