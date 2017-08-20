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
 * \file evolve/utils/Export.h
 * \brief Export header file for evolve/utils
 * \author Thomas Ratouit
 *
 * Pre-processor definitions for evolve/utils component
 */

#ifndef EVOLVE_EXPORT_H
#define EVOLVE_EXPORT_H

#if defined (_MSC_VER)
# pragma warning(disable: 4251) //disable dll export warning
# pragma warning(disable: 4275)//disable dll export warning
# pragma warning(disable: 4996)//disable depreciated function warning
#endif

#ifdef WIN32
#	ifdef EVOLVE_BUILD_SHARED_LIBRARY
#  		define EVOLVE_EXPORT __declspec(dllexport)
#	else
#		define EVOLVE_EXPORT __declspec(dllimport)
#	endif
#else
#	define EVOLVE_UTILS_EXPORT
#endif

#endif
