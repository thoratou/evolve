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
 * \file evolve/log/LoggerReporter.h
 * \brief evolve/log log message output handling header file
 * \author
 *
 */

#ifndef EVOLVE_LOGGER_REPORTER_H
#define EVOLVE_LOGGER_REPORTER_H

#include <evolve/log/logger.h>
#include <evolve/utils/clock.h>
#include <evolve/log/export.h>
#include <string>
#include <fstream>

/**
 * Namespace for all evolve classes
 */
namespace evolve {
    /**
     * Namespace for all utility classes
     */
    namespace log {

        /**
         * \brief Interface for all log reporters
         */
		class EVOLVE_LOG_EXPORT LoggerReporter {
		public:
			/**
			 * \brief Default constructor
			 */
			LoggerReporter();

			/**
			 * \brief Destructor
			 */
			virtual ~LoggerReporter() = 0;

			/**
			* \brief Log a debug message
			*
			* \param[in] iLogMessage the message to log
			*/
			virtual void log(const LogMessage& iLogMessage) = 0;
		protected:
			/**
			* \brief Helper method to fo)=
			*
			* \param[in] iLogMessage the message to log
			* \param[out] oLog the formatted log
			*/
			virtual void formatLogMessage(const LogMessage& iLogMessage, std::string& oLog);

			evolve::utils::Clock _clock;
		};

        /**
         * \brief Standard output reporter
         *
         * Writes the log messages into the console
         */
        class EVOLVE_LOG_EXPORT CoutLoggerReporter : public LoggerReporter {
        public:
            /**
             * \brief Default constructor
             */
            CoutLoggerReporter();

            /**
             * \brief Destructor
             */
			virtual ~CoutLoggerReporter();

			/**
			* \brief Log a debug message
			*
			* \param[in] iLogMessage the message to log
			*/
			virtual void log(const LogMessage& iLogMessage);
        };
        
        /**
         * \brief File output reporter
         *
         * Writes the log messages into a given file
         */
        class EVOLVE_LOG_EXPORT FileLoggerReporter : public LoggerReporter {
        public:
            /**
             * \brief Constructor with output file path
             *
             * \param[in] iFile complete file path
             */
            FileLoggerReporter(const char* iFile);

            /**
             * \brief Destructor
             */
			virtual ~FileLoggerReporter();

			/**
			* \brief Log a debug message
			*
			* \param[in] iLogMessage the message to log
			*/
			virtual void log(const LogMessage& iLogMessage);

        private:
            std::ofstream _fileStream; ///< output file stream
        };
    }
}

#endif
