workspace "Minecraft"
	architecture "x64"
	startproject "Minecraft"
	buildoptions { "/MP" } --Multi threaded compiling

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--[[
project "Minecraft"
	location "Minecraft"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}


	includedirs
	{
		"%{prj.name}/vendor/spdlog/include"
	}
	filter "system:windows"
		cppdialect "c++20"
		staticruntime "On"
		systemversion "10.0"

		postbuildcommands
		{
			("{MKDIR} ../bin/" .. outputdir .. "/Sandbox"),
			("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}
		
		filter "configurations:Debug"
			defines
			{
				"EY_PLATFORM_WINDOWS",
				"EY_BUILD_DLL",
				"_DEBUG",
				"_CONSOLE"
			}
		filter "configurations:Release"
			defines
			{
				"EY_PLATFORM_WINDOWS",
				"EY_BUILD_DLL",
				"NDEBUG",
				"_CONSOLE"
			}
		filter "configurations:Dist"
			defines
			{
				"EY_PLATFORM_WINDOWS",
				"EY_BUILD_DLL",
				"NDEBUG",
				"_CONSOLE"
			}

		

	filter "configurations:Debug"
		defines "EY_DEBUG"
		symbols "On"
	filter "configurations:Release"
		defines "EY_RELEASE"
		optimize "On"
	filter "configurations:Dist"
		defines "EY_DIST"
		optimize "On"
--]]

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

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"--,
		--"%{prj.name}/vendor/glm/glm/**.h",
		--"%{prj.name}/vendor/glm/glm/**.hpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
		--"%{prj.name}/vendor/glfw/bin/" .. outputdir .. "/GLFW",
		--"Minecraft/src"
	}


	--libdirs
	--{
	--	"%{prj.name}/vendor/GLFW/bin/" .. outputdir .. "/GLFW"
	--}

	links
	{
		"GLFW",
		"Glad",	
		"ImGui",
		"opengl32.lib"
		--"EasyEngine"
	}


	filter "system:windows"
		cppdialect "c++20"
		staticruntime "off"
		systemversion "10.0"
		defines
			{
				"GLFW_INCLUDE_NONE",
				--"EY_PLATFORM_WINDOWS",
				"_CONSOLE"
			}
		
		filter "configurations:Debug"
			defines
			{
				
				"_DEBUG"
			}
		filter "configurations:Release"
			defines
			{
				
				"NDEBUG"
			}
		filter "configurations:Dist"
			defines
			{
				
				"NDEBUG"
			}


	filter "configurations:Debug"
		defines "MC_DEBUG"
		symbols "On"
	filter "configurations:Release"
		defines "MC_RELEASE"
		optimize "On"
	filter "configurations:Dist"
		defines "MC_DIST"
		optimize "On"