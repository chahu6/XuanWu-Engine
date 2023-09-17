project "XuanWu-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
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
		"%{IncludeDir.entt}"
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