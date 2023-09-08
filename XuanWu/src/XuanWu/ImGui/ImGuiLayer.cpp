#include "xwpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "XuanWu/Core/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "font.h"


namespace XuanWu {

	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		XW_PROFILE_FUNCTION();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

		io.ConfigFlags |= ImGuiViewportFlags_NoDecoration;
		io.ConfigFlags |= ImGuiCol_DockingEmptyBg;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

		// 字体
		ImFontConfig f_cfg;
		f_cfg.FontDataOwnedByAtlas = false;
		ImFont* font = io.Fonts->AddFontFromMemoryTTF((void*)fonts_data, fonts_size, 18.0f, &f_cfg, io.Fonts->GetGlyphRangesChineseFull());
	}

	void ImGuiLayer::OnDetach()
	{
		XW_PROFILE_FUNCTION();

		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);// 当前OnImGuiRender层显示DemoUI窗口
	}

	void ImGuiLayer::Begin()
	{
		XW_PROFILE_FUNCTION();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//可停靠的窗口
		//ImGui::DockSpaceOverViewport();
	}

	void ImGuiLayer::End()
	{
		XW_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		//更新和渲染额外的平台窗口
		//(平台函数可能会改变当前的OpenGL上下文，所以我们保存/恢复它，以便更容易地将此代码粘贴到其他地方。
		//对于这个特定的演示应用程序，我们也可以直接调用glfwMakeContextCurrent(window)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}