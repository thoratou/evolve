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
 * \file evolve/log/LoggerReporter.cpp
 * \brief evolve/log log message output handling source file
 * \author
 *
 */

#include <evolve/log/loggerreporter.h>
#include <iostream>
#include <sstream>

/**
 * Namespace for all evolve classes
 */
namespace evolve {
    /**
     * Namespace for all utility classes
     */
    namespace log {


        LoggerReporter::LoggerReporter()
			:_clock()
		{}

        LoggerReporter::~LoggerReporter() {}

		void LoggerReporter::formatLogMessage(const LogMessage& iLogMessage, std::string& oLog) {
			std::stringstream aSs;

			aSs << "[" << _clock.getFormattedDateAndTime() << " " << " | ";

			aSs.setf(std::ios::left);
			aSs.fill(' ');
			aSs.width(15);
			aSs << iLogMessage._file << "#" << iLogMessage._line << " > ";
				
			
			aSs.setf(std::ios::right);
			aSs.fill(' ');
			aSs.width(40);
			aSs << iLogMessage._func << " | ";

			aSs.setf(std::ios::right);
			aSs.fill(' ');
			aSs.width(8);
			aSs << Logger::_LogLevelStringMap[iLogMessage._level] << "] ";

			aSs << iLogMessage._message;
			oLog = aSs.str();
		}


        CoutLoggerReporter::CoutLoggerReporter()
            :evolve::log::LoggerReporter() {}

        CoutLoggerReporter::~CoutLoggerReporter() {}

        void CoutLoggerReporter::log(const LogMessage& iLogMessage) {
			std::string aLog;
			formatLogMessage(iLogMessage, aLog);
            std::cout << aLog << std::endl;
        }

        FileLoggerReporter::FileLoggerReporter(const char* iFile)
                :LoggerReporter(), _fileStream() {
			_fileStream.open(iFile, std::ofstream::out | std::ofstream::app);
		}

        FileLoggerReporter::~FileLoggerReporter() {
			_fileStream.close();
		}

        void FileLoggerReporter::log(const LogMessage& iLogMessage) {
			std::string aLog;
			formatLogMessage(iLogMessage, aLog);
            _fileStream << aLog << std::endl << std::flush;
        }
    }
}
