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
IncludeDir["plf_nanotimer"] = "%{prj.name}/vendor/plf_nanotimer"
IncludeDir["lodepng"] = "%{prj.name}/vendor/lodepng"
IncludeDir["libpng"] = "%{prj.name}/vendor/libpng"
IncludeDir["zlib"] = "%{prj.name}/vendor/libpng/zlib"

externalproject "SDL3-static"
   location "PingPlotter/vendor/SDL/build/"
   uuid "1a708b00-5301-11ee-9027-0800200c9a66"
   kind "StaticLib"
   language "C"

   externalproject "zlibstatic"
   location "PingPlotter/vendor/libpng/zlib/build/"
   uuid "8e36aa21-c0f6-4b2e-b709-5583da679515"
   kind "StaticLib"
   language "C"

   externalproject "png_static"
   location "PingPlotter/vendor/libpng/"
   uuid "74182e39-f892-4244-8781-d3498dd4df99"
   kind "StaticLib"
   language "C"

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
        "%{prj.name}/vendor/ImGui/*.h",
        "%{prj.name}/vendor/ImGui/*.cpp",
        "%{prj.name}/vendor/ImPlot/**.h",
        "%{prj.name}/vendor/ImPlot/**.cpp",
        "%{prj.name}/vendor/ImGui/backends/imgui_impl_sdlrenderer3.h",
        "%{prj.name}/vendor/ImGui/backends/imgui_impl_sdlrenderer3.cpp",
        "%{prj.name}/vendor/ImGui/backends/imgui_impl_sdl3.h",
        "%{prj.name}/vendor/ImGui/backends/imgui_impl_sdl3.cpp",
        "%{prj.name}/vendor/lodepng/lodepng.h",
        "%{prj.name}/vendor/lodepng/lodepng.cpp",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.SDL}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImPlot}",
        "%{IncludeDir.Icmplib}",
        "%{IncludeDir.plf_nanotimer}",
        "%{IncludeDir.lodepng}",
        "%{IncludeDir.libpng}",
        "%{IncludeDir.zlib}",

    }

    links
    {
        "SDL3-static",
        "png_static",
        "zlibstatic",
        "setupapi.lib",
        "winmm.lib",
        "version.lib",    
    }

    filter "system:windows"
        systemversion "latest"
        kind "WindowedApp"
        defines
        {
            "WINDOWS"
        }
        
        postbuildcommands
        {
            "{COPY} src/PingPlotterConfig.ini ../bin/" .. outputdir .. "/%{prj.name}"
        }

    filter "system:macosx"
        kind "WindowedApp"

    filter "system:linux"
        kind "WindowedApp"

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