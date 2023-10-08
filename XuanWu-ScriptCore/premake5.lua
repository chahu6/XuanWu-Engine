project "XuanWu-ScriptCore"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"
    
	targetdir ("../XuanWu-Editor/Resources/Scripts")
	objdir ("../XuanWu-Editor/Resources/Scripts/Intermediates")

    files
    {
		"Source/**.cs"
    }

    filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"