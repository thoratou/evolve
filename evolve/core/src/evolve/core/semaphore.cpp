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
* \file evolve/core/semaphore.cpp
* \brief evolve/core Semaphore Handling
* \author
*
*/

#include <evolve/core/semaphore.h>
#include <evolve/log/log.h>

/**
 * Namespace for all evolve classes
 */
namespace evolve {
	/**
	* Namespace for graphics and computation
	*/
	namespace core {
		Semaphore::Semaphore(const std::shared_ptr<evolve::core::Instance>& iInstancePtr,
			const std::shared_ptr<evolve::core::GPUDevices>& iDevices)
			:_instancePtr(iInstancePtr),
			_devices(iDevices),
			_semaphore(VK_NULL_HANDLE){

			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			if (vkCreateSemaphore(_devices->getLogicalDevice(), &semaphoreInfo, nullptr, &_semaphore) != VK_SUCCESS) {
				EVOLVE_CRITICAL_EXCEPTION("failed to create semaphore");
			}
		}

		Semaphore::~Semaphore() {

			vkDestroySemaphore(_devices->getLogicalDevice(), _semaphore, nullptr);
		}

		VkSemaphore Semaphore::getSemaphore() const {
			return _semaphore;
		}
	}
}
