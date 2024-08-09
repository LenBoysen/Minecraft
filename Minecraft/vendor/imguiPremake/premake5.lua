project "ImGui"
	kind "StaticLib"
	language "C++"
	location "../imgui/"

	targetdir ("../imgui/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../imgui/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "../imgui/imconfig.h",
        "../imgui/imgui.h",
        "../imgui/imgui.cpp",
        "../imgui/imgui_draw.cpp",
        "../imgui/imgui_internal.h",
        "../imgui/imgui_widgets.cpp",
        "../imgui/imgui_rectpack.h",
        "../imgui/imgui_textedit.h",
        "../imgui/imgui_truetype.h",
        "../imgui/imgui_demo.cpp"
    }
	filter "system:windows"
		cppdialect "c++20"
		staticruntime "On"
		systemversion "10.0"	

	filter "configurations:Debug"
		symbols "On"
	filter "configurations:Release"
		optimize "On"
	filter "configurations:Dist"
		optimize "On"
