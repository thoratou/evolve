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
 * \file evolve/utils/Policies.cpp
 * \brief evolve/utils policy source file
 * \author
 *
 */

#include <evolve/core/instance.h>
#include <evolve/log/log.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/**
 * Namespace for all evolve classes
 */
namespace evolve {
	/**
	* Namespace for graphics and computation
	*/
	namespace core {
		Instance::Instance(
			const std::string& iApplicationName,
			const std::shared_ptr<evolve::core::ValidationLayers>& ioValidationLayers)
				:_applicationName(iApplicationName), _validationLayers(ioValidationLayers) {

			VkApplicationInfo aApplicationInfo = {};
			aApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			aApplicationInfo.pApplicationName = _applicationName.c_str();
			aApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			aApplicationInfo.pEngineName = "No Engine";
			aApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			aApplicationInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo aCreateInfo = {};
			aCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			aCreateInfo.pApplicationInfo = &aApplicationInfo;

			retrieveRequiredExtensions();
			aCreateInfo.enabledExtensionCount = static_cast<uint32_t>(_extensions.size());
			aCreateInfo.ppEnabledExtensionNames = _extensions.data();

			aCreateInfo.enabledLayerCount = _validationLayers->size();
			aCreateInfo.ppEnabledLayerNames = _validationLayers->data();

			if (vkCreateInstance(&aCreateInfo, nullptr, &_instance) != VK_SUCCESS) {
				EVOLVE_CRITICAL_EXCEPTION("failed to create Vulkan instance");
			}

			_validationLayers->setupDebugCall(*this);
		}


		Instance::~Instance() {
			_validationLayers->destroyDebugCall(*this);
			vkDestroyInstance(_instance, nullptr);
		}

		const VkInstance& Instance::get() const {
			return _instance;
		}

		const std::vector<const char*>& Instance::getExtensions() const {
			return _extensions;
		}
		std::vector<const char*>& Instance::accessExtensions() {
			return _extensions;
		}

		const evolve::core::ValidationLayers& Instance::getValidationLayers() const {
			return *_validationLayers.get();
		}


		void Instance::retrieveRequiredExtensions() {

			unsigned int aGlfwExtensionCount = 0;
			const char** aGlfwExtensions;
			aGlfwExtensions = glfwGetRequiredInstanceExtensions(&aGlfwExtensionCount);

			for (unsigned int i = 0; i < aGlfwExtensionCount; i++) {
				_extensions.push_back(aGlfwExtensions[i]);
			}

			_validationLayers->updateExtensions(*this);
		}
	}
}
