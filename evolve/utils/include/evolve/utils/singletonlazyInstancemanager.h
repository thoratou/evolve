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
 * \file evolve/utils/SingletonLazyInstance.h
 * \brief evolve/utils singleton lazy instance handler header file
 * \author
 *
 */

#ifndef EVOLVE_SINGLETON_LAZY_INSTANCE_MGR_H
#define EVOLVE_SINGLETON_LAZY_INSTANCE_MGR_H

#include <evolve/utils/export.h>
#include <stack>

/**
 * Namespace for all evolve classes
 */
namespace evolve {
    /**
     * Namespace for all utility classes
     */
    namespace utils {
        class SingletonLazyInstanceInterface;
		
        /**
         * \brief Singleton lazy instance manager.
         *
         * It handles all singleton instance for UniqueSingleton model.
         * The current model only allow lazy instance destruction in the reverse order of the instanciation.
         * It ensure for example the utilsger instance will be destroy at the last end if it is instanciated first.
         */
        class EVOLVE_EXPORT SingletonLazyInstanceManager{
			public:
                /**
                 * \brief Register a lazy instance
                 *
                 * \param[in,out] ioBase The lazy instance interface to register
                 */
				static void registerLazy(SingletonLazyInstanceInterface* ioBase);
			private:
                /**
                 * \brief Default constructor
                 */
				SingletonLazyInstanceManager();

                /**
                 * \brief Destructor
                 */
				~SingletonLazyInstanceManager();

                static SingletonLazyInstanceManager _Instance; ///< global instance for SingletonLazyInstanceManager
                std::stack<SingletonLazyInstanceInterface*> _stack; ///< lazy instance stack
		};
	}
}

#endif

