workspace "Fennec"
    platforms "x64"
    architecture "x64"
    startproject "FennecApp"
    
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "Fennec/vendor/spdlog/include"
IncludeDir["SDL"] = "Fennec/vendor/SDL/include"
IncludeDir["ImGui"] = "Fennec/vendor/ImGui"
IncludeDir["DirectXTK12"] = "Fennec/vendor/DirectXTK12/Inc"
IncludeDir["DirectXHeaders"] = "Fennec/vendor/DirectX-Headers/include/directx"
IncludeDir["assimp"] = "Fennec/vendor/assimp/include"

externalproject "SDL"
   location "Fennec/vendor/SDL/VisualC/SDL/"
   uuid "1a708b00-5301-11ee-9027-0800200c9a66"
   kind "StaticLib"
   language "C"

--include "Fennec/vendor/assimp/assimp.lua"
include "Fennec/vendor/ImGui/ImGui.lua"
include "Fennec/vendor/DirectXTK12/DirectXTK12.lua"

project "Fennec"
    location "Fennec"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    
    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "fenpch.h"
    pchsource "Fennec/src/fenpch.cpp"

    shaderobjectfileoutput ("../bin/" .. outputdir .. "/FennecApp/%%(Filename).pdb")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hlsl",
        "%{prj.name}/src/**.hlsli"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.SDL}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.DirectXTK12}",
        "%{IncludeDir.DirectXHeaders}",
        "%{IncludeDir.assimp}"
    }

    links
    {
        "SDL",
        "ImGui",
       -- "assimp",
        "DirectXTK12",
        "d3d12.lib",
        "dxgi.lib",
        "dxguid.lib",
        "setupapi.lib",
        "winmm.lib",
        "imm32.lib",
        "version.lib"
    }

    filter "system:windows"
        systemversion "latest"
    
        defines
        {
            "FEN_PLATFORM_WINDOWS"
        }

        --buildoptions
        --{
        --    "/external:anglebrackets",
        --    "/external:W0"
        --}
        
        postbuildcommands
        {
            --"{COPY} vendor/SDL/libs/%{cfg.buildcfg}/SDL3.dll ../bin/" .. outputdir .. "/FennecApp"
        }

    filter "configurations:Debug"
        defines "FEN_DEBUG"
        runtime "Debug"
        symbols "on"
        defines {"FEN_DEBUG"}
        debugdir ("bin/" .. outputdir .. "/%{prj.name}")

    filter "configurations:Release"
        defines "FEN_RELEASE"
        runtime "Release"
        optimize "on"
        defines {"FEN_RELEASE"}
        debugdir("bin/" .. outputdir .. "/%{prj.name}")

    filter "configurations:Dist"
        defines "FEN_DIST"
        runtime "Release"
        optimize "on"
        defines {"FEN_DIST"}
        debugdir("bin/" .. outputdir .. "/%{prj.name}")

    filter { "files:**.hlsli" }
        flags "ExcludeFromBuild"
    filter { "files:**.hlsl" }
        flags "ExcludeFromBuild"
        shadermodel "6.6"
    filter { "files:**-p.hlsl" }
        removeflags "ExcludeFromBuild"
        shadertype "Pixel"
        shaderentry "PS"
    filter { "files:**-v.hlsl" }
        removeflags "ExcludeFromBuild"
        shadertype "Vertex"
        shaderentry "VS"