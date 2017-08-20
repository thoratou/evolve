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

#ifndef EVOLVE_THREAD_H
#define EVOLVE_THREAD_H

#include <thread>
#include <mutex>

namespace evolve {
    namespace utils {

        /*!  \class NullMutexType
          *  \brief Empty type, do nothing. For single threading
          *  \author Ratouit Thomas
          *  \version 1.0
          *  \date 2008
          */
        struct NullMutexType {}
        ;

        /*!  \class NullScopeLock
          *  \brief mutex scope lock For single threading. Do nothing.
          *  \author Ratouit Thomas
          *  \version 1.0
          *  \date 2008
          */
        struct NullScopeLock {
            NullScopeLock(NullMutexType mutex) {}
            ~NullScopeLock() {}
            typedef NullMutexType MutexType;
        };

        /*!  \class SingleThreadingModel
          *  \brief Model for single thread object.
          *  \author Ratouit Thomas
          *  \version 1.0
          *  \date 2008
          */
        struct SingleThreadingModel {
            typedef struct NullScopeLock ScopeLockType;
            typedef NullScopeLock::MutexType MutexType;
        };

        /*!  \class MultipleThreadingModel
          *  \brief Model for single thread object.
          *  \author Ratouit Thomas
          *  \version 1.0
          *  \date 2008
          */
        struct MultipleThreadingModel {
            typedef std::lock_guard<std::mutex> ScopeLockType;
            typedef std::mutex MutexType;
        };
    }
}

#endif
