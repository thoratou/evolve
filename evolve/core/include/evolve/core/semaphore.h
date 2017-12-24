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
 * \file evolve/core/semaphore.h
 * \brief evolve/core Semaphore Handling
 * \author
 *
 */

#ifndef EVOLVE_SEMAPHORE_H
#define EVOLVE_SEMAPHORE_H

#include <evolve/core/export.h>

#include <vulkan/vulkan.h>
#include <evolve/core/instance.h>
#include <evolve/core/gpudevices.h>

#include <memory>

/**
 * Namespace for all evolve classes
 */
namespace evolve {
	/**
	 * Namespace for graphics and computation 
	 */
	namespace core {

		/**
		 * \brief Semaphore for Vulkan API handling
		 */
        class EVOLVE_CORE_EXPORT Semaphore {
        public:
			Semaphore(const std::shared_ptr<evolve::core::Instance>& iInstancePtr,
				const std::shared_ptr<evolve::core::GPUDevices>& iDevices);
			~Semaphore();

			VkSemaphore getSemaphore() const;

		//non-copyable
		public:
			Semaphore() = delete;
			Semaphore(const Semaphore&) = delete;
			Semaphore& operator=(const Semaphore&) = delete;

		private:
			std::shared_ptr<evolve::core::Instance> _instancePtr;
			std::shared_ptr<evolve::core::GPUDevices> _devices;
			VkSemaphore _semaphore;
		};
    }
}

#endif
