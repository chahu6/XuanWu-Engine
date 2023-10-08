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
		"%{wks.location}/XuanWu/vendor/spdlog/include",
		"%{wks.location}/XuanWu/src",
		"%{wks.location}/XuanWu/vendor",
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
		symbols "On"

	filter "configurations:Release"
		defines "XW_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "XW_DIST"
		runtime "Release"
		optimize "On"
		symbols "Off"