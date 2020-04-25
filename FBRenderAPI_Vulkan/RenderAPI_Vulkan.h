#pragma once
#include "../FBRenderAPI/RenderAPI.h"

extern "C" {
	MPGE_VULKAN_DLL fb::RenderAPI* Initialize(fb::RenderAPI::eResult* ret, fb::InitInfo* initInfo);
}

namespace fb
{
	class MPGEVulkan : public RenderAPI
	{
		MPGEVulkan(InitInfo* initInfo);
		~MPGEVulkan();

		mutable RenderAPI::eResult LastResult = RenderAPI::eResult::Success;
		HMODULE VulkanModule;
		vk::Instance vkInst;

	public:
		static MPGEVulkan* Initialize(InitInfo* initInfo);
		void Finalize() override;

		void CreateInstance(InitInfo* initInfo);
		RenderAPI::eResult GetLastResult() const { return LastResult; }

		std::vector<PhysicalDeviceProperties> GetGPUs() const override;
		// public functions end

	private:
		bool IsSupportedLayer(const char* layerName) const;

		
		
	};
}