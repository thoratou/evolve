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

#ifndef EVOLVE_SINGLETON_LAZY_INSTANCE_H
#define EVOLVE_SINGLETON_LAZY_INSTANCE_H

#include <evolve/utils/threadingmodel.h>
#include <evolve/utils/singletonlazyinstancemanager.h>
#include <evolve/utils/export.h>
#include <iostream>
// separate from others Lazy classes
// only used for singleton lazy instanciation

/**
 * Namespace for all evolve classes
 */
namespace evolve {
    /**
     * Namespace for all utility classes
     */
    namespace utils {

        /**
         * \brief Singleton lazy instance interface.
         *
         *  The goal of this class is to have a common interface all instances handled by SingletonLazyInstanceManager
         */
		class EVOLVE_UTILS_EXPORT SingletonLazyInstanceInterface {
		public:
            /**
             * \brief Default constructor
             */
			SingletonLazyInstanceInterface();

            /**
             * \brief Destructor
             */
			virtual ~SingletonLazyInstanceInterface();

            /**
             * \brief Destroy lazy instance method
             *
             * Must only be called by SingletonLazyInstanceManager
             */
			virtual void destroy() = 0;
		};
		
        /**
         * \brief Singleton lazy instance class.
         *
         * \tparam T Instance type
         * \tparam ThreadingModel Single or multiple threading policy
         */
        template <class T,
				  class ThreadingModel = SingleThreadingModel>
		class SingletonLazyInstance : public SingletonLazyInstanceInterface {
		public:
            /**
             * \brief Default constructor
             */
		    SingletonLazyInstance()
				:SingletonLazyInstanceInterface(),_instance(NULL){}
		
            /**
             * \brief Destructor
             */
            virtual ~SingletonLazyInstance(){}
		
            /**
             * \brief Method to retrieve the lazy instance
             *
             * Create the instance on the first call
             *
             * \return Returns the singleton lazy instance reference
             */
            T& get() {
				if(_instance==NULL) {
					typename ThreadingModel::ScopeLockType aGuard(_mutex);
					if(_instance==NULL) {
						_instance = new T();
						SingletonLazyInstanceInterface* aBase = this;
						SingletonLazyInstanceManager::registerLazy(aBase);
		            }
		        }
				return (*_instance);
		    }

            /**
             * \brief Destroy lazy instance method
             *
             * Must only be called by SingletonLazyInstanceManager
             */
            void destroy(){
				delete _instance;
                _instance = NULL;
			}
		
		private:
            typename ThreadingModel::MutexType _mutex; ///< Mutex instance for lazy instantiation
            T* _instance; ///< lazy instance pointer
		};
	}
}


#endif

