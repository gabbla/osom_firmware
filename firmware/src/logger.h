/*
 * logger.h
 *
 *  Created on: 28 ott 2018
 *      Author: gabbla
 */

#ifndef FIRMWARE_SRC_LOGGER_H_
#define FIRMWARE_SRC_LOGGER_H_

#define DEBUG(fmt, ...)	SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, 	"[DEBUG] " fmt, ##__VA_ARGS__)
#define INFO(fmt, ...)	SYS_DEBUG_PRINT(SYS_ERROR_INFO, 	"[INFO ] " fmt, ##__VA_ARGS__)
#define WARN(fmt, ...)	SYS_DEBUG_PRINT(SYS_ERROR_WARNING, 	"[WARN ] " fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...)	SYS_DEBUG_PRINT(SYS_ERROR_ERROR, 	"[ERROR] " fmt, ##__VA_ARGS__)
#define FATAL(fmt, ...)	SYS_DEBUG_PRINT(SYS_ERROR_FATAL, 	"[FATAL] " fmt, ##__VA_ARGS__)

#endif /* FIRMWARE_SRC_LOGGER_H_ */
