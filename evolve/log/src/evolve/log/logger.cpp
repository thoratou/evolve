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

SINGLETON_IMPL(UniqueSingleton,evolve::log::Logger)

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

        void Logger::debug(const std::string& iLog) {
            if(_reporter==NULL){
                std::cerr << "Can't log : undefined LoggerReporter" << std::endl;
                std::cerr << "To log : [DEBUG] " << iLog << std::endl;
            }
            else{
                _reporter->debug(iLog);
            }
        }

        void Logger::info(const std::string& iLog) {
            if(_reporter==NULL){
                std::cerr << "Can't log : undefined LoggerReporter" << std::endl;
                std::cerr << "To log : [INFO] " << iLog << std::endl;
            }
            else{
                _reporter->info(iLog);
            }
        }

        void Logger::warning(const std::string& iLog) {
            if(_reporter==NULL){
                std::cerr << "Can't log : undefined LoggerReporter" << std::endl;
                std::cerr << "To log : [WARNING] " << iLog << std::endl;
            }
            else{
                _reporter->warning(iLog);
            }
        }

        void Logger::error(const std::string& iLog) {
            if(_reporter==NULL){
                std::cerr << "Can't log : undefined LoggerReporter" << std::endl;
                std::cerr << "To log : [ERROR] " << iLog << std::endl;
            }
            else{
                _reporter->error(iLog);
            }
        }

        void Logger::call(CallPosition iPos,
                          CallType iType,
                          const char* iClassName,
                          const std::string& iFunctionName,
                          const std::string& iAddress){
            if(_reporter==NULL){
                std::cerr << "Can't log : undefined LoggerReporter" << std::endl;
                std::cerr << "To log : [CALL] ..." << std::endl;
            }
            else{
                _reporter->call(iPos,iType,iClassName,iFunctionName,iAddress);
            }
        }

        Logger::Logger()
            :_reporter(NULL) {}

        Logger::~Logger() {
            delete _reporter;
            _reporter = NULL;
        }

        ScopeLog::ScopeLog(evolve::log::CallType iType,
                           const char* iClassName,
                           const std::string& iFunctionName,
                           const std::string& iAddress)
            :_type(iType),_className(iClassName), _functionName(iFunctionName), _address(iAddress){
            EVOLVE_LOG_CALL(evolve::log::CALL_BEGIN,_type,_className,_functionName,_address);
        }

        ScopeLog::~ScopeLog(){
            EVOLVE_LOG_CALL(evolve::log::CALL_END,_type,_className,_functionName,_address);
        }
    }
}
