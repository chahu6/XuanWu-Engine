#pragma once

#include "XuanWu/Core/Core.h"

namespace XuanWu
{
	enum class FramebufferTextureFormat : uint8_t
	{
		None = 0,

		// Color
		RGBA8,

		RED_INTEGER,

		// Depth/stencil
		DEPTH24TENCIL8,

		// Defaults
		Depth = DEPTH24TENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		FramebufferAttachmentSpecification Attachments;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		//��shared_ptr����ָ��Ļ������Բ��ðѻ������������д���麯����Ҳ���Բ���д������������Ϊ std::shared_ptr Ĭ��ʹ�� delete ��������ͷŶ����ڴ棬�Ӷ������� ������ ������������ ���� ������������
		virtual ~Framebuffer() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual uint32_t GetDepthAttachmentRendererID() const = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}