project "Parser"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "xwpch.h"
	pchsource "src/xwpch.cpp"

	-- 定义自定义构建规则
	if os.istarget("windows") then
		postbuildcommands {
			-- 复制DLL文件到输出路径
			"{COPY} vendor/LLVM/bin/x64/libclang.dll %{cfg.targetdir}"
		}
	elseif os.istarget("macosx") then
		postbuildcommands {
			-- 复制DLL文件到输出路径
			"cp vendor/LLVM/bin/macOS/libclang.dylib %{cfg.targetdir}"
		}
	elseif os.istarget("linux") then
		postbuildcommands {
			-- 复制DLL文件到输出路径
			"cp vendor/LLVM/bin/Linux/libclang.so.12 %{cfg.targetdir}"
		}
	end

	files
	{
		"src/**.h",
		"src/**.cpp",

		"vendor/Mustache/mustache.hpp"
	}

	defines
	{
		-- "_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"src",
		"vendor/LLVM/include",
		"vendor/Mustache"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			-- "XW_BUILD_DLL"
		}

		links
		{
			"vendor/LLVM/lib/x64/libclang.lib"
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
		symbols "off"