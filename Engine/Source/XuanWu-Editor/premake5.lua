project "XuanWu-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Engine/Source/XuanWu/vendor/spdlog/include",
		"%{wks.location}/Engine/Source/XuanWu/src",
		"%{wks.location}/Engine/Source/XuanWu/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"XuanWu"
	}

	filter "system:windows"
		systemversion "latest"
	
	filter "configurations:Debug"
		defines "XW_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "XW_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "XW_DIST"
		runtime "Release"
		optimize "on"
		symbols "off"