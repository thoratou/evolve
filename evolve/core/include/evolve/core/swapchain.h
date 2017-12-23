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

#ifndef EVOLVE_SWAP_CHAIN_H
#define EVOLVE_SWAP_CHAIN_H

#include <evolve/core/export.h>
#include <evolve/core/instance.h>
#include <evolve/core/gpudevices.h>

#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <memory>

/**
 * Namespace for all evolve classes
 */
namespace evolve {
	/**
	 * Namespace for graphics and computation 
	 */
	namespace core {
		class Window;
		class Surface;
		class Semaphore;

		/**
		 * \brief General Instance for Vulkan API handling
		 */
        class EVOLVE_CORE_EXPORT SwapChain {
        public:
			SwapChain(const std::shared_ptr<evolve::core::Instance>& iInstancePtr,
				const std::shared_ptr<evolve::core::GPUDevices>& iDevices,
				const evolve::core::Window& iWindow);
			~SwapChain();

			const VkSurfaceCapabilitiesKHR& getCapabilities() const;
			const std::vector<VkSurfaceFormatKHR>& getFormats() const;
			const std::vector<VkPresentModeKHR>& getPresentModes() const;

			void recreateSwapchain();

			void drawFrame();

			VkResult acquireNextImage(const evolve::core::Semaphore& iSemaphore, uint32_t& oImageIndex);


		private:
			VkSwapchainKHR _swapChain;
			std::vector<VkImage> _swapChainImages;

			void querySwapChainSupport(const evolve::core::Surface& iSurface,
				const evolve::core::GPUDevices& iDevices);

			VkSurfaceCapabilitiesKHR _capabilities;

			std::vector<VkSurfaceFormatKHR> _formats;
			VkFormat _swapChainImageFormat;
			VkSurfaceFormatKHR chooseSwapSurfaceFormat();

			std::vector<VkPresentModeKHR> _presentModes;
			VkPresentModeKHR chooseSwapPresentMode();

			VkExtent2D _swapChainExtent;
			VkExtent2D chooseSwapExtent(const evolve::core::Window& iWindow);

			void cleanupSwapchain();

			std::vector<VkImageView> _swapChainImageViews;
			void recreateSwapChainViews();

			VkRenderPass _renderPass;
			void recreateRenderPass();

			VkPipelineLayout _pipelineLayout;
			VkPipeline _graphicsPipeline;
			void recreateGraphicsPipeline();

			std::vector<VkFramebuffer> _swapChainFramebuffers;
			void recreateFramebuffers();

			VkCommandPool _commandPool;
			void recreateCommandPool();

			std::vector<VkCommandBuffer> _commandBuffers;
			void recreateCommandBuffers();

			std::shared_ptr<evolve::core::Instance> _instancePtr;
			std::shared_ptr<evolve::core::GPUDevices> _devices;
		};
    }
}

#endif
