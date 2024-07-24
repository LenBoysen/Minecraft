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

include "Minecraft/vendor/glfw"

project "Minecraft"
	location "Minecraft"
	kind "ConsoleApp"
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
		"%{prj.name}/src",
		"%{prj.name}/vendor/glfw/include",
		"%{prj.name}/vendor/glfw/bin/" .. outputdir .. "/GLFW",
		--"Minecraft/src"
	}

	links
	{
		"GLFW",
		"opengl32"
		--"EasyEngine"
	}


	filter "system:windows"
		cppdialect "c++20"
		staticruntime "On"
		systemversion "10.0"
	
		
		filter "configurations:Debug"
			defines
			{
				--"EY_PLATFORM_WINDOWS",
				"_DEBUG",
				"_CONSOLE"
			}
		filter "configurations:Release"
			defines
			{
				--"EY_PLATFORM_WINDOWS",
				"NDEBUG",
				"_CONSOLE"
			}
		filter "configurations:Dist"
			defines
			{
				--"EY_PLATFORM_WINDOWS",
				"NDEBUG",
				"_CONSOLE"
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