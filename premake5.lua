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
IncludeDir["GLFW"] = "Minecraft/vendor/GLFW/include"
IncludeDir["Glad"] = "Minecraft/vendor/Glad/include"
IncludeDir["glm"] = "Minecraft/vendor/glm"
IncludeDir["ImGui"] = "Minecraft/vendor/imgui"

include "Minecraft/vendor/GLFW"
include "Minecraft/vendor/Glad"
include "Minecraft/vendor/imguiPremake"

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

	filter "action:vs*"
		buildoptions { "/MP" } --Multi threaded compilin


	

	defines
	{
		"GLFW_INCLUDE_NONE",
		"_CONSOLE"
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
