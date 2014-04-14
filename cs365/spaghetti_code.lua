solution "Editor"
  language "C++"
  location ("build/")
 
  defines { "_CRT_SECURE_NO_WARNINGS", "_WINDOWS", "WIN32" }

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
    --"Unicode"
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
    --"Unicode"
  }

  configuration { "vs2013" }
    debugdir "assets/"  
    targetdir("bin/")
    buildoptions { "-wd4201 -wd4505" } -- nonstandard struct union

project "Spaghetti Code"
  kind("ConsoleApp")
  files { "spaghetti_code/**.*" }

  includedirs { "./", "spaghetti_code/", "gmock/", "gtest/" }
  libdirs { "bin/" }
  --linkoptions{"/ENTRY:WinMainCRTStartup"}

  links { "Google Test Framework" }
  links { "gdi32", "opengl32" }
  links { "winmm", "comctl32", "rpcrt4" }

  vpaths({ ["source"] = {"**.cpp", "**.c", "**.h"} })

  configuration "Release"
    targetname( "spaghetti_code_r" )
    
  configuration "Debug"
    targetname( "spaghetti_code_d" )


project "Google Test Framework"
  kind("StaticLib")
  files { "gmock/*", "gtest/*" }
  includedirs { "./", "gmock/", "gtest/" }

  vpaths({ ["source"] = {"**.cpp", "**.c", "**.h"} })

  configuration "Release"
    targetname( "google_test_r" )

  configuration "Debug"
    targetname( "google_test_d" )