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
	include "XuanWu/vendor/Box2D"
	include "XuanWu/vendor/GLFW"
	include "XuanWu/vendor/Glad"
	include "XuanWu/vendor/imgui"
	include "XuanWu/vendor/yaml-cpp"
group ""

include "XuanWu"
include "XuanWu-Editor"
include "Sandbox"