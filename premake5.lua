workspace "PingSight"
    platforms "x64"
    architecture "x64"
    startproject "PingSight"
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
IncludeDir["stb"] = "%{prj.name}/vendor/stb"

externalproject "SDL3-static"
   location "PingSight/vendor/SDL/build/"
   uuid "1a708b00-5301-11ee-9027-0800200c9a66"
   kind "StaticLib"
   language "C"
   
externalproject "SDL_uclibc"
   location "PingSight/vendor/SDL/build/"
   uuid "f0946cdd-5260-4743-a524-dbeabe01871e"
   kind "StaticLib"
   language "C"

project "PingSight"
    location "PingSight"
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
        "%{prj.name}/vendor/ImGui/*.h",
        "%{prj.name}/vendor/ImGui/*.cpp",
        "%{prj.name}/vendor/ImPlot/**.h",
        "%{prj.name}/vendor/ImPlot/**.cpp",
        "%{prj.name}/vendor/ImGui/backends/imgui_impl_sdlrenderer3.h",
        "%{prj.name}/vendor/ImGui/backends/imgui_impl_sdlrenderer3.cpp",
        "%{prj.name}/vendor/ImGui/backends/imgui_impl_sdl3.h",
        "%{prj.name}/vendor/ImGui/backends/imgui_impl_sdl3.cpp",
        "%{prj.name}/vendor/STB/stb_image.h",        
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.SDL}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImPlot}",
        "%{IncludeDir.Icmplib}",
        "%{IncludeDir.plf_nanotimer}",
        "%{IncludeDir.CImg}",
        "%{IncludeDir.zlib}",
        "%{IncludeDir.libpng}",
        "%{IncludeDir.stb}",
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
            "{COPY} src/PingSightConfig.ini ../bin/" .. outputdir .. "/%{prj.name}",
            "{COPY} Roboto.ttf ../bin/" .. outputdir .. "/%{prj.name}"
        }
        
        files { 'PingSight/PingSight.rc', '**.ico' }
        vpaths { ['Resources/*'] = { '*.rc', '**.ico' }}
        removefiles("%{prj.name}/src/Icon.h","%{prj.name}/src/Icon.cpp","%{prj.name}/src/AppIcon.h","%{prj.name}/src/AppIcon.cpp")

    filter "system:macosx"
        kind "WindowedApp"

    filter "system:linux"
        kind "WindowedApp"
        libdirs { "%{prj.name}/vendor/SDL/build" }
        links{"SDL3"}
        --linkoptions { "-Wl,-rpath,$(ORIGIN)/%{prj.name}/vendor/SDL/build" }
                
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