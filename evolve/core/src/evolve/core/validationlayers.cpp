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

#include <evolve/core/validationlayers.h>
#include <evolve/core/instance.h>
#include <evolve/log/log.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT iFlags, VkDebugReportObjectTypeEXT iObjType, uint64_t iObj, size_t iLocation, int32_t iCode, const char* iLayerPrefix, const char* iMsg, void* iUserData) {
	EVOLVE_LOG_ERROR("validation layer: " << iMsg);

	return VK_FALSE;
}

static VkResult CreateDebugReportCallbackEXT(VkInstance iInstance, const VkDebugReportCallbackCreateInfoEXT* iCreateInfo, const VkAllocationCallbacks* iAllocator, VkDebugReportCallbackEXT* iCallback) {
	PFN_vkCreateDebugReportCallbackEXT func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(iInstance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) {
		return func(iInstance, iCreateInfo, iAllocator, iCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

/**
 * Namespace for all evolve classes
 */
namespace evolve {
	/**
	* Namespace for graphics and computation
	*/
	namespace core {
		ValidationLayers::ValidationLayers()
#ifndef NDEBUG
			:_validationLayers({ "VK_LAYER_LUNARG_standard_validation" }) {
			//check validation layer availability
			uint32_t aLayerCount = 0;
			vkEnumerateInstanceLayerProperties(&aLayerCount, nullptr);

			std::vector<VkLayerProperties> aAvailableLayers(aLayerCount);
			vkEnumerateInstanceLayerProperties(&aLayerCount, aAvailableLayers.data());

			for (const char* aLayerName : _validationLayers) {
				bool aLayerFound = false;

				for (const auto& aLayerProperties : aAvailableLayers) {
					if (strcmp(aLayerName, aLayerProperties.layerName) == 0) {
						aLayerFound = true;
						break;
					}
				}

				EVOLVE_CRITICAL_EXCEPTION_IF(!aLayerFound, "failed to setup Vulkan validation layers, not available");
			}
		}
#else
			:_validationLayers() {}
#endif


		ValidationLayers::~ValidationLayers() {}

		bool ValidationLayers::isEnabled() const {
#ifndef NDEBUG
			return true;
#else
			return false;
#endif
		}


		uint32_t ValidationLayers::size() const {
			return static_cast<uint32_t>(_validationLayers.size());
		}

		const std::vector<const char*>::value_type* ValidationLayers::data() const {
			return _validationLayers.empty() ? nullptr : _validationLayers.data();
		}

		void ValidationLayers::updateExtensions(evolve::core::Instance& iInstance) {
#ifndef NDEBUG
			iInstance.accessExtensions().push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif
		}

		void ValidationLayers::setupDebugCall(const evolve::core::Instance& iInstance) {
#ifndef NDEBUG
			VkDebugReportCallbackCreateInfoEXT aCreateInfo = {};
			aCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
			aCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
			aCreateInfo.pfnCallback = debugCallback;

			if (CreateDebugReportCallbackEXT(iInstance.get(), &aCreateInfo, nullptr, &_callback) != VK_SUCCESS) {
				EVOLVE_CRITICAL_EXCEPTION("failed to setup Vulkan debug callback");
			}
#endif
		}

		void ValidationLayers::destroyDebugCall(const evolve::core::Instance& iInstance) {
#ifndef NDEBUG
			auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(iInstance.get(), "vkDestroyDebugReportCallbackEXT");
			if (func != nullptr) {
				func(iInstance.get(), _callback, nullptr);
			}
#endif
		}
	}
}
