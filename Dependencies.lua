

IncludeDir = {}
-- 引擎内部
IncludeDir["Reflection"] = "%{wks.location}/Engine/Intermediate/_generated/reflection"

-- 第三方库
IncludeDir["Box2D"] = "%{wks.location}/Engine/Source/Runtime/vendor/Box2D/include"
IncludeDir["entt"] = "%{wks.location}/Engine/Source/Runtime/vendor/entt/include"
IncludeDir["GLFW"] = "%{wks.location}/Engine/Source/Runtime/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Engine/Source/Runtime/vendor/Glad/include"
IncludeDir["glm"] = "%{wks.location}/Engine/Source/Runtime/vendor/glm"
IncludeDir["ImGui"] = "%{wks.location}/Engine/Source/Runtime/vendor/imgui"
IncludeDir["ImGuizmo"] = "%{wks.location}/Engine/Source/Runtime/vendor/ImGuizmo"
IncludeDir["stb_image"] = "%{wks.location}/Engine/Source/Runtime/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Engine/Source/Runtime/vendor/yaml-cpp/include"
IncludeDir["assimp"] = "%{wks.location}/Engine/Source/Runtime/vendor/assimp/include"


LibraryDir = {}
LibraryDir["assimp"] = "%{wks.location}/Engine/Source/Runtime/vendor/assimp/lib"

Library = {}
Library["assimp"] = "%{LibraryDir.assimp}/assimp-vc143-mtd.lib"