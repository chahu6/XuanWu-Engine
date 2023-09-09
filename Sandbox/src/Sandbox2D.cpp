#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include "XuanWu/Render/Renderer2D.h"


static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWDDDWWWWWW"
"WWWWWWWWDDDDDWWWWWWWWWWW"
"WWWDDDDWWWWWWWWDDDDWWWWW"
"WWWDDDDWWWWWWWWDDDDWWWWW"
"WWWDDDDWssssWWWDDDDWWWWW"
"WWWDDDDWWWWWWWWDDDDWWWWW"
"WWWDDDDWWWWWWWWDDDDWWWWW"
"WWWDDDDWWWWWWWWDDDDWWWWW"
"WWWDDDDWWWWWWWWDDDDWWWWW"
"WWWWWWWWDDDDDWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWDD"
"WWWWWWWWWWWWWWWWWWWWWWDD"
;

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, false)
{

}

void Sandbox2D::OnAttach()
{
	XW_PROFILE_FUNCTION();

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / m_MapWidth;

	m_Texture = XuanWu::Texture2D::Create("assets/textures/Checkerboard.png");
	m_SpriteTexture = XuanWu::Texture2D::Create("assets/games/RPGpack_sheet_2X.png");

	m_TextureGrass = XuanWu::SubTexture2D::CreateFromCoords(m_SpriteTexture, { 2, 3 }, { 128, 128 });

	s_TextureMap['W'] = XuanWu::SubTexture2D::CreateFromCoords(m_SpriteTexture, { 11, 11 }, { 128, 128 });
	s_TextureMap['D'] = XuanWu::SubTexture2D::CreateFromCoords(m_SpriteTexture, { 1, 11 }, { 128, 128 });

	XuanWu::FramebufferSpecification spec;
	spec.Width = 1280;
	spec.Height = 720;
	m_Framebuffer = XuanWu::Framebuffer::Create(spec);

	// Init Particle
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 5.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	m_CameraController.SetZoomLevel(6.5f);
}

void Sandbox2D::OnDetach()
{
	XW_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(XuanWu::Timestep ts)
{
	XW_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	XuanWu::Renderer2D::ResetStats();

	// 绑定帧缓冲
	m_Framebuffer->Bind();

	{
		XW_PROFILE_SCOPE("Renderer Prep");

		XuanWu::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
		XuanWu::RenderCommand::Clear();
	}
#if 0
	{
		static float rotation = 0.f;
		rotation += ts * 50.f;

		XW_PROFILE_SCOPE("Renderer Draw");
		XuanWu::Renderer2D::BeginScene(m_CameraController.GetCamera());
		XuanWu::Renderer2D::DrawQuad({ -1.0f, -0.5f }, { 0.8f, 0.8f }, { 0.8f, 0.8f, 0.8f , 1.0f });
		XuanWu::Renderer2D::DrawQuad({ -1.0f, 0.5f }, { 0.5f, 0.75f }, m_SquareColor);
		XuanWu::Renderer2D::DrawRotatedQuad({ 0.0f, .0f, 0.1f }, { 1.0f, 1.0f }, glm::radians(rotation), m_Texture, 10.0f);
		XuanWu::Renderer2D::DrawQuad({ .0f, .0f , -0.1f}, { 10.0f, 10.0f }, m_Texture, 1.0f);
		XuanWu::Renderer2D::EndScene();

		XuanWu::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f, 0.6f };
				XuanWu::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		XuanWu::Renderer2D::EndScene();
	}
#endif
	if (XuanWu::Input::IsMouseButtonPressed(XW_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = XuanWu::Input::GetMousePosition();
		auto width = XuanWu::Application::Get().GetWindow().GetWidth();
		auto height = XuanWu::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	XuanWu::Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (uint32_t y = 0; y < m_MapHeight; y++)
	{
		for (uint32_t x = 0; x < m_MapWidth; x++)
		{
			char titleType = s_MapTiles[x + y * m_MapWidth];
			XuanWu::Ref<XuanWu::SubTexture2D> texture;
			if (s_TextureMap.find(titleType) != s_TextureMap.end())
			{
				texture = s_TextureMap[titleType];
			}
			else
			{
				texture = m_TextureGrass;
			}
			XuanWu::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight / 2.0f - y, 0.5f }, { 1.0f, 1.0f }, texture);
		}
	}
	XuanWu::Renderer2D::EndScene();

	// 解除帧缓冲绑定
	m_Framebuffer->Unbind();
}

void Sandbox2D::OnImGuiRender()
{
	XW_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	{
		auto&stats = XuanWu::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats: ");
		ImGui::Text(u8"绘制次数：%d", stats.DrawCalls);
		ImGui::Text(u8"四边形数量：%d", stats.QuadCount);
		ImGui::Text(u8"顶点数量：%d", stats.GetTotalVertexCount());
		ImGui::Text(u8"索引数量：%d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 1280, 720 }, ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::End();


	if (ImGui::BeginMainMenuBar())
	{
		// 添加一个菜单
		if (ImGui::BeginMenu("Options"))
		{
			// 添加菜单项
			if (ImGui::MenuItem("Exit"))
			{
				XuanWu::Application::Get().Close();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

}

void Sandbox2D::OnEvent(XuanWu::Event& event)
{
	m_CameraController.OnEvent(event);
}
