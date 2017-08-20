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
 * \file evolve/log/Logger.h
 * \brief evolve/log log handling header file
 * \author
 *
 */

#ifndef EVOLVE_LOGGER_H
#define EVOLVE_LOGGER_H

#include <evolve/utils/singleton.h>
#include <evolve/utils/export.h>
#include <string>

/**
 * Namespace for all evolve classes
 */
namespace evolve {
    /**
     * Namespace for all utility classes
     */
    namespace log {

        /**
         * \brief Call position (begin or end of scope)
         */
    	enum CallPosition{
    		CALL_BEGIN,
    		CALL_END
    	};
    	
        /**
         * \brief Call type (constructor, destructor; method, ...)
         */
        enum CallType{
    		CALL_CONSTRUCTOR,
    		CALL_DESTRUCTOR,
    		CALL_METHOD,
    		CALL_STATIC_METHOD,
    		CALL_FUNCTION
    	};
    
        class LoggerReporter;

        /**
         * \brief Singleton for log message handling
         *
         * Must be used with log macros (EVOLVE_ATTACH_LOGGER_REPORTER,
         * EVOLVE_LOG_DEBUG, EVOLVE_LOG_INFO, EVOLVE_LOG_WARNING, EVOLVE_LOG_ERROR)
         */
        class EVOLVE_EXPORT Logger : public evolve::utils::UniqueSingleton<Logger> {
        	SINGLETON_DECL(UniqueSingleton,Logger)
        public:
            /**
             * \brief Attach a specific reporter
             *
             * Please attach the reporter you need at beginning of main function
             *
             * \param[in] iReporter The reporter to attach
             */
            void attachReporter(LoggerReporter* iReporter);

            /**
             * \brief Log a debug message
             *
             * \param[in] iLog the debug message
             */
            void debug(const std::string& iLog);

            /**
             * \brief Log an info message
             *
             * \param[in] iLog the info message
             */
            void info(const std::string& iLog);

            /**
             * \brief Log a warning message
             *
             * \param[in] iLog the warning message
             */
            void warning(const std::string& iLog);

            /**
             * \brief Log an error message
             *
             * \param[in] iLog the error message
             */
            void error(const std::string& iLog);

            /**
             * \brief Log a call message
             *
             * \param[in] iPos The call position (begin or end)
             * \param[in] iType The call type
             * \param[in] iClassName The name of the class
             * \param[in] iFunctionName The function or method name
             * \param[in] iAddress The address of the traced instance
             */
            void call(CallPosition iPos,
                      CallType iType,
                      const char* iClassName,
                      const std::string& iFunctionName,
                      const std::string& iAddress);
        private:
            /**
             * \brief Default constructor
             */
            Logger();

            /**
             * \brief Destructor
             */
            ~Logger();

            LoggerReporter* _reporter; ///< reporter instance
        };

        /**
         * \brief Class to automatically handle log scope destruction
         *
         * With this class, you will need only EVOLVE_DECL_CONSTRUCTOR, ... to create a complete log scope
         */
        class EVOLVE_EXPORT ScopeLog{
        public:
            /**
             * \brief Constructor
             *
             * Log begin scope call
             *
             * \param[in] iType Call type
             * \param[in] iClassName Class name
             * \param[in] iFunctionName Function or method name
             * \param[in] iAddress Instance address to trace
             */
            ScopeLog(evolve::log::CallType iType,
                     const char* iClassName,
                     const std::string& iFunctionName,
                     const std::string& iAddress);

            /**
             * \brief Destructor
             *
             * Log end scope call
             */
            ~ScopeLog();
        private:
            evolve::log::CallType _type; ///< Call type
            const char* _className; ///< Current class name
            std::string _functionName; ///< Function or method name
            std::string _address; ///< Cureent instance address
        };

    }
}

# if defined(USE_EVOLVE_LOG_DEBUG) || defined(USE_EVOLVE_LOG_INFO) || defined(USE_EVOLVE_LOG_WARNING) || defined(USE_EVOLVE_LOG_ERROR)
#  define EVOLVE_ATTACH_LOGGER_REPORTER(r) evolve::log::Logger::Instance()->attachReporter((r));
# else
#  define EVOLVE_ATTACH_LOGGER_REPORTER(r)
# endif

# if defined(USE_EVOLVE_LOG_DEBUG)
#  define EVOLVE_LOG_DEBUG(s)	do{std::stringstream _ss; _ss << s; evolve::log::Logger::Instance()->debug(_ss.str());} while(0)
# else
#  define EVOLVE_LOG_DEBUG(s)
# endif

# if defined(USE_EVOLVE_LOG_DEBUG) || defined(USE_EVOLVE_LOG_INFO)
#  define EVOLVE_LOG_INFO(s)	do {std::stringstream _ss; _ss << s; evolve::log::Logger::Instance()->info(_ss.str());} while(0)
# else
#  define EVOLVE_LOG_INFO(s)
# endif

# if defined(USE_EVOLVE_LOG_DEBUG) || defined(USE_EVOLVE_LOG_INFO) || defined(USE_EVOLVE_LOG_WARNING)
#  define EVOLVE_LOG_WARNING(s)	do{std::stringstream _ss; _ss << s; evolve::log::Logger::Instance()->warning(_ss.str());} while(0)
# else
#  define EVOLVE_LOG_WARNING(s)
# endif

# if defined(USE_EVOLVE_LOG_DEBUG) || defined(USE_EVOLVE_LOG_INFO) || defined(USE_EVOLVE_LOG_WARNING) || defined(USE_EVOLVE_LOG_ERROR)
#  define EVOLVE_LOG_ERROR(s)	do{std::stringstream _ss; _ss << s; evolve::log::Logger::Instance()->error(_ss.str());} while(0)
# else
#  define EVOLVE_LOG_ERROR(s)
# endif

# ifdef USE_EVOLVE_LOG_CALL_TRACER
#  define EVOLVE_LOG_CALL(p,t,c,f,a) evolve::log::Logger::Instance()->call(p,t,c,f,a);
#  define EVOLVE_SCOPE_CALL(t,c,f,a) evolve::log::ScopeLog _scopeLog(t,c,f,a);
#else
#  define EVOLVE_LOG_CALL(p,t,c,f,a)
#  define EVOLVE_SCOPE_CALL(t,c,f,a)
# endif

#endif
