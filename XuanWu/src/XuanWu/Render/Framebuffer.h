#pragma once

#include "XuanWu/Core/Core.h"

namespace XuanWu
{
	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

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

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}