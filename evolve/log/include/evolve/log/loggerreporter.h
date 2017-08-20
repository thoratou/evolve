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
#include <evolve/utils/export.h>
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
        class EVOLVE_EXPORT LoggerReporter {
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
             * \param[in] iLog the debug message
             */
            virtual void debug(const std::string& iLog) = 0;

            /**
             * \brief Log an info message
             *
             * \param[in] iLog the info message
             */
            virtual void info(const std::string& iLog) = 0;

            /**
             * \brief Log a warning message
             *
             * \param[in] iLog the warning message
             */
            virtual void warning(const std::string& iLog) = 0;

            /**
             * \brief Log an error message
             *
             * \param[in] iLog the error message
             */
            virtual void error(const std::string& iLog) = 0;

            /**
             * \brief Log a call message
             *
             * \param[in] iPos The call position (begin or end)
             * \param[in] iType The call type
             * \param[in] iClassName The name of the class
             * \param[in] iFunctionName The function or method name
             * \param[in] iAddress The address of the traced instance
             */
            virtual void call(CallPosition iPos,
                      CallType iType,
                      const char* iClassName,
                      const std::string& iFunctionName,
                      const std::string& iAddress) = 0;
        };

        /**
         * \brief Standard output reporter
         *
         * Writes the log messages into the console
         */
        class EVOLVE_EXPORT CoutLoggerReporter : public LoggerReporter {
        public:
            /**
             * \brief Default constructor
             */
            CoutLoggerReporter();

            /**
             * \brief Destructor
             */
            ~CoutLoggerReporter();

            /**
             * \brief Log a debug message in standard output stream
             *
             * \param[in] iLog the debug message
             */
            void debug(const std::string& iLog);

            /**
             * \brief Log an info message in standard output stream
             *
             * \param[in] iLog the info message
             */
            void info(const std::string& iLog);

            /**
             * \brief Log a warning message in standard output stream
             *
             * \param[in] iLog the warning message
             */
            void warning(const std::string& iLog);

            /**
             * \brief Log an error message in standard output stream
             *
             * \param[in] iLog the error message
             */
            void error(const std::string& iLog);

            /**
             * \brief Log a call message in standard output stream
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
        };
        
        /**
         * \brief File output reporter
         *
         * Writes the log messages into a given file
         */
        class EVOLVE_EXPORT FileLoggerReporter : public LoggerReporter {
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
            ~FileLoggerReporter();

            /**
             * \brief Log a debug message in file output stream
             *
             * \param[in] iLog the debug message
             */
            void debug(const std::string& iLog);

            /**
             * \brief Log an info message in file output stream
             *
             * \param[in] iLog the info message
             */
            void info(const std::string& iLog);

            /**
             * \brief Log a warning message in file output stream
             *
             * \param[in] iLog the warning message
             */
            void warning(const std::string& iLog);

            /**
             * \brief Log an error message in file output stream
             *
             * \param[in] iLog the error message
             */
            void error(const std::string& iLog);

            /**
             * \brief Log a call message in file output stream
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
            std::ofstream _fileStream; ///< output file stream
        };
    }
}

#endif
