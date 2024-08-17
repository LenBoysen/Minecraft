workspace "Minecraft"
	architecture "x64"
	startproject "Minecraft"


	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir ={}
IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"
IncludeDir["Glad"] = "Engine/vendor/Glad/include"
IncludeDir["glm"] = "Engine/vendor/glm"
IncludeDir["ImGui"] = "Engine/vendor/imgui"
IncludeDir["Engine"] = "Engine/src"

require "vendor/premake-export-compile-commands/export-compile-commands"

include "Engine/vendor/GLFW"
include "Engine/vendor/Glad"
include "Engine/vendor/imguiPremake"


project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "c++17"
	staticruntime "off"
	warnings "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


	postbuildcommands
	{
		--"{COPYFILE} %[cfg.buildtarget.directory] %[/bin/" .. outputdir .. "/Minecraft]"
		("{MKDIR} ../bin/" .. outputdir .. "/Minecraft"),
		("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Minecraft")
	}



	files
	{

		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

	}


	includedirs
	{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
	}



	filter "system:linux"
		pic "On"  -- Position Independent Code (PIC) for shared libraries
		systemversion "latest"

		links
		{
			"X11",
			"pthread",
			"dl",
			"GL",
		}

	filter "system:windows"
		systemversion "10.0"


		links
		{
			"gdi32", "user32",
			"shell32", "advapi32",
			"kernel32", "opengl32",
		}





	filter "action:gmake2"
		buildoptions { "-MP" } --Multi threaded compiling
		filter "configurations:Debug"
			prebuildcommands
			{
				--"{COPYFILE} %[./compile_commands/debug.json] %[./compile_commands.json]"
			}
		filter "configurations:Release"
			prebuildcommands
			{
				"{COPYFILE} %[./compile_commands/release.json] %[./compile_commands.json]"
			}
		filter "configurations:Dist"
			prebuildcommands
			{
				"{COPYFILE} %[./compile_commands/dist.json] %[./compile_commands.json]"
			}

	filter "action:vs*"
		buildoptions { "/MP" } --Multi threaded compilin




	defines
	{
		"GLFW_INCLUDE_NONE",
		"ENGINE_BUILD_DLL",
		"ENGINE_PLATFORM_WINDOWS",
	}
	filter "configurations:Debug"
		defines
		{
			"_DEBUG",
			"MC_DEBUG",
		}
		symbols "On"

		

	filter "configurations:Release"
		defines
		{
			"NDEBUG",
			"MC_RELEASE",
		}
		optimize "On"
	filter "configurations:Dist"
		defines
		{
			"NDEBUG",
			"MC_DIST",
		}
		optimize "On"





project "Minecraft"
	location "Minecraft"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{

		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

	}

	includedirs
	{
		"%{IncludeDir.Engine}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"Engine",
		"GLFW",
		"Glad",
		"ImGui",
	}


	filter "system:windows"
		systemversion "10.0"


		links
		{
			"gdi32", "user32",
			"shell32", "advapi32",
			"kernel32", "opengl32",
		}

	filter "action:vs*"
		buildoptions { "/MP" } --Multi threaded compilin

	defines
	{
		"_CONSOLE",
		"ENGINE_PLATFORM_WINDOWS",
	}
	filter "configurations:Debug"
		defines
		{
			"_DEBUG",
			"MC_DEBUG",
		}
		symbols "On"

		

	filter "configurations:Release"
		defines
		{
			"NDEBUG",
			"MC_RELEASE",
		}
		optimize "On"
	filter "configurations:Dist"
		defines
		{
			"NDEBUG",
			"MC_DIST",
		}
		optimize "On"




if _ACTION == "gmake2" then
	printf(_ACTION)
	os.execute(_PREMAKE_COMMAND .. " export-compile-commands")
	os.copyfile("./compile_commands/debug.json", "./compile_commands.json")
end
if _ACTION == "clean" then
	os.remove("./compile_commands.json")
	os.remove("./Minecraft.sln")
	os.remove("./Makefile")
	os.rmdir("./compile_commands/")
	os.rmdir("./bin/")
	os.rmdir("./bin-int/")
end
