workspace "PingPlotter"
    platforms "x64"
    architecture "x64"
    startproject "PingPlotter"
    
    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["SDL"] = "%{prj.name}/vendor/SDL/include"
IncludeDir["ImGui"] = "%{prj.name}/vendor/ImGui"
IncludeDir["ImPlot"] = "%{prj.name}/vendor/ImPlot"
IncludeDir["Icmplib"] = "%{prj.name}/vendor/icmplib"

externalproject "SDL"
   location "PingPlotter/vendor/SDL/VisualC/SDL/"
   uuid "1a708b00-5301-11ee-9027-0800200c9a66"
   kind "StaticLib"
   language "C"

include "PingPlotter/vendor/ImGui/ImGui.lua"

project "PingPlotter"
    location "PingPlotter"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    
    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/ImPlot/**.h",
        "%{prj.name}/vendor/ImPlot/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.SDL}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImPlot}",
        "%{IncludeDir.Icmplib}"
    }

    links
    {
        "SDL",
        "ImGui",
        "setupapi.lib",
        "winmm.lib",
        "version.lib"
    }

    filter "system:windows"
        systemversion "latest"
    
        defines
        {
            "WINDOWS"
        }
        
        postbuildcommands
        {
            --"{COPY} vendor/SDL/libs/%{cfg.buildcfg}/SDL3.dll ../bin/" .. outputdir .. "/FennecApp"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        debugdir ("bin/" .. outputdir .. "/%{prj.name}")

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        debugdir("bin/" .. outputdir .. "/%{prj.name}")

    filter { "files:ImPlot/.github/example_implot.cpp" }
        flags "ExcludeFromBuild"