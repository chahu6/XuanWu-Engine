workspace "XuanWu"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "XuanWu/vendor/GLFW/include"
IncludeDir["Glad"] = "XuanWu/vendor/Glad/include"
IncludeDir["ImGui"] = "XuanWu/vendor/imgui"
IncludeDir["glm"] = "XuanWu/vendor/glm"
IncludeDir["stb_image"] = "XuanWu/vendor/stb_image"

group "Dependencies"
	include "XuanWu/vendor/GLFW"
	include "XuanWu/vendor/Glad"
	include "XuanWu/vendor/imgui"
group ""


project "XuanWu"
	location "XuanWu"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "xwpch.h"
	pchsource "XuanWu/src/xwpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"XW_PLATFORM_WINDOWS",
			"XW_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

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



project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"XuanWu/vendor/spdlog/include",
		"XuanWu/src",
		"XuanWu/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"XuanWu"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"XW_PLATFORM_WINDOWS"
		}
	
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




project "XuanWu-Editor"
	location "XuanWu-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"XuanWu/vendor/spdlog/include",
		"XuanWu/src",
		"XuanWu/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"XuanWu"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"XW_PLATFORM_WINDOWS"
		}
	
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