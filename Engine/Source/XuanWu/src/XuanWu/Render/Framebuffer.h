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
		//用shared_ptr共享指针的话，可以不用把基类的析构函数写成虚函数，也可以不用写析构函数，因为 std::shared_ptr 默认使用 delete 运算符来释放对象内存，从而调用了 派生类 的析构函数和 基类 的析构函数。
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