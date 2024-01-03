include "Dependencies.lua"

workspace "XuanWu"
	architecture "x64"
	startproject "XuanWu-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	-- 多处理器编译是一个可选的优化选项，开启与否需要根据项目实际情况和开发者的需求进行决策。
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Engine/Source/Runtime/vendor/Box2D"
	include "Engine/Source/Runtime/vendor/GLFW"
	include "Engine/Source/Runtime/vendor/Glad"
	include "Engine/Source/Runtime/vendor/imgui"
	include "Engine/Source/Runtime/vendor/yaml-cpp"
group ""

include "Engine/Source/Runtime"
include "Engine/Source/XuanWu-Editor"
include "Engine/Source/Sandbox"

group "Tools"
	include "Engine/Source/Parser"
	include "Engine/Source/PreCompile"
group ""