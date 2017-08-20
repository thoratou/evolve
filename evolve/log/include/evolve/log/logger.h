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
#include <evolve/utils/waitqueue.h>
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
		enum LogLevel {
			LEVEL_DEBUG = 0,
			LEVEL_INFO,
			LEVEL_WARNING,
			LEVEL_ERROR,
			LEVEL_OFF,
		};

        class LoggerReporter;

		struct LogMessage {
			LogLevel _level;
			std::string _message;
			const char* _file;
			unsigned int _line;
			const char* _func;
			std::thread::id _threadId;
		};

        /**
         * \brief Singleton for log message handling
         *
         * Must be used with log macros (EVOLVE_ATTACH_LOGGER_REPORTER,
         * EVOLVE_LOG_DEBUG, EVOLVE_LOG_INFO, EVOLVE_LOG_WARNING, EVOLVE_LOG_ERROR)
         */
        class EVOLVE_EXPORT Logger : public evolve::utils::UniqueSingleton<Logger> {
			SINGLETON_DECL(UniqueSingleton, Logger)

				friend std::thread;
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
             * \param[in] iLogMessage the message to log
             */
			void log(const LogMessage& iLogMessage);

			static std::vector<std::string> _LogLevelStringMap;
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
			evolve::utils::WaitQueue<LogMessage> _logQueue;
			std::thread _logThread;
			bool _closureCondition;

			void loopMessageLogs();
        };
    }
}

# if defined(USE_EVOLVE_LOG_DEBUG) || defined(USE_EVOLVE_LOG_INFO) || defined(USE_EVOLVE_LOG_WARNING) || defined(USE_EVOLVE_LOG_ERROR)
#  define EVOLVE_ATTACH_LOGGER_REPORTER(r) evolve::log::Logger::Instance()->attachReporter((r));
# else
#  define EVOLVE_ATTACH_LOGGER_REPORTER(r)
# endif

# if defined(USE_EVOLVE_LOG_DEBUG) || defined(USE_EVOLVE_LOG_INFO) || defined(USE_EVOLVE_LOG_WARNING) || defined(USE_EVOLVE_LOG_ERROR)
#  define EVOLVE_LOG(level, message) EVOLVE_LOG_(level, message, __FILE__, __LINE__, __FUNCTION__)
#  define EVOLVE_LOG_(level, message, file, line, func) \
	do{ \
		evolve::log::LogMessage aLogMesssage; \
		aLogMesssage._level = level; \
		std::stringstream _ss; \
		_ss << message; \
		aLogMesssage._message = _ss.str(); \
		aLogMesssage._file = file; \
		aLogMesssage._line = line; \
		aLogMesssage._func = func; \
		aLogMesssage._threadId = std::this_thread::get_id(); \
		evolve::log::Logger::Instance()->log(aLogMesssage); \
	} while(0)
# else
#  define EVOLVE_LOG(level, message)
#  define EVOLVE_LOG_(level, message)
# endif

# if defined(USE_EVOLVE_LOG_DEBUG)
#  define EVOLVE_LOG_DEBUG(message)	EVOLVE_LOG(evolve::log::LEVEL_DEBUG, message)
# else
#  define EVOLVE_LOG_DEBUG(message)
# endif

# if defined(USE_EVOLVE_LOG_DEBUG) || defined(USE_EVOLVE_LOG_INFO)
#  define EVOLVE_LOG_INFO(message)	EVOLVE_LOG(evolve::log::LEVEL_INFO, message)
# else
#  define EVOLVE_LOG_INFO(message)
# endif

# if defined(USE_EVOLVE_LOG_DEBUG) || defined(USE_EVOLVE_LOG_INFO) || defined(USE_EVOLVE_LOG_WARNING)
#  define EVOLVE_LOG_WARNING(message)	EVOLVE_LOG(evolve::log::LEVEL_WARNING, message)
# else
#  define EVOLVE_LOG_WARNING(message)
# endif

# if defined(USE_EVOLVE_LOG_DEBUG) || defined(USE_EVOLVE_LOG_INFO) || defined(USE_EVOLVE_LOG_WARNING) || defined(USE_EVOLVE_LOG_ERROR)
#  define EVOLVE_LOG_ERROR(message)	EVOLVE_LOG(evolve::log::LEVEL_ERROR, message)
# else
#  define EVOLVE_LOG_ERROR(message)
# endif

#endif
