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

#ifndef EVOLVE_CLOCK_H
#define EVOLVE_CLOCK_H

#ifdef WIN32
#include <windows.h>
#else
#include <ctime>
#endif

#include <sstream>

namespace evolve {
    namespace utils {
        class Clock {
        public:
            inline explicit Clock() {

            }
            inline ~Clock() {}
            inline std::string getFormattedDateAndTime() {
#ifdef WIN32
				SYSTEMTIME time;
				GetSystemTime(&time);
				std::stringstream aSs;

				aSs.width(4);
				aSs.fill('0');
				aSs << time.wYear << "-";

				aSs.width(2);
				aSs.fill('0');
				aSs << time.wMonth << "-";

				aSs.width(2);
				aSs.fill('0');
				aSs << time.wDay << " ";

				aSs.width(2);
				aSs.fill('0');
				aSs << time.wHour << ":";

				aSs.width(2);
				aSs.fill('0');
				aSs << time.wMinute << ":";

				aSs.width(2);
				aSs.fill('0');
				aSs << time.wSecond << ".";

				aSs.width(3);
				aSs.fill('0');
				aSs << time.wMilliseconds;

				return aSs.str();
#else
                start = std::clock();
#endif
            }
        private:
#ifdef WIN32
			SYSTEMTIME time;
#else
            unsigned long start;
#endif
        };
    }
}

#endif
