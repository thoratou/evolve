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
 * \file evolve/core/Instance.h
 * \brief evolve/core General Instance for Vulkan API handling
 * \author
 *
 */

#ifndef EVOLVE_INSTANCE_H
#define EVOLVE_INSTANCE_H

#include <evolve/core/export.h>
#include <evolve/core/validationlayers.h>
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

		/**
		 * \brief General Instance for Vulkan API handling
		 */
        class EVOLVE_CORE_EXPORT Instance {
        public:
			Instance(const std::string& iApplicationName,
					 const std::shared_ptr<evolve::core::ValidationLayers>& ioValidationLayers);
			~Instance();

			const VkInstance& get() const;

			const std::vector<const char*>& getExtensions() const;
			std::vector<const char*>& accessExtensions();

			const evolve::core::ValidationLayers& getValidationLayers() const;

		//non-copyable
		public:
			Instance() = delete;
			Instance(const Instance&) = delete;
			Instance& operator=(const Instance&) = delete;

		private:
			void retrieveRequiredExtensions();
			
			VkInstance _instance;
			std::string _applicationName;
			std::vector<const char*> _extensions;

			std::shared_ptr<evolve::core::ValidationLayers> _validationLayers;
		};
    }
}

#endif
