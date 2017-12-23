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
 * \file evolve/core/window.cpp
 * \brief evolve/core policy source file
 * \author
 *
 */

#include <evolve/core/window.h>
#include <evolve/core/instance.h>
#include <evolve/log/log.h>
#include <functional>

/**
 * Namespace for all evolve classes
 */
namespace evolve {
	/**
	* Namespace for graphics and computation
	*/
	namespace core {
		Window::Window(int iWidth, int iHeight){
			using namespace std::placeholders;

			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			_window = glfwCreateWindow(iWidth, iHeight, "Vulkan", nullptr, nullptr);

			glfwSetWindowUserPointer(_window, this);

			glfwSetWindowSizeCallback(_window, &Window::_onResize);
		}

		Window::~Window() {
			glfwDestroyWindow(_window);

			glfwTerminate();
		}

		void Window::createSurface(const std::shared_ptr<evolve::core::Instance>& iInstancePtr) {
			_surfacePtr = std::unique_ptr<evolve::core::Surface>(new evolve::core::Surface(iInstancePtr, *this));
		}

		void Window::destroySurface() {
			_surfacePtr.release();
		}

		const evolve::core::Surface& Window::getSurface() const {
			EVOLVE_CRITICAL_EXCEPTION_IF(!_surfacePtr, "Vulkan surface not initialized, please call Window::createSurface first");
			return *_surfacePtr.get();
		}
		evolve::core::Surface& Window::accessSurface() {
			EVOLVE_CRITICAL_EXCEPTION_IF(!_surfacePtr, "Vulkan surface not initialized, please call Window::createSurface first");
			return *_surfacePtr.get();
		}

		GLFWwindow* Window::get() const {
			return _window;
		}

		bool Window::shouldClose() const {
			return glfwWindowShouldClose(_window);
		}

		void Window::pollEvents() const {
			glfwPollEvents();
		}

		void Window::_onResize(GLFWwindow* ioWindow, int iWidth, int iHeight) {
			if (iWidth == 0 || iHeight == 0) {
				EVOLVE_LOG_WARNING("trying to resize window this with or height set to 0, ignored");
				return;
			}

			Window* this_ = reinterpret_cast<Window*>(glfwGetWindowUserPointer(ioWindow));
			this_->onResize(iWidth, iHeight);

		}
	}
}
