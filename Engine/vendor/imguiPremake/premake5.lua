project "ImGui"
	kind "StaticLib"
	language "C++"
	location "../imgui/"
	staticruntime "on"
	cppdialect "c++20"

	targetdir ("../imgui/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../imgui/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "../imgui/imconfig.h",
        "../imgui/imgui.h",
        "../imgui/imgui.cpp",
        "../imgui/imgui_demo.cpp",
        "../imgui/imgui_draw.cpp",
        "../imgui/imgui_internal.h",
        "../imgui/imgui_tables.cpp",
        "../imgui/imgui_widgets.cpp",
        "../imgui/imstb_rectpack.h",
        "../imgui/imstb_textedit.h",
        "../imgui/imstb_truetype.h",
    }
	filter "system:windows"
		systemversion "latest"	

	filter "configurations:Debug"
		symbols "On"
	filter "configurations:Release"
		optimize "On"
	filter "configurations:Dist"
		optimize "On"
