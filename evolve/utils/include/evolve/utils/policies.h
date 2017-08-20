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
 * \file evolve/utils/Policies.h
 * \brief evolve/utils policy header file
 * \author
 *
 */

#ifndef EVOLVE_POLICIES_H
#define EVOLVE_POLICIES_H

#include <evolve/utils/export.h>
#include <evolve/utils/singletonLazyInstance.h>
#include <exception>
#include <cstring>

/**
 * Namespace for all evolve classes
 */
namespace evolve {
	/**
	 * Namespace for all utility classes
	 */
	namespace utils {

		/**
		 * \brief policy for the creation and the destruction that use new and delete
		 *
		 * \tparam T Instance type
		 */
        template <class T>
        class CreationWithNew {
        public:
			/**
			 * \brief Create a new instance and returns its pointer
			 *
			 * \return Returns an instance pointer
			 */
			static T* Create(){
				return new T();
			}

			/**
			 * \brief Destroy an instance
			 *
			 * \param[in] iInst instance to destroy
			 */
			static void Delete(T* iInst){
				delete iInst;
			}
        };

		/**
		 * \brief policy for the creation and the destruction that use a static instance
		 *
		 * Be careful. The instance can't manually be destroy.
		 * If the same policy is reused, the state of the instance is the same.
		 *
		 * \tparam T Instance type
		 */
        template <class T>
        class CreationWithStatic {
        public:
			/**
			 * \brief Returns the static instance pointer
			 *
			 * \return Returns an instance pointer
			 */
			static T* Create(){
				return &T::_policy_instance;
			}

			/**
			 * \brief To nothing
			 *
			 * This method is here to be compatible with all creation policies
			 * In pratice, the method does nothing
			 *
			 * \param[in] iInst instance to destroy
			 */
			static void Delete(T* iInst){}
		};

		/**
		 * \brief policy for the creation and the destruction that use a static instance
		 *
		 * Be careful. The instance can't manually be destroy.
		 * If the same policy is reused, the state of the instance is the same.
		 *
		 * \tparam T Instance type
		 */
        template <class T>
        class CreationWithLazy {
        public:
			/**
			 * \brief Returns the lazy instance pointer
			 *
			 * \return Returns an instance pointer
			 */
			static T* Create(){
				SingletonLazyInstance<T>* instance = new evolve::utils::SingletonLazyInstance<T>();
				return &instance->get();
			}

			/**
			 * \brief To nothing
			 *
			 * This method is here to be compatible with all creation policies
			 * In pratice, the method does nothing
			 *
			 * \param[in] iInst instance to destroy
			 */
			static void Delete(T* iInst){}
		};
        
		/**
		 * \brief policy interface for the instance lifetime
		 */
		struct LifeTimePolicyMother {
            /**
             * \brief Default constructor
             */
            LifeTimePolicyMother() {}

            /**
             * \brief Destructor
             */
            virtual ~LifeTimePolicyMother() {}

            /**
             * \brief Register the fact that an instanciation has been performed
             */
            virtual inline void instanciated() = 0;

            /**
             * \brief Register the instance usage
             */
            virtual inline void instanceUsed() = 0;

            /**
             * \brief Indicate if the deletion of the instance is allowed
             *
             * \return Is deletion allowed
             */
            virtual inline bool isAuthorisedDeletion() = 0;

            /**
             * \brief Register the fact that an instance has been deleted
             */
            virtual inline void deleted() = 0;
        };

		/**
		 * \brief policy for undestroyable instance
		 */
		struct NoDestructionLifeTime : public LifeTimePolicyMother {
            /**
             * \brief Default constructor
             */
            NoDestructionLifeTime()
                    :LifeTimePolicyMother() {}

            /**
             * \brief Destructor
             */
            ~NoDestructionLifeTime() {}

            /**
             * \brief To nothing
             */
            inline void instanciated() {}

            /**
             * \brief To nothing
             */
            inline void instanceUsed() {}

            /**
             * \brief Always refuse instance deletion
             */
            inline bool isAuthorisedDeletion() {
                return false;
            }

            /**
             * \brief To nothing
             */
            inline void deleted() {}
        };

		/**
		 * \brief policy for destroyable instance
		 */
		struct DestroyableInstanceLifeTime : public LifeTimePolicyMother {
            /**
             * \brief Default constructor
             */
            DestroyableInstanceLifeTime()
					:LifeTimePolicyMother(),_instanced(true) {}

            /**
             * \brief Destructor
             */
            ~DestroyableInstanceLifeTime() {}

            /**
             * \brief Register the fact that an instanciation has been performed
             */
			inline void instanciated() {
				_instanced=true;
            }

            /**
             * \brief Register the instance usage
             *
             * Do nothing in this case
             */
			inline void instanceUsed() {}

            /**
             * \brief Indicate if the deletion of the instance is allowed
             *
             * \return Is deletion allowed
             */
			inline bool isAuthorisedDeletion() {
				return _instanced;
            }

            /**
             * \brief Register the fact that an instance has been deleted
             */
			inline void deleted() {
				_instanced=false;
            }
        private:
            bool _instanced; ///< Internal flag
        };

		/**
		 * \brief policy for destroyable instance based on reference count
		 */
		struct ReferenceCountLifeTime : public LifeTimePolicyMother {
            /**
             * \brief Default constructor
             */
            ReferenceCountLifeTime()
					:LifeTimePolicyMother(),_refcount(-1) {}

            /**
             * \brief Destructor
             */
            ~ReferenceCountLifeTime() {}

            /**
             * \brief Register the fact that an instanciation has been performed
             *
             * Set the counter to 0
             */
            inline void instanciated() {
				_refcount=0;
            }

            /**
             * \brief Register the instance usage
             *
             * Bump the counter
             */
			inline void instanceUsed() {
				++_refcount;
            }

            /**
             * \brief Indicate if the deletion of the instance is allowed
             *
             * the deletion is allowed if count is -1
             *
             * \return Is deletion allowed
             */
			inline bool isAuthorisedDeletion() {
				return (_refcount<0);
            }

            /**
             * \brief Register the fact that an instance has been deleted
             *
             * Decrease the counter
             */
			inline void deleted() {
				--_refcount;
            }
        private:
            int _refcount; ///< Internal counter
        };

		/**
		 * \brief policy for destroyable instance based on reference count
		 *
		 * Throws exception if too many destructions
		 */
		struct SafeReferenceCountLifeTime : public LifeTimePolicyMother {
            /**
             * \brief Default constructor
             */
            SafeReferenceCountLifeTime()
					:LifeTimePolicyMother(),_refcount(-1) {}

            /**
             * \brief Destructor
             */
            ~SafeReferenceCountLifeTime() {}

            /**
             * \brief Register the fact that an instanciation has been performed
             *
             * Set the counter to 0
             */
			inline void instanciated() {
				if (_refcount>=0)
                    throw std::exception(); /*too many instanciations*/
                else
					_refcount=0;
            }

            /**
             * \brief Register the instance usage
             *
             * Bump the counter
             */
			inline void instanceUsed() {
				++_refcount;
            }

            /**
             * \brief Indicate if the deletion of the instance is allowed
             *
             * the deletion is allowed if count is -1
             *
             * \return Is deletion allowed
             */
			inline bool isAuthorisedDeletion() {
                return (_refcount<0);
            }

            /**
             * \brief Register the fact that an instance has been deleted
             *
             * Decrease the counter
             */
            inline void deleted() {
				if (_refcount>=0)
					--_refcount;
                else
                    throw std::exception();  /*too many deletions*/
            }
        private:
            int _refcount; ///< Internal counter
        };
    }
}

#endif
