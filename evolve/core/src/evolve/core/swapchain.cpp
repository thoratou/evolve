/******************************************************************
* This source file is part of Evolve (Easy VOxeL Vulkan Engine)  *
*                                                                *
* Copyright (c) 2017 Ratouit Thomas                              *
*                                                                *
* Licensed to the Apache Software Foundation (ASF) under one     *
* or more contributor license agreements.  See the NOTICE file   *
* distributed with this work for additional information          *
* regarding copyright ownership.  The ASF licenses this file     *
* to you under the Apache License, Version 2.0 (the              *
* "License"); you may not use this file except in compliance     *
* with the License.  You may obtain a copy of the License at     *
*                                                                *
* http://www.apache.org/licenses/LICENSE-2.0                     *
*                                                                *
* Unless required by applicable law or agreed to in writing,     *
* software distributed under the License is distributed on an    *
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY         *
* KIND, either express or implied.  See the License for the      *
* specific language governing permissions and limitations        *
* under the License.                                             *
******************************************************************/

/**
* \file evolve/core/surface.h
* \brief evolve/core Window Surface
* \author
*
*/

#include <evolve/core/swapchain.h>
#include <evolve/core/window.h>
#include <evolve/core/surface.h>
#include <evolve/core/semaphore.h>
#include <evolve/log/log.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <functional>

/**
 * Namespace for all evolve classes
 */
namespace evolve {
	/**
	* Namespace for graphics and computation
	*/
	namespace core {
		SwapChain::SwapChain(const std::shared_ptr<evolve::core::Instance>& iInstancePtr,
							 const std::shared_ptr<evolve::core::GPUDevices>& iDevices,
							 const evolve::core::Window& iWindow)
			:_swapChain(NULL), _renderPass(NULL), _pipelineLayout(NULL), _graphicsPipeline(NULL), _commandPool(NULL),
			_instancePtr(iInstancePtr),
			_devices(iDevices),
			_imageAvailableSemaphore(iInstancePtr, iDevices),
			_renderFinishedSemaphore(iInstancePtr, iDevices){

			EVOLVE_LOG_DEBUG("Creating Vulkan swapchain");

			recreateSwapchain(iWindow);

			EVOLVE_LOG_DEBUG("Vulkan swapchain created");
		}

		SwapChain::~SwapChain() {
			cleanupSwapchain();

			EVOLVE_LOG_DEBUG("Vulkan swapchain destroyed");
		}

		void SwapChain::cleanupSwapchain() {
			if (_swapChain != NULL) {
				for (size_t i = 0; i < _swapChainFramebuffers.size(); i++) {
					vkDestroyFramebuffer(_devices->getLogicalDevice(), _swapChainFramebuffers[i], nullptr);
				}

				vkFreeCommandBuffers(_devices->getLogicalDevice(), _commandPool, static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());

				vkDestroyPipeline(_devices->getLogicalDevice(), _graphicsPipeline, nullptr);
				vkDestroyPipelineLayout(_devices->getLogicalDevice(), _pipelineLayout, nullptr);
				vkDestroyRenderPass(_devices->getLogicalDevice(), _renderPass, nullptr);

				for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
					vkDestroyImageView(_devices->getLogicalDevice(), _swapChainImageViews[i], nullptr);
				}

				vkDestroyCommandPool(_devices->getLogicalDevice(), _commandPool, nullptr);

				vkDestroySwapchainKHR(_devices->getLogicalDevice(), _swapChain, nullptr);

				EVOLVE_LOG_DEBUG("Vulkan swapchain cleaned-up");
			}
		}

		const VkSurfaceCapabilitiesKHR& SwapChain::getCapabilities() const {
			return _capabilities;
		}
		const std::vector<VkSurfaceFormatKHR>& SwapChain::getFormats() const {
			return _formats;
		}
		const std::vector<VkPresentModeKHR>& SwapChain::getPresentModes() const {
			return _presentModes;
		}

		void SwapChain::recreateSwapchain(const evolve::core::Window& iWindow) {
			//ensure that everything is completed before unallocation
			_devices->waitIdleDevice();

			//clean-up
			cleanupSwapchain();

			EVOLVE_LOG_DEBUG("Recreating swapchain Vulkan swapchain");

			querySwapChainSupport(iWindow.getSurface(), *_devices);

			VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat();
			VkPresentModeKHR presentMode = chooseSwapPresentMode();
			VkExtent2D extent = chooseSwapExtent(iWindow);

			uint32_t imageCount = _capabilities.minImageCount + 1;
			if (_capabilities.maxImageCount > 0 && imageCount > _capabilities.maxImageCount) {
				imageCount = _capabilities.maxImageCount;
			}

			//swap chain creation
			VkSwapchainCreateInfoKHR createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = iWindow.getSurface().get();

			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			uint32_t queueFamilyIndices[] = { _devices->getGraphicsFamily(), _devices->getPresentFamily() };

			if (_devices->getGraphicsFamily() != _devices->getPresentFamily()) {
				createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				createInfo.queueFamilyIndexCount = 2;
				createInfo.pQueueFamilyIndices = queueFamilyIndices;
			}
			else {
				createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			}

			createInfo.preTransform = _capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE;

			if (vkCreateSwapchainKHR(_devices->getLogicalDevice(), &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
				EVOLVE_CRITICAL_EXCEPTION("failed to create swap chain");
			}

			vkGetSwapchainImagesKHR(_devices->getLogicalDevice(), _swapChain, &imageCount, nullptr);
			_swapChainImages.resize(imageCount);
			vkGetSwapchainImagesKHR(_devices->getLogicalDevice(), _swapChain, &imageCount, _swapChainImages.data());

			_swapChainImageFormat = surfaceFormat.format;
			_swapChainExtent = extent;

			//swap chain views
			createSwapChainViews();

			//render pass
			createRenderPass();

			//graphics pipeline
			createGraphicsPipeline();

			//framebuffers
			createFramebuffers();

			//command pool
			createCommandPool();

			//command buffers
			createCommandBuffers();
		}

		void SwapChain::drawFrame(const evolve::core::Window& iWindow) {
			uint32_t aImageIndex;

#undef max
			VkResult result = vkAcquireNextImageKHR(_devices->getLogicalDevice(), _swapChain, std::numeric_limits<uint64_t>::max(), _imageAvailableSemaphore.getSemaphore(), VK_NULL_HANDLE, &aImageIndex);

			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				EVOLVE_LOG_INFO("Out of date swapchain, recreating it");

				recreateSwapchain(iWindow);
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
				EVOLVE_CRITICAL_EXCEPTION("failed to acquire swap chain image!");
			}

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { _imageAvailableSemaphore.getSemaphore() };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &_commandBuffers[aImageIndex];

			VkSemaphore signalSemaphores[] = { _renderFinishedSemaphore.getSemaphore() };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			if (vkQueueSubmit(_devices->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
				EVOLVE_CRITICAL_EXCEPTION("failed to submit draw command buffer");
			}

			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			VkSwapchainKHR swapChains[] = { _swapChain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;

			presentInfo.pImageIndices = &aImageIndex;

			result = vkQueuePresentKHR(_devices->getPresentQueue(), &presentInfo);

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
				recreateSwapchain(iWindow);
				return;
			}
			else if (result != VK_SUCCESS) {
				EVOLVE_CRITICAL_EXCEPTION("failed to present swap chain image");
			}

			_devices->waitIdlePresentQueue();
		}

		void SwapChain::querySwapChainSupport(const evolve::core::Surface& iSurface,
											  const evolve::core::GPUDevices& iDevices) {

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(iDevices.getPhysicalDevice(), iSurface.get(), &_capabilities);

			uint32_t aFormatCount = 0;
			vkGetPhysicalDeviceSurfaceFormatsKHR(iDevices.getPhysicalDevice(), iSurface.get(), &aFormatCount, nullptr);

			if (aFormatCount != 0) {
				_formats.resize(aFormatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(iDevices.getPhysicalDevice(), iSurface.get(), &aFormatCount, _formats.data());
			}

			uint32_t aPresentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(iDevices.getPhysicalDevice(), iSurface.get(), &aPresentModeCount, nullptr);

			if (aPresentModeCount != 0) {
				_presentModes.resize(aPresentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(iDevices.getPhysicalDevice(), iSurface.get(), &aPresentModeCount, _presentModes.data());
			}
		}

		VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat() {
			if (_formats.size() == 1 && _formats[0].format == VK_FORMAT_UNDEFINED) {
				return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
			}

			for (const auto& aFormat : _formats) {
				if (aFormat.format == VK_FORMAT_B8G8R8A8_UNORM && aFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return aFormat;
				}
			}

			return _formats[0];
		}

		VkPresentModeKHR SwapChain::chooseSwapPresentMode() {
			VkPresentModeKHR aBestMode = VK_PRESENT_MODE_FIFO_KHR;

			for (const auto& aPresentMode : _presentModes) {
				if (aPresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return aPresentMode;
				}
				else if (aPresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
					aBestMode = aPresentMode;
				}
			}

			return aBestMode;
		}

		VkExtent2D SwapChain::chooseSwapExtent(const evolve::core::Window& iWindow) {
#undef max
#undef min
			if (_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
				return _capabilities.currentExtent;
			}
			else {
				int aWidth, aHeight;
				glfwGetWindowSize(iWindow.get(), &aWidth, &aHeight);

				VkExtent2D actualExtent = {
					static_cast<uint32_t>(aWidth),
					static_cast<uint32_t>(aHeight)
				};

				actualExtent.width = std::max(_capabilities.minImageExtent.width, std::min(_capabilities.maxImageExtent.width, actualExtent.width));
				actualExtent.height = std::max(_capabilities.minImageExtent.height, std::min(_capabilities.maxImageExtent.height, actualExtent.height));

				return actualExtent;
			}
		}

		void SwapChain::createSwapChainViews() {
			_swapChainImageViews.resize(_swapChainImages.size());

			for (size_t i = 0; i < _swapChainImages.size(); i++) {
				VkImageViewCreateInfo aCreateInfo = {};
				aCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				aCreateInfo.image = _swapChainImages[i];
				aCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				aCreateInfo.format = _swapChainImageFormat;
				aCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				aCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				aCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				aCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
				aCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				aCreateInfo.subresourceRange.baseMipLevel = 0;
				aCreateInfo.subresourceRange.levelCount = 1;
				aCreateInfo.subresourceRange.baseArrayLayer = 0;
				aCreateInfo.subresourceRange.layerCount = 1;

				if (vkCreateImageView(_devices->getLogicalDevice(), &aCreateInfo, nullptr, &_swapChainImageViews[i]) != VK_SUCCESS) {
					EVOLVE_CRITICAL_EXCEPTION("failed to create image views");
				}
			}
		}

		void SwapChain::createRenderPass() {
			VkAttachmentDescription aColorAttachment = {};
			aColorAttachment.format = _swapChainImageFormat;
			aColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			aColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			aColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			aColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			aColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			aColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			aColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference aColorAttachmentRef = {};
			aColorAttachmentRef.attachment = 0;
			aColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription aSubpass = {};
			aSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			aSubpass.colorAttachmentCount = 1;
			aSubpass.pColorAttachments = &aColorAttachmentRef;

			VkSubpassDependency aDependency = {};
			aDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			aDependency.dstSubpass = 0;
			aDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			aDependency.srcAccessMask = 0;
			aDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			aDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo aRenderPassInfo = {};
			aRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			aRenderPassInfo.attachmentCount = 1;
			aRenderPassInfo.pAttachments = &aColorAttachment;
			aRenderPassInfo.subpassCount = 1;
			aRenderPassInfo.pSubpasses = &aSubpass;
			aRenderPassInfo.dependencyCount = 1;
			aRenderPassInfo.pDependencies = &aDependency;

			if (vkCreateRenderPass(_devices->getLogicalDevice(), &aRenderPassInfo, nullptr, &_renderPass) != VK_SUCCESS) {
				EVOLVE_CRITICAL_EXCEPTION("failed to create render pass");
			}
		}

		VkShaderModule createShaderModule(const evolve::core::GPUDevices& iDevices, const std::vector<char>& code) {
			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

			VkShaderModule shaderModule;
			if (vkCreateShaderModule(iDevices.getLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
				EVOLVE_CRITICAL_EXCEPTION("failed to create shader module");
			}

			return shaderModule;
		}

		static std::vector<char> readFile(const std::string& filename) {
			std::ifstream file(filename, std::ios::ate | std::ios::binary);

			if (!file.is_open()) {
				std::stringstream aErr;
				aErr << "failed to open file " << filename;
				EVOLVE_CRITICAL_EXCEPTION(aErr.str());
			}

			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);

			file.close();

			return buffer;
		}

		void SwapChain::createGraphicsPipeline() {
			auto aVertShaderCode = readFile("../shaders/vert.spv");
			auto aFragShaderCode = readFile("../shaders/frag.spv");

			VkShaderModule aVertShaderModule = createShaderModule(*_devices, aVertShaderCode);
			VkShaderModule aFragShaderModule = createShaderModule(*_devices, aFragShaderCode);

			VkPipelineShaderStageCreateInfo aVertShaderStageInfo = {};
			aVertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			aVertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			aVertShaderStageInfo.module = aVertShaderModule;
			aVertShaderStageInfo.pName = "main";

			VkPipelineShaderStageCreateInfo aFragShaderStageInfo = {};
			aFragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			aFragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			aFragShaderStageInfo.module = aFragShaderModule;
			aFragShaderStageInfo.pName = "main";

			VkPipelineShaderStageCreateInfo aShaderStages[] = { aVertShaderStageInfo, aFragShaderStageInfo };

			VkPipelineVertexInputStateCreateInfo aVertexInputInfo = {};
			aVertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			aVertexInputInfo.vertexBindingDescriptionCount = 0;
			aVertexInputInfo.vertexAttributeDescriptionCount = 0;

			VkPipelineInputAssemblyStateCreateInfo aInputAssembly = {};
			aInputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			aInputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			aInputAssembly.primitiveRestartEnable = VK_FALSE;

			VkViewport aViewport = {};
			aViewport.x = 0.0f;
			aViewport.y = 0.0f;
			aViewport.width = (float)_swapChainExtent.width;
			aViewport.height = (float)_swapChainExtent.height;
			aViewport.minDepth = 0.0f;
			aViewport.maxDepth = 1.0f;

			VkRect2D aScissor = {};
			aScissor.offset = { 0, 0 };
			aScissor.extent = _swapChainExtent;

			VkPipelineViewportStateCreateInfo aViewportState = {};
			aViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			aViewportState.viewportCount = 1;
			aViewportState.pViewports = &aViewport;
			aViewportState.scissorCount = 1;
			aViewportState.pScissors = &aScissor;

			VkPipelineRasterizationStateCreateInfo aRasterizer = {};
			aRasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			aRasterizer.depthClampEnable = VK_FALSE;
			aRasterizer.rasterizerDiscardEnable = VK_FALSE;
			aRasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			aRasterizer.lineWidth = 1.0f;
			aRasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			aRasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
			aRasterizer.depthBiasEnable = VK_FALSE;

			VkPipelineMultisampleStateCreateInfo aMultisampling = {};
			aMultisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			aMultisampling.sampleShadingEnable = VK_FALSE;
			aMultisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

			VkPipelineColorBlendAttachmentState aColorBlendAttachment = {};
			aColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			aColorBlendAttachment.blendEnable = VK_FALSE;

			VkPipelineColorBlendStateCreateInfo aColorBlending = {};
			aColorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			aColorBlending.logicOpEnable = VK_FALSE;
			aColorBlending.logicOp = VK_LOGIC_OP_COPY;
			aColorBlending.attachmentCount = 1;
			aColorBlending.pAttachments = &aColorBlendAttachment;
			aColorBlending.blendConstants[0] = 0.0f;
			aColorBlending.blendConstants[1] = 0.0f;
			aColorBlending.blendConstants[2] = 0.0f;
			aColorBlending.blendConstants[3] = 0.0f;

			VkPipelineLayoutCreateInfo aPipelineLayoutInfo = {};
			aPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			aPipelineLayoutInfo.setLayoutCount = 0;
			aPipelineLayoutInfo.pushConstantRangeCount = 0;

			if (vkCreatePipelineLayout(_devices->getLogicalDevice(), &aPipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
				EVOLVE_CRITICAL_EXCEPTION("failed to create pipeline layout");
			}

			VkGraphicsPipelineCreateInfo aPipelineInfo = {};
			aPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			aPipelineInfo.stageCount = 2;
			aPipelineInfo.pStages = aShaderStages;
			aPipelineInfo.pVertexInputState = &aVertexInputInfo;
			aPipelineInfo.pInputAssemblyState = &aInputAssembly;
			aPipelineInfo.pViewportState = &aViewportState;
			aPipelineInfo.pRasterizationState = &aRasterizer;
			aPipelineInfo.pMultisampleState = &aMultisampling;
			aPipelineInfo.pColorBlendState = &aColorBlending;
			aPipelineInfo.layout = _pipelineLayout;
			aPipelineInfo.renderPass = _renderPass;
			aPipelineInfo.subpass = 0;
			aPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

			if (vkCreateGraphicsPipelines(_devices->getLogicalDevice(), VK_NULL_HANDLE, 1, &aPipelineInfo, nullptr, &_graphicsPipeline) != VK_SUCCESS) {
				EVOLVE_CRITICAL_EXCEPTION("failed to create graphics pipeline");
			}

			vkDestroyShaderModule(_devices->getLogicalDevice(), aFragShaderModule, nullptr);
			vkDestroyShaderModule(_devices->getLogicalDevice(), aVertShaderModule, nullptr);

		}

		void SwapChain::createFramebuffers() {
			_swapChainFramebuffers.resize(_swapChainImageViews.size());

			for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
				VkImageView aAttachments[] = {
					_swapChainImageViews[i]
				};

				VkFramebufferCreateInfo aFramebufferInfo = {};
				aFramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				aFramebufferInfo.renderPass = _renderPass;
				aFramebufferInfo.attachmentCount = 1;
				aFramebufferInfo.pAttachments = aAttachments;
				aFramebufferInfo.width = _swapChainExtent.width;
				aFramebufferInfo.height = _swapChainExtent.height;
				aFramebufferInfo.layers = 1;

				if (vkCreateFramebuffer(_devices->getLogicalDevice(), &aFramebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS) {
					EVOLVE_CRITICAL_EXCEPTION("failed to create framebuffer");
				}
			}

		}

		void SwapChain::createCommandPool() {
			VkCommandPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = _devices->getGraphicsFamily();

			if (vkCreateCommandPool(_devices->getLogicalDevice(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
				EVOLVE_CRITICAL_EXCEPTION("failed to create command pool");
			}
		}

		void SwapChain::createCommandBuffers() {
			_commandBuffers.resize(_swapChainFramebuffers.size());

			VkCommandBufferAllocateInfo aAllocInfo = {};
			aAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			aAllocInfo.commandPool = _commandPool;
			aAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			aAllocInfo.commandBufferCount = (uint32_t)_commandBuffers.size();

			if (vkAllocateCommandBuffers(_devices->getLogicalDevice(), &aAllocInfo, _commandBuffers.data()) != VK_SUCCESS) {
				EVOLVE_CRITICAL_EXCEPTION("failed to allocate command buffers");
			}

			for (size_t i = 0; i < _commandBuffers.size(); i++) {
				VkCommandBufferBeginInfo aBeginInfo = {};
				aBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				aBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

				vkBeginCommandBuffer(_commandBuffers[i], &aBeginInfo);

				VkRenderPassBeginInfo aRenderPassInfo = {};
				aRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				aRenderPassInfo.renderPass = _renderPass;
				aRenderPassInfo.framebuffer = _swapChainFramebuffers[i];
				aRenderPassInfo.renderArea.offset = { 0, 0 };
				aRenderPassInfo.renderArea.extent = _swapChainExtent;

				VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
				aRenderPassInfo.clearValueCount = 1;
				aRenderPassInfo.pClearValues = &clearColor;

				vkCmdBeginRenderPass(_commandBuffers[i], &aRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);

				vkCmdDraw(_commandBuffers[i], 3, 1, 0, 0);

				vkCmdEndRenderPass(_commandBuffers[i]);

				if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
					EVOLVE_CRITICAL_EXCEPTION("failed to record command buffer");
				}
			}
		}
	}
}
