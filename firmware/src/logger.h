/*
 * logger.h
 *
 *  Created on: 28 ott 2018
 *      Author: gabbla
 */

#ifndef FIRMWARE_SRC_LOGGER_H_
#define FIRMWARE_SRC_LOGGER_H_

#ifdef __XC32
#include "system/debug/sys_debug.h"
#elif defined __linux__
#include <stdio.h>
#endif

#ifdef __XC32
#define DEBUG(fmt, ...) SYS_DEBUG_PRINT(SYS_ERROR_DEBUG,    "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#define INFO(fmt, ...)  SYS_DEBUG_PRINT(SYS_ERROR_INFO,     "[INFO ] " fmt "\n", ##__VA_ARGS__)
#define WARN(fmt, ...)  SYS_DEBUG_PRINT(SYS_ERROR_WARNING,  "[WARN ] " fmt "\n", ##__VA_ARGS__)
#define ERROR(fmt, ...) SYS_DEBUG_PRINT(SYS_ERROR_ERROR,    "[ERROR] " fmt "\n", ##__VA_ARGS__)
#define FATAL(fmt, ...) SYS_DEBUG_PRINT(SYS_ERROR_FATAL,    "[FATAL] " fmt "\n", ##__VA_ARGS__)
#elif defined __linux__
#define DEBUG(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#define INFO(fmt, ...)  printf("[INFO ] " fmt "\n", ##__VA_ARGS__)
#define WARN(fmt, ...)  printf("[WARN ] " fmt "\n", ##__VA_ARGS__)
#define ERROR(fmt, ...) printf("[ERROR] " fmt "\n", ##__VA_ARGS__)
#define FATAL(fmt, ...) printf("[FATAL] " fmt "\n", ##__VA_ARGS__)
#endif

#endif /* FIRMWARE_SRC_LOGGER_H_ */
