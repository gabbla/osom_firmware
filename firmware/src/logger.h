/*
 * logger.h
 *
 *  Created on: 28 ott 2018
 *      Author: gabbla
 */

#ifndef FIRMWARE_SRC_LOGGER_H_
#define FIRMWARE_SRC_LOGGER_H_

#define DEBUG(fmt, ...)	SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, 	fmt, ##__VA_ARGS__)
#define INFO(fmt, ...)	SYS_DEBUG_PRINT(SYS_ERROR_INFO, 	fmt, ##__VA_ARGS__)
#define WARN(fmt, ...)	SYS_DEBUG_PRINT(SYS_ERROR_WARNING, 	fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...)	SYS_DEBUG_PRINT(SYS_ERROR_ERROR, 	fmt, ##__VA_ARGS__)
#define FATAL(fmt, ...)	SYS_DEBUG_PRINT(SYS_ERROR_FATAL, 	fmt, ##__VA_ARGS__)

#endif /* FIRMWARE_SRC_LOGGER_H_ */
