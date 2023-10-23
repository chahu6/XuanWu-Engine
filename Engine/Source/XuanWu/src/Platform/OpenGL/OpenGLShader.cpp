#include "xwpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace XuanWu {

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
		:m_Name(name)
	{
		XW_PROFILE_FUNCTION();

		std::string vertexSrc = ReadFile(vertexPath);
		std::string fragmentSrc = ReadFile(fragmentPath);
		Compile(vertexSrc, fragmentSrc);
	}

	OpenGLShader::~OpenGLShader()
	{
		XW_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		XW_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			XW_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}

	void OpenGLShader::Compile(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		XW_PROFILE_FUNCTION();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(vertexShader);

			XW_CORE_ERROR("{0}", infoLog.data());
			XW_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			XW_CORE_ERROR("{0}", infoLog.data());
			XW_CORE_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

		m_RendererID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		// Link our program
		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			XW_CORE_ERROR("{0}", infoLog.data());
			XW_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
	}

	void OpenGLShader::Bind() const
	{
		XW_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		XW_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		XW_PROFILE_FUNCTION();

		setInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* value, uint32_t count)
	{
		XW_PROFILE_FUNCTION();

		UploadUniformIntArray(name, value, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		XW_PROFILE_FUNCTION();

		setFloat(name, value);
	}

	void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value)
	{
		XW_PROFILE_FUNCTION();

		setVec3(name, value);
	}
	void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& value)
	{
		XW_PROFILE_FUNCTION();

		setVec4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		XW_PROFILE_FUNCTION();

		setMat4(name, value);
	}

	void OpenGLShader::setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), (int)value);
	}

	void OpenGLShader::setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* value, uint32_t count)
	{
		glUniform1iv(glGetUniformLocation(m_RendererID, name.c_str()), count, value);
	}

	void OpenGLShader::setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}

	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
	}

	void OpenGLShader::setMat3(const std::string& name, const glm::mat3& model) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &model[0][0]);
	}

	void OpenGLShader::setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, glm::value_ptr(value));
	}
	
	void OpenGLShader::setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), x, y, z);
	}

	void OpenGLShader::setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, glm::value_ptr(value));
	}

	void OpenGLShader::setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, glm::value_ptr(value));
	}
}