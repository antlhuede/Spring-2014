solution "Programming"
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


project "Test"
  kind("ConsoleApp")
  files { "meta_test/**.*" }

  includedirs { "./", "meta/", "meta_test/", "gmock/", "gtest/", "json/" }
  libdirs { "bin/" }
  --linkoptions{"/ENTRY:WinMainCRTStartup"}

  links { "Meta", "Serializers", "Google" }
  links { "gdi32", "opengl32" }
  links { "winmm", "comctl32", "rpcrt4" }

  vpaths({ ["source"] = {"**.cpp", "**.c", "**.h"} })

  configuration "Release"
    targetname( "meta_test_r" )
    
  configuration "Debug"
    targetname( "meta_test_d" )

project "Meta"
  kind("StaticLib")
  files { "meta/**.*" }
  includedirs { "./", "gmock/", "gtest/", "meta/", "json/" }
  vpaths({ ["source"] = {"**.cpp", "**.c", "**.h"} })

  configuration "Release"
    targetname( "meta_r" )
    
  configuration "Debug"
    targetname( "meta_d" )


project "Serializers"
  kind("StaticLib")
  files { "json/*", "xml/*" }
  includedirs { "./", "json/", "xml/" }

  vpaths({ ["json"] = {"json/**.cpp", "json/**.c", "json/**.h"} })
  vpaths({ ["xml"] = {"xml/**.cpp", "xml/**.c", "xml/**.h"} })

  configuration "Release"
    targetname( "serialize_r" )
    
  configuration "Debug"
    targetname( "serialize_d" )


project "Google"
  kind("StaticLib")
  files { "gmock/*", "gtest/*" }
  includedirs { "./", "gmock/", "gtest/" }

  vpaths({ ["source"] = {"**.cpp", "**.c", "**.h"} })

  configuration "Release"
    targetname( "test_r" )

  configuration "Debug"
    targetname( "test_d" )