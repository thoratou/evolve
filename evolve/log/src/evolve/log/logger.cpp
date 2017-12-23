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
 * \file evolve/log/Logger.cpp
 * \brief evolve/log log handling source file
 * \author
 *
 */


#include <evolve/log/logger.h>
#include <evolve/log/loggerReporter.h>
#include <iostream>

SINGLETON_IMPL(UniqueSingleton, evolve::log::Logger)

std::vector<std::string> evolve::log::Logger::_LogLevelStringMap{ "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL", "OFF" };

/**
 * Namespace for all evolve classes
 */
namespace evolve {
    /**
     * Namespace for all utility classes
     */
    namespace log {

        void Logger::attachReporter(LoggerReporter* iReporter) {
            if(_reporter!=NULL)
                delete _reporter;
            _reporter = iReporter;
        }

        void Logger::log(const LogMessage& aMessage) {
				_logQueue.push(aMessage);
        }

        Logger::Logger()
            :_reporter(NULL),
			 _logQueue(),
			 _logThread(&Logger::loopMessageLogs ,this),
			 _closureCondition(false) {
		}

        Logger::~Logger() {
			_closureCondition = true;
			
			LogMessage aClosureMessage;
			aClosureMessage._level = LEVEL_OFF;
			log(aClosureMessage);

			//wait thread completion
			_logThread.join();

            delete _reporter;
            _reporter = NULL;
        }

		void Logger::loopMessageLogs() {
			while (!_closureCondition) {
				LogMessage aLogMessage;
				_logQueue.pop(aLogMessage);
				if (aLogMessage._level != LEVEL_OFF)
				{
					if (_reporter == NULL) {
						std::cerr << "Can't log : undefined raporter" << std::endl;
						std::cerr << "To log : " << aLogMessage._message << std::endl;
					}
					else {
						this->_reporter->log(aLogMessage);
					}
				}
			}
		}
    }
}
