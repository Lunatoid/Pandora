workspace "Pandora"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }
    
	flags {
		"MultiProcessorCompile"
	}
    
    files {
        "premake5.lua"
    }

outputdir = "%{cfg.buildcfg}"
depsdir   = "Dependencies"

inc_dirs = {}
inc_dirs["enet"]       = (depsdir .. "/ENet/include")
inc_dirs["freetype"]   = (depsdir .. "/FreeType/include")
inc_dirs["steamworks"] = (depsdir .. "/steamworks-146/include")
inc_dirs["soloud"]     = (depsdir .. "/SoLoud/include")
inc_dirs["assimp"]     = (depsdir .. "/assimp/include")

lib_dirs = {}
lib_dirs["pandora"]      = ("bin/" .. outputdir .. "/Pandora")
lib_dirs["enet"]       = (depsdir .. "/ENet/lib/x64")
lib_dirs["freetype"]   = (depsdir .. "/FreeType/lib/x64")
lib_dirs["steamworks"] = (depsdir .. "/steamworks-146/lib/win64")
lib_dirs["soloud"]     = (depsdir .. "/SoLoud/lib")
lib_dirs["assimp"]     = (depsdir .. "/assimp/lib/x64")

-- Register command-line options
newoption {
    trigger = "box-builder",
    description = "Enables or disables the box builder"
}

newoption {
    trigger = "no-assimp",
    description = "Removes assimp as a dependency. Meshes are loaded through .box resources only"
}

newoption {
    trigger = "no-directx",
    description = "Removes the DirectX video backend"
}

newoption {
    trigger = "no-opengl",
    description = "Removes the OpenGL video backend"
}

newoption {
    trigger = "no-soloud",
    description = "Removes the SoLoud audio backend"
}

newoption {
    trigger = "no-imgui",
    description = "Removes Dear ImGui"
}

-- Set up options

configuration "box-builder"
    defines "PD_BOX_BUILDER"

configuration "no-assimp"
    defines "PD_NO_ASSIMP"
    
configuration "not no-assimp"
    includedirs "%{inc_dirs.assimp}"
    libdirs "%{lib_dirs.assimp}"
    links "assimp"

configuration "no-directx"
    defines "PD_NO_DIRECTX"
    
configuration "no-opengl"
    defines "PD_NO_DIRECTX"
    
configuration "no-soloud"
    defines "PD_NO_SOLOUD"

configuration "not no-soloud"
    includedirs "%{inc_dirs.soloud}"
    libdirs "%{lib_dirs.soloud}"
    links "soloud_static_x64"
    
configuration "no-imgui"
    defines "PD_NO_IMGUI"

-- General config
cppdialect "C++14"
staticruntime "On"
systemversion "latest"
language "C++"
exceptionhandling "Off"
runtime "Release"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("obj/" .. outputdir .. "/%{prj.name}")

configuration "Debug"
    symbols "On"
    defines "PD_DEBUG"

configuration "Release"
    optimize "On"
    defines "PD_RELEASE"

    filter "system:windows"
        
filter "system:windows"
    buildoptions "/MP /wd26444 /wd6387 /wd4099 /wd4006 /sdl- /MT"
    linkoptions "/ignore:4006 /ignore:4099" -- Ignore "__NULL_IMPORT_DESCRIPTOR already defined in *.lib", Ignore "*.pdb not found"
    
    defines {
        "PD_WINDOWS",
        "_WINSOCKAPI_",
        "WIN32_LEAN_AND_MEAN"
    }
        
-- Individual projects

project "Pandora"
    location "Pandora"
    kind "StaticLib"
    language "C++"
    exceptionhandling "Off"
    runtime "Release"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")
    
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/Pandora.natvis"
    }

    includedirs {
        "Pandora/src",
        "%{inc_dirs.freetype}"
    }

    libdirs {
        "%{lib_dirs.freetype}"
    }

    links {
        "freetype"
    }

    defines {
        "PD_LIB"
    }

    filter "system:windows"
        links {
            "ws2_32.lib",
            "winmm.lib",
            "shlwapi.lib"
        }

    configuration "Debug"
        links "dbghelp.lib"

    configuration "Release"
        buildoptions "/MT"
        
project "PBox"
    location "PBox"
    dependson "Pandora"
    kind "ConsoleApp"
    
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.cpp",
    }

    includedirs "Pandora/src"
    libdirs "%{lib_dirs.pandora}"
    links "Pandora.lib"

    -- PBox always requires box builder
    defines {
        "PD_BOX_BUILDER"
    }

project "Sandbox"
    location "Sandbox"
    dependson "Pandora"
    kind "ConsoleApp"
    
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.cpp",
    }

    includedirs "Pandora/src"
    libdirs "%{lib_dirs.pandora}"
    links "Pandora.lib"

project "Tetro"
    location "Tetro"
    dependson "Pandora"
    kind "ConsoleApp"
    
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.cpp",
    }

    includedirs "Pandora/src"
    libdirs "%{lib_dirs.pandora}"
    links "Pandora.lib"
