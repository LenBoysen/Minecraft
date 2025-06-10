project "FastNoise2"
	kind "StaticLib"
	language "C++"
	location "../FastNoise2/"
	staticruntime "on"
	cppdialect "c++17"

	targetdir ("../FastNoise2/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../FastNoise2/bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
        "../FastNoise2/include",
        "../FastNoise2/include",
	}

	--buildoptions { "/arch:AVX512" }
	buildoptions { "-march=core-avx2" }
	buildoptions { "-mavx512f -mavx512dq" }
	files
	{
        "../FastNoise2/include/**.h",
        "../FastNoise2/include/**.cpp",
        "../FastNoise2/src/**.h",
        "../FastNoise2/src/**.cpp",
    }
	defines
	{
		"FASTNOISE_STATIC_LIB",
	}
	filter "system:windows"
		systemversion "latest"	

	filter "configurations:Debug"
		symbols "On"
	filter "configurations:Release"
		optimize "On"
	filter "configurations:Dist"
		optimize "On"
