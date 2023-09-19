workspace "XuanWu"
	architecture "x64"
	startproject "XuanWu-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/XuanWu/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/XuanWu/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/XuanWu/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/XuanWu/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/XuanWu/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/XuanWu/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/XuanWu/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/XuanWu/vendor/ImGuizmo"

group "Dependencies"
	include "XuanWu/vendor/GLFW"
	include "XuanWu/vendor/Glad"
	include "XuanWu/vendor/imgui"
	include "XuanWu/vendor/yaml-cpp"
group ""

include "XuanWu"
include "XuanWu-Editor"
include "Sandbox"