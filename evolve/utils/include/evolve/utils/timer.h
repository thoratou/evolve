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

#ifndef EVOLVE_TIMER_H
#define EVOLVE_TIMER_H

#ifdef WIN32
#include <windows.h>
#else
#include <ctime>
#endif

namespace evolve {
    namespace utils {
        class Timer {
        public:
            inline explicit Timer() {
#ifdef WIN32
            	QueryPerformanceFrequency(&frequency);
#endif
                reset();
            }
            inline ~Timer() {}
            inline void reset() {
#ifdef WIN32
            	QueryPerformanceCounter(&start);
#else
                start = std::clock();
#endif
            }
#ifdef WIN32
            inline double getSeconds() const {
            	LARGE_INTEGER current;
            	QueryPerformanceCounter(&current);
                return (current.QuadPart - start.QuadPart) / static_cast<double>(frequency.QuadPart);
            }
            inline double getMilliseconds() const {
            	LARGE_INTEGER current;
            	QueryPerformanceCounter(&current);
                return ((current.QuadPart - start.QuadPart)*1000) / static_cast<double>(frequency.QuadPart);
            }
            inline double getMicroseconds() const {
            	LARGE_INTEGER current;
            	QueryPerformanceCounter(&current);
                return ((current.QuadPart - start.QuadPart)*1000000) / static_cast<double>(frequency.QuadPart);
            }
#else
            inline double getSeconds() const {
                return ((double) (std::clock()-start))/((double) CLOCKS_PER_SEC);
            }
            inline double getMilliseconds() const {
            	return (unsigned long) (((double) (std::clock()-start))* ((1000.0) / ((double) CLOCKS_PER_SEC)));
            }
            inline double getMicroseconds() const {
                return (unsigned long) (((double) (std::clock()-start))* ((1000000.0) / ((double) CLOCKS_PER_SEC)));
            }
#endif
        private:
#ifdef WIN32
        	LARGE_INTEGER frequency;
        	LARGE_INTEGER start;
#else
            unsigned long start;
#endif
        };
    }
}

#endif
