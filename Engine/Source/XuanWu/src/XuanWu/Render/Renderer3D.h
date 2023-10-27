#pragma once

#include <glm/glm.hpp>

namespace XuanWu
{
	class EditorCamera;
	class Model;

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void DrawModel(const glm::mat4& transform, const glm::vec2& size, Model* model, int entityID);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t TriangleCount = 0;

			uint32_t GetTotalVertexCount() { return TriangleCount * 3; }
			//uint32_t GetTotalIndexCount() { return TriangleCount * 6; }
		};

		static void ResetStats();

		static Statistics GetStats();
	private:
		static void StartBatch();
		static void NextBatch();
	};
}