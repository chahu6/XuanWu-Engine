#pragma once

#include "XuanWu/Render/Shader.h"

#include <glm/glm.hpp>

namespace XuanWu {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual const std::string& GetName() const override { return m_Name; }

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* value, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetVec3(const std::string& name, const glm::vec3& value) override;
		virtual void SetVec4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void UploadUniformIntArray(const std::string& name, int* value, uint32_t count);
		void setFloat(const std::string& name, float value) const;
		void setMat4(const std::string& name, const glm::mat4& model) const;
		void setVec4(const std::string& name, const glm::vec4& value) const;
		void setMat3(const std::string& name, const glm::mat3& model) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec3(const std::string& name, const glm::vec3& value) const;
		void setVec2(const std::string& name, const glm::vec2& value) const;
	private:
		std::string ReadFile(const std::string& filepath);
		void Compile(const std::string& vertexSrc, const std::string& fragmentSrc);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}