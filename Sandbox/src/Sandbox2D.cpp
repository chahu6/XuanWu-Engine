#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include "XuanWu/Render/Renderer2D.h"

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, false)
{

}

void Sandbox2D::OnAttach()
{
	XW_PROFILE_FUNCTION();

	m_Texture = XuanWu::Texture2D::Create("assets/textures/Checkerboard.png");
	m_SpriteTexture = XuanWu::Texture2D::Create("assets/games/RPGpack_sheet_2X.png");

	m_TextureTree = XuanWu::SubTexture2D::CreateFromCoords(m_SpriteTexture, {0, 1}, { 128, 128 }, {1, 2});
	m_TextureRoof = XuanWu::SubTexture2D::CreateFromCoords(m_SpriteTexture, { 0, 4 }, { 128, 128 }, { 2, 3 });

	// Init Particle
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 5.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
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
	XuanWu::Renderer2D::DrawQuad({ -1.f, 0.f, 0.5f }, {1.f, 2.f}, m_TextureTree);
	XuanWu::Renderer2D::DrawQuad({ 1.f, 0.f, 0.5f }, {2.f, 3.f}, m_TextureRoof);
	XuanWu::Renderer2D::EndScene();
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
	}
	ImGui::End();
}

void Sandbox2D::OnEvent(XuanWu::Event& event)
{
	m_CameraController.OnEvent(event);
}
