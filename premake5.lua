workspace "PingTracker"
    platforms "x64"
    architecture "x64"
    startproject "PingTracker"
    dpiawareness "HighPerMonitor"
    
    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["SDL"] = "%{prj.name}/vendor/SDL/include"
IncludeDir["ImGui"] = "%{prj.name}/vendor/imgui"
IncludeDir["ImPlot"] = "%{prj.name}/vendor/implot"
IncludeDir["Icmplib"] = "%{prj.name}/vendor/icmplib"
IncludeDir["plf_nanotimer"] = "%{prj.name}/vendor/plf_nanotimer"

project "PingTracker"
    location "PingTracker"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    
    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/imgui/*.h",
        "%{prj.name}/vendor/imgui/*.cpp",
        "%{prj.name}/vendor/implot/**.h",
        "%{prj.name}/vendor/implot/**.cpp",
        "%{prj.name}/vendor/imgui/backends/imgui_impl_sdlrenderer3.h",
        "%{prj.name}/vendor/imgui/backends/imgui_impl_sdlrenderer3.cpp",
        "%{prj.name}/vendor/imgui/backends/imgui_impl_sdl3.h",
        "%{prj.name}/vendor/imgui/backends/imgui_impl_sdl3.cpp",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.SDL}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImPlot}",
        "%{IncludeDir.Icmplib}",
        "%{IncludeDir.plf_nanotimer}",
    }

    filter "system:windows"
        systemversion "latest"
        kind "WindowedApp"
        links
        {
            "SDL3-static",
            "setupapi.lib",
            "winmm.lib",
            "version.lib",    
        }

        defines
        {
            "WINDOWS"
        }
        
        postbuildcommands
        {
            "{COPY} src/PingTracker/PingTrackerConfig.ini ../bin/" .. outputdir .. "/%{prj.name}",
            "{COPY} resources/Roboto.ttf ../bin/" .. outputdir .. "/%{prj.name}"
        }

        files { 'PingTracker/PingTracker.rc', '**.ico' }
        vpaths { ['Resources/*'] = { '*.rc', '**.ico' }}
        
        filter "configurations:Debug"
            libdirs
            {
                "PingTracker/vendor/SDL/build/Debug"
            }
        
        filter "configurations:Release"
            libdirs
            {
                "PingTracker/vendor/SDL/build/Release"
            }

       

    filter "system:macosx"
        kind "WindowedApp"

    filter "system:linux"
        kind "WindowedApp"
        libdirs { "%{prj.name}/vendor/SDL/build" }
        links{"SDL3"}
        --linkoptions { "-Wl,-rpath,$(ORIGIN)/%{prj.name}/vendor/SDL/build" }
        removefiles("%{prj.name}/vendor/ImPlot/.github/example_implot.cpp")
                
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