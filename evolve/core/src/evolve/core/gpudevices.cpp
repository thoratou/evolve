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
* \file evolve/core/physicaldevices.cpp
* \brief evolve/core Physical Devices Detection
* \author
*
*/

#include <evolve/core/gpudevices.h>
#include <evolve/core/instance.h>
#include <evolve/core/window.h>
#include <evolve/core/surface.h>
#include <evolve/core/swapchain.h>
#include <evolve/log/log.h>
#include <functional>
#include <set>

/**
 * Namespace for all evolve classes
 */
namespace evolve {
	/**
	* Namespace for graphics and computation
	*/
	namespace core {
		GPUDevices::GPUDevices(const std::shared_ptr<evolve::core::Instance>& iInstancePtr,
				const evolve::core::Window& iWindow)
			:_instancePtr(iInstancePtr),
			 _physicalDevice(VK_NULL_HANDLE),
			 _deviceExtensions({VK_KHR_SWAPCHAIN_EXTENSION_NAME}),
			_graphicsFamily(-1),
			_presentFamily(-1){

			//retrieve physical GPU device
			uint32_t aDeviceCount = 0;
			vkEnumeratePhysicalDevices(_instancePtr->get(), &aDeviceCount, nullptr);

			EVOLVE_CRITICAL_EXCEPTION_IF(aDeviceCount == 0, "failed to find GPUs with Vulkan support");

			std::vector<VkPhysicalDevice> aDevices(aDeviceCount);
			vkEnumeratePhysicalDevices(_instancePtr->get(), &aDeviceCount, aDevices.data());

			for (const VkPhysicalDevice& aDevice : aDevices) {
				if (isDeviceSuitable(aDevice, iWindow)) {
					_physicalDevice = aDevice;
					break;
				}
			}

			EVOLVE_CRITICAL_EXCEPTION_IF(_physicalDevice == VK_NULL_HANDLE, "failed to find a suitable GPU");

			//retrieve logical GPU device
			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::set<uint32_t> uniqueQueueFamilies = { _graphicsFamily, _presentFamily };

			float queuePriority = 1.0f;
			for (uint32_t queueFamily : uniqueQueueFamilies) {
				VkDeviceQueueCreateInfo queueCreateInfo = {};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				queueCreateInfos.push_back(queueCreateInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures = {};

			VkDeviceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
			createInfo.pQueueCreateInfos = queueCreateInfos.data();

			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = static_cast<uint32_t>(_deviceExtensions.size());
			createInfo.ppEnabledExtensionNames = _deviceExtensions.data();

			createInfo.enabledLayerCount = _instancePtr->getValidationLayers().size();
			createInfo.ppEnabledLayerNames = _instancePtr->getValidationLayers().data();

			if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_logicalDevice) != VK_SUCCESS) {
				throw std::runtime_error("failed to create logical device!");
			}

			vkGetDeviceQueue(_logicalDevice, _graphicsFamily, 0, &_graphicsQueue);
			vkGetDeviceQueue(_logicalDevice, _presentFamily, 0, &_presentQueue);
		}

		GPUDevices::~GPUDevices() {

			vkDestroyDevice(_logicalDevice, nullptr);
		}

		VkPhysicalDevice GPUDevices::getPhysicalDevice() const {
			return _physicalDevice;
		}

		VkDevice GPUDevices::getLogicalDevice() const {
			return _logicalDevice;
		}

		void GPUDevices::waitIdleDevice() const {
			vkDeviceWaitIdle(_logicalDevice);
		}

		uint32_t GPUDevices::getGraphicsFamily() const {
			return _graphicsFamily;
		}

		uint32_t GPUDevices::getPresentFamily() const {
			return _presentFamily;
		}


		VkQueue GPUDevices::getGraphicsQueue() const {
			return _graphicsQueue;
		}
		VkQueue GPUDevices::getPresentQueue() const {
			return _presentQueue;
		}

		void GPUDevices::waitIdleGraphicsQueue() const {
			vkQueueWaitIdle(_graphicsQueue);
		}

		void GPUDevices::waitIdlePresentQueue() const {
			vkQueueWaitIdle(_presentQueue);
		}

		bool GPUDevices::isDeviceSuitable(VkPhysicalDevice iDevice, const evolve::core::Window& iWindow) {
			findQueueFamilies(iDevice, iWindow.getSurface());

			bool extensionsSupported = checkDeviceExtensionSupport(iDevice);

			bool swapChainAdequate = false;
			if (extensionsSupported) {
				//to fix
				//std::unique_ptr<evolve::core::SwapChain> aSwapchain(new evolve::core::SwapChain(_instancePtr, iWindow, *this);

				//swapChainAdequate = !_swapchain->getFormats().empty() && !_swapchain->getPresentModes().empty();
				swapChainAdequate = true;
			}

			return isFamilyComplete() && extensionsSupported && swapChainAdequate;
		}

		void GPUDevices::findQueueFamilies(VkPhysicalDevice iDevice, const evolve::core::Surface& iSurface) {

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(iDevice, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(iDevice, &queueFamilyCount, queueFamilies.data());

			int i = 0;
			for (const auto& queueFamily : queueFamilies) {
				if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					_graphicsFamily = i;
				}

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(iDevice, i, iSurface.get(), &presentSupport);

				if (queueFamily.queueCount > 0 && presentSupport) {
					_presentFamily = i;
				}

				if (isFamilyComplete()) {
					break;
				}

				i++;
			}
		}

		bool GPUDevices::checkDeviceExtensionSupport(VkPhysicalDevice device) {
			uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

			std::set<std::string> requiredExtensions(_deviceExtensions.begin(), _deviceExtensions.end());

			for (const auto& extension : availableExtensions) {
				requiredExtensions.erase(extension.extensionName);
			}

			return requiredExtensions.empty();
		}

		bool GPUDevices::isFamilyComplete() const {
			return _graphicsFamily >= 0 && _presentFamily >= 0;
		}

	}
}
