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
 * \file evolve/core/physicaldevices.h
 * \brief evolve/core Physical Devices Detection
 * \author
 *
 */

#ifndef EVOLVE_PHYSICAL_DEVICES_H
#define EVOLVE_PHYSICAL_DEVICES_H

#include <evolve/core/export.h>

#include <vulkan/vulkan.h>
#include <evolve/core/instance.h>

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
		class Surface;
		class Window;

		/**
		 * \brief General Instance for Vulkan API handling
		 */
        class EVOLVE_CORE_EXPORT GPUDevices {
        public:
			GPUDevices(const std::shared_ptr<evolve::core::Instance>& iInstancePtr,
				const evolve::core::Window& iWindow);
			~GPUDevices();

			VkPhysicalDevice getPhysicalDevice() const;
			VkDevice getLogicalDevice() const;
			void waitIdleDevice() const;

			uint32_t getGraphicsFamily() const;
			uint32_t getPresentFamily() const;

			VkQueue getGraphicsQueue() const;
			VkQueue getPresentQueue() const;

			void waitIdleGraphicsQueue() const;
			void waitIdlePresentQueue() const;

		private:
			VkPhysicalDevice _physicalDevice;
			VkDevice _logicalDevice;

			std::shared_ptr<evolve::core::Instance> _instancePtr;
			const std::vector<const char*> _deviceExtensions;

			VkQueue _graphicsQueue;
			VkQueue _presentQueue;

			bool isDeviceSuitable(VkPhysicalDevice iDevice, const evolve::core::Window& iWindow);
			void findQueueFamilies(VkPhysicalDevice device, const evolve::core::Surface& iSurface);
			bool checkDeviceExtensionSupport(VkPhysicalDevice device);

			bool isFamilyComplete() const;

			uint32_t _graphicsFamily;
			uint32_t _presentFamily;
		};
    }
}

#endif
