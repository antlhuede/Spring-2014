solution "Google Test Framework"
  language "C++"
  location ("builds/google_test/")
 
  defines { "_CRT_SECURE_NO_WARNINGS", "_WINDOWS", "WIN32" }

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
    --"WinMain",
    "Symbols",
    "NoMinimalRebuild",
    "NoEditAndContinue",
    "No64BitChecks",
    --"Unicode"
  }

configuration "Debug"
  defines { "_DEBUG" }
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
  targetdir("bin/google_test/")
  buildoptions { "-wd4201 -wd4505" } -- nonstandard struct union

project "Google Test Framework"
  kind("StaticLib")
  files { "google_test/gmock/*", "google_test/gtest/*" }
  includedirs { "./", "google_test/", "google_test/gmock/", "google_test/gtest/" }

  vpaths({ ["source"] = {"**.cpp", "**.c", "**.h"} })

  configuration "Release"
    targetname( "google_test_r" )

  configuration "Debug"
    targetname( "google_test_d" )