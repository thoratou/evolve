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
 * \file evolve/utils/Policies.cpp
 * \brief evolve/utils policy source file
 * \author
 *
 */

#include <evolve/utils/threadutils.h>
#include <map>

/**
 * Namespace for all evolve classes
 */
namespace evolve {
    /**
     * Namespace for all utility classes
     */
    namespace utils {

		unsigned int GetThreadId(const std::thread::id& iId) {
			static std::map<std::thread::id, unsigned int> aThreadIdMap;
			static unsigned int aThreadIdCount = 0;

			std::map<std::thread::id, unsigned int>::const_iterator aIt = aThreadIdMap.find(iId);
			if (aIt != aThreadIdMap.end()) {
				return aIt->second;
			}
			else {
				aThreadIdMap[iId] = aThreadIdCount;
				return aThreadIdCount++;
			}
		}

	}
}
