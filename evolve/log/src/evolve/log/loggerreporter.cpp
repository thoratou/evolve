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

/**
 * Namespace for all evolve classes
 */
namespace evolve {
    /**
     * Namespace for all utility classes
     */
    namespace log {

        const char* _DefaultCallPos[] = {
            "begin",
            "end"
        };

        const char* _DefaultCallType[] = {
            "constructor",
            "destructor",
            "method",
            "static method",
            "function"
        };

        LoggerReporter::LoggerReporter() {}

        LoggerReporter::~LoggerReporter() {}

        CoutLoggerReporter::CoutLoggerReporter()
            :evolve::log::LoggerReporter() {}

        CoutLoggerReporter::~CoutLoggerReporter() {}

        void CoutLoggerReporter::debug(const std::string& iLog) {
            std::cout << "[DEBUG] " << iLog << std::endl;
        }

        void CoutLoggerReporter::info(const std::string& iLog) {
            std::cout << "[INFO] " << iLog << std::endl;
        }

        void CoutLoggerReporter::warning(const std::string& iLog) {
            std::cout << "[WARNING] " << iLog << std::endl;
        }

        void CoutLoggerReporter::error(const std::string& iLog) {
            std::cout << "[ERROR] " << iLog << std::endl;
        }

        void CoutLoggerReporter::call(CallPosition iPos,
                                      CallType iType,
                                      const char* iClassName,
                                      const std::string& iFunctionName,
                                      const std::string& iAddress){
            switch(iType){
            case CALL_FUNCTION:
                std::cout 	<< "[CALL] ["
                            << _DefaultCallPos[iPos]
                            << "]["
                            << _DefaultCallType[iType]
                            << " = "
                            << iFunctionName
                            << "]"
                            << std::endl;
                break;
            case CALL_STATIC_METHOD:
                std::cout 	<< "[CALL] ["
                            << _DefaultCallPos[iPos]
                            << "]["
                            << _DefaultCallType[iType]
                            << " = "
                            << iClassName
                            << "::"
                            << iFunctionName
                            << "]"
                            << std::endl;
                break;
            default:
                std::cout 	<< "[CALL] ["
                            << _DefaultCallPos[iPos]
                            << "]["
                            << _DefaultCallType[iType]
                            << " = "
                            << iClassName
                            << "::"
                            << iFunctionName
                            << "][address = "
                            << iAddress
                            << "]"
                            << std::endl;
                break;
            }
        }

        FileLoggerReporter::FileLoggerReporter(const char* iFile)
                :LoggerReporter(), _fileStream() {
			_fileStream.open(iFile, std::ofstream::out | std::ofstream::app);
		}

        FileLoggerReporter::~FileLoggerReporter() {
			_fileStream.close();
		}

        void FileLoggerReporter::debug(const std::string& iLog) {
            _fileStream << "[DEBUG] " << iLog << std::endl << std::flush;
        }

        void FileLoggerReporter::info(const std::string& iLog) {
            _fileStream << "[INFO] " << iLog << std::endl << std::flush;
        }

        void FileLoggerReporter::warning(const std::string& iLog) {
            _fileStream << "[WARNING] " << iLog << std::endl << std::flush;
        }

        void FileLoggerReporter::error(const std::string& iLog) {
            _fileStream << "[ERROR] " << iLog << std::endl << std::flush;
        }

        void FileLoggerReporter::call(CallPosition iPos,
                                      CallType iType,
                                      const char* iClassName,
                                      const std::string& iFunctionName,
                                      const std::string& iAddress){
            switch(iType){
            case CALL_FUNCTION:
                _fileStream 	<< "[CALL] ["
                                << _DefaultCallPos[iPos]
                                << "]["
                                << _DefaultCallType[iType]
                                << " = "
                                << iFunctionName
                                << "]"
                                << std::endl << std::flush;
                break;
            case CALL_STATIC_METHOD:
                _fileStream 	<< "[CALL] ["
                                << _DefaultCallPos[iPos]
                                << "]["
                                << _DefaultCallType[iType]
                                << " = "
                                << iClassName
                                << "::"
                                << iFunctionName
                                << "]"
                                << std::endl << std::flush;
                break;
            default:
                _fileStream 	<< "[CALL] ["
                                << _DefaultCallPos[iPos]
                                << "]["
                                << _DefaultCallType[iType]
                                << " = "
                                << iClassName
                                << "::"
                                << iFunctionName
                                << "][address = "
                                << iAddress
                                << "]"
                                << std::endl << std::flush;
                break;
            }
        }
    }
}
