project "PreCompile"
    kind "Utility"
    language "C++"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	  objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
   
    -- 在编译之前执行的事件
    prebuildcommands 
    {
		"%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Parser/Parser.exe D:/Test-C++/XuanWu/Engine/Intermediate/precompile.json D:/Test-C++/XuanWu/Engine/Intermediate/parser_header.h D:/Test-C++/XuanWu/Engine/Source/XuanWu/src * XuanWu 0"
    }

    links
    {
        "Parser"
    }

    filter "system:windows"
      systemversion "latest"

      defines
      {
        -- "XW_BUILD_DLL"
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