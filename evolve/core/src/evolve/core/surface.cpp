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

#include <evolve/core/surface.h>
#include <evolve/core/window.h>
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
		Surface::Surface(const std::shared_ptr<evolve::core::Instance>& iInstancePtr,
						 const evolve::core::Window& iWindow)
			:_instancePtr(iInstancePtr) {
			if (glfwCreateWindowSurface(_instancePtr->get(), iWindow.get(), nullptr, &_surface) != VK_SUCCESS) {
				EVOLVE_CRITICAL_EXCEPTION("failed to create window surface");
			}
		}
		Surface::~Surface() {
			vkDestroySurfaceKHR(_instancePtr->get(), _surface, nullptr);
		}

		VkSurfaceKHR Surface::get() const {
			return _surface;
		}

	}
}
