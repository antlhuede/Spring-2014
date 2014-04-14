solution "Editor"
  language "C++"
  location ("builds/editor/")
 
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
    debugdir("assets/")
    targetdir("bin/editor/")
    buildoptions { "-wd4201 -wd4505" } -- nonstandard struct union

project "Test Meta"
  kind("ConsoleApp")
  files { "test_meta/**.*" }

  includedirs { "./", "meta/", "test_meta/", "google_test/gmock/", "google_test/gtest/", "serializers/" }
  libdirs { "bin/editor/", "bin/google_test/" }
  --linkoptions{"/ENTRY:WinMainCRTStartup"}

  links { "Game", "Meta", "Serializers" }
  links { "gdi32", "opengl32" }
  links { "winmm", "comctl32", "rpcrt4" }

  vpaths({ ["source"] = {"**.cpp", "**.c", "**.h"} })

  configuration "Release"
    targetname( "test_meta_r" )
    links { "google_test_r" }
    
  configuration "Debug"
    targetname( "test_meta_d" )
    links { "google_test_d" }


project "Game"
  kind("StaticLib")
  files { "game/**.*" }
  includedirs { "./", "game/", "$(DXSDK_DIR)/Include/" }
  vpaths({ ["source"] = {"**.cpp", "**.c", "**.h"} })

  configuration "Release"
    targetname( "game_r" )
    
  configuration "Debug"
    targetname( "game_d" )


project "Meta"
  kind("StaticLib")
  files { "meta/**.*" }
  includedirs { "./", "gmock/", "gtest/", "meta/", "serializers/" }
  vpaths({ ["source"] = {"**.cpp", "**.c", "**.h"} })

  configuration "Release"
    targetname( "meta_r" )
    
  configuration "Debug"
    targetname( "meta_d" )


project "Serializers"
  kind("StaticLib")
  files { "serializers/*" }
  includedirs { "./", "serializers/" }

  vpaths({ ["serializers"] = {"serializers/**.cpp", "serializers/**.c", "serializers/**.h"} })

  configuration "Release"
    targetname( "serializers_r" )
    
  configuration "Debug"
    targetname( "serializers_d" )
