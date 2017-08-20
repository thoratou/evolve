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
* \file evolve/utils/waitqueue.h
* \brief evolve/utils wait queue with multithreading support
* \author
*
*/

#ifndef EVOLVE_WAIT_QUEUE_H
#define EVOLVE_WAIT_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

/**
* Namespace for all evolve classes
*/
namespace evolve {
	/**
	* Namespace for all utility classes
	*/
	namespace utils {

		/**
		* \brief Wait queue with multithreading support
		*
		* Use to push a item in one thread and process it in a seperate process
		*/
		template <class T>
		class WaitQueue {
		public:
			WaitQueue()
				:_queue(), _mutex(), _condition()
			{}

			/**
			* \brief Push the message content to the queue
			*
			* \param[in] iMessage The message to log
			*/
			void push(const T& iMessage) {
				std::lock_guard<std::mutex> scopedLock(_mutex);
				_queue.push(iMessage);
				_condition.notify_one();
			}

			/**
			* \brief Pop the message content from the queue
			* 
			* please not the pop methods waits until there is a message in the queue
			* This means to be used in a separate thread
			* 
			* \param[in] iLog the debug message
			*/
			void pop(T& oMessage) {
				std::unique_lock<std::mutex> scopedLock(_mutex);
				while (_queue.empty())
				{
					//release lock until notify_one()
					_condition.wait(scopedLock);
				}
				oMessage = _queue.front();
				_queue.pop();
			}
		private:
			std::queue<T> _queue;
			std::mutex _mutex;
			std::condition_variable _condition;
		};
	}
}

#endif
