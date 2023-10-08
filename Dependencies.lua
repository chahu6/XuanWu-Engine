

IncludeDir = {}
IncludeDir["Box2D"] = "%{wks.location}/XuanWu/vendor/Box2D/include"
IncludeDir["entt"] = "%{wks.location}/XuanWu/vendor/entt/include"
IncludeDir["GLFW"] = "%{wks.location}/XuanWu/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/XuanWu/vendor/Glad/include"
IncludeDir["glm"] = "%{wks.location}/XuanWu/vendor/glm"
IncludeDir["ImGui"] = "%{wks.location}/XuanWu/vendor/imgui"
IncludeDir["ImGuizmo"] = "%{wks.location}/XuanWu/vendor/ImGuizmo"
IncludeDir["mono"] = "%{wks.location}/XuanWu/vendor/mono/include"
IncludeDir["stb_image"] = "%{wks.location}/XuanWu/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/XuanWu/vendor/yaml-cpp/include"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/XuanWu/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["Bcrypt"] = "Bcrypt.lib"
