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
 * \file evolve/core/Window.h
 * \brief evolve/core General Instance for Vulkan API handling
 * \author
 *
 */

#ifndef EVOLVE_WINDOW_H
#define EVOLVE_WINDOW_H

#include <evolve/core/surface.h>
#include <evolve/core/export.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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
		class Instance;

		/**
		 * \brief General Instance for Vulkan API handling
		 */
        class EVOLVE_CORE_EXPORT Window {
        public:
			Window(int iWidth, int iHeight);
			virtual ~Window();

			void createSurface(const std::shared_ptr<evolve::core::Instance>& iInstancePtr);
			void destroySurface();

			const evolve::core::Surface& getSurface() const;
			evolve::core::Surface& accessSurface();

			bool shouldClose() const;
			void pollEvents() const;

			virtual void onResize(int iWidth, int iHeight) = 0;

			GLFWwindow* get() const;
		protected:
			GLFWwindow* _window;
			std::unique_ptr<evolve::core::Surface> _surfacePtr;
		private:
			static void _onResize(GLFWwindow* ioWindow, int iWidth, int iHeight);
		};
    }
}

#endif
