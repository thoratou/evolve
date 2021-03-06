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

#ifndef EVOLVE_SURFACE_H
#define EVOLVE_SURFACE_H

#include <evolve/core/export.h>
#include <evolve/core/instance.h>

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
		/**
		 * \brief General Instance for Vulkan API handling
		 */
        class EVOLVE_CORE_EXPORT Surface {
        public:
			Surface(const std::shared_ptr<evolve::core::Instance>& iInstancePtr,
					const evolve::core::Window& iWindow);
			~Surface();

			VkSurfaceKHR get() const;

		//non-copyable
		public:
			Surface() = delete;
			Surface(const Surface&) = delete;
			Surface& operator=(const Surface&) = delete;

		private:
			VkSurfaceKHR _surface;
			std::shared_ptr<evolve::core::Instance> _instancePtr;
		};
    }
}

#endif
