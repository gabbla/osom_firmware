#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-Debug.mk)" "nbproject/Makefile-local-Debug.mk"
include nbproject/Makefile-local-Debug.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Debug
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/speedometer.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/speedometer.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/bleApp.c ../src/main.c ../src/somparser.c ../../../../framework/driver/tmr/src/dynamic/drv_tmr.c ../../../../framework/driver/usart/src/dynamic/drv_usart.c ../../../../framework/driver/usart/src/dynamic/drv_usart_buffer_queue.c ../../../../framework/driver/usart/src/dynamic/drv_usart_read_write.c ../../../../framework/system/console/src/sys_console.c ../../../../framework/system/console/src/sys_console_uart.c ../../../../framework/system/debug/src/sys_debug.c ../../../../framework/system/int/src/sys_int_pic32.c ../../../../framework/system/tmr/src/sys_tmr.c ../src/system_config/Debug/framework/system/clk/src/sys_clk_pic32mx.c ../src/system_config/Debug/framework/system/devcon/src/sys_devcon.c ../src/system_config/Debug/framework/system/devcon/src/sys_devcon_pic32mx.c ../src/system_config/Debug/framework/system/ports/src/sys_ports_static.c ../src/system_config/Debug/system_init.c ../src/system_config/Debug/system_interrupt.c ../src/system_config/Debug/system_exceptions.c ../src/system_config/Debug/system_tasks.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/bleApp.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/somparser.o ${OBJECTDIR}/_ext/185269848/drv_tmr.o ${OBJECTDIR}/_ext/260586732/drv_usart.o ${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o ${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o ${OBJECTDIR}/_ext/30809027/sys_console.o ${OBJECTDIR}/_ext/30809027/sys_console_uart.o ${OBJECTDIR}/_ext/1408546951/sys_debug.o ${OBJECTDIR}/_ext/122796885/sys_int_pic32.o ${OBJECTDIR}/_ext/1264926591/sys_tmr.o ${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o ${OBJECTDIR}/_ext/992133666/sys_devcon.o ${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o ${OBJECTDIR}/_ext/1235143467/sys_ports_static.o ${OBJECTDIR}/_ext/1475841732/system_init.o ${OBJECTDIR}/_ext/1475841732/system_interrupt.o ${OBJECTDIR}/_ext/1475841732/system_exceptions.o ${OBJECTDIR}/_ext/1475841732/system_tasks.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/bleApp.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1360937237/somparser.o.d ${OBJECTDIR}/_ext/185269848/drv_tmr.o.d ${OBJECTDIR}/_ext/260586732/drv_usart.o.d ${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o.d ${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o.d ${OBJECTDIR}/_ext/30809027/sys_console.o.d ${OBJECTDIR}/_ext/30809027/sys_console_uart.o.d ${OBJECTDIR}/_ext/1408546951/sys_debug.o.d ${OBJECTDIR}/_ext/122796885/sys_int_pic32.o.d ${OBJECTDIR}/_ext/1264926591/sys_tmr.o.d ${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o.d ${OBJECTDIR}/_ext/992133666/sys_devcon.o.d ${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o.d ${OBJECTDIR}/_ext/1235143467/sys_ports_static.o.d ${OBJECTDIR}/_ext/1475841732/system_init.o.d ${OBJECTDIR}/_ext/1475841732/system_interrupt.o.d ${OBJECTDIR}/_ext/1475841732/system_exceptions.o.d ${OBJECTDIR}/_ext/1475841732/system_tasks.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/bleApp.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/somparser.o ${OBJECTDIR}/_ext/185269848/drv_tmr.o ${OBJECTDIR}/_ext/260586732/drv_usart.o ${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o ${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o ${OBJECTDIR}/_ext/30809027/sys_console.o ${OBJECTDIR}/_ext/30809027/sys_console_uart.o ${OBJECTDIR}/_ext/1408546951/sys_debug.o ${OBJECTDIR}/_ext/122796885/sys_int_pic32.o ${OBJECTDIR}/_ext/1264926591/sys_tmr.o ${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o ${OBJECTDIR}/_ext/992133666/sys_devcon.o ${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o ${OBJECTDIR}/_ext/1235143467/sys_ports_static.o ${OBJECTDIR}/_ext/1475841732/system_init.o ${OBJECTDIR}/_ext/1475841732/system_interrupt.o ${OBJECTDIR}/_ext/1475841732/system_exceptions.o ${OBJECTDIR}/_ext/1475841732/system_tasks.o

# Source Files
SOURCEFILES=../src/bleApp.c ../src/main.c ../src/somparser.c ../../../../framework/driver/tmr/src/dynamic/drv_tmr.c ../../../../framework/driver/usart/src/dynamic/drv_usart.c ../../../../framework/driver/usart/src/dynamic/drv_usart_buffer_queue.c ../../../../framework/driver/usart/src/dynamic/drv_usart_read_write.c ../../../../framework/system/console/src/sys_console.c ../../../../framework/system/console/src/sys_console_uart.c ../../../../framework/system/debug/src/sys_debug.c ../../../../framework/system/int/src/sys_int_pic32.c ../../../../framework/system/tmr/src/sys_tmr.c ../src/system_config/Debug/framework/system/clk/src/sys_clk_pic32mx.c ../src/system_config/Debug/framework/system/devcon/src/sys_devcon.c ../src/system_config/Debug/framework/system/devcon/src/sys_devcon_pic32mx.c ../src/system_config/Debug/framework/system/ports/src/sys_ports_static.c ../src/system_config/Debug/system_init.c ../src/system_config/Debug/system_interrupt.c ../src/system_config/Debug/system_exceptions.c ../src/system_config/Debug/system_tasks.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/${CND_CONF}/${IMAGE_TYPE}/speedometer.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX170F256B
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/bleApp.o: ../src/bleApp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bleApp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bleApp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/bleApp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1360937237/bleApp.o.d" -o ${OBJECTDIR}/_ext/1360937237/bleApp.o ../src/bleApp.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/somparser.o: ../src/somparser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/somparser.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/somparser.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/somparser.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1360937237/somparser.o.d" -o ${OBJECTDIR}/_ext/1360937237/somparser.o ../src/somparser.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/185269848/drv_tmr.o: ../../../../framework/driver/tmr/src/dynamic/drv_tmr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/185269848" 
	@${RM} ${OBJECTDIR}/_ext/185269848/drv_tmr.o.d 
	@${RM} ${OBJECTDIR}/_ext/185269848/drv_tmr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/185269848/drv_tmr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/185269848/drv_tmr.o.d" -o ${OBJECTDIR}/_ext/185269848/drv_tmr.o ../../../../framework/driver/tmr/src/dynamic/drv_tmr.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/260586732/drv_usart.o: ../../../../framework/driver/usart/src/dynamic/drv_usart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/260586732" 
	@${RM} ${OBJECTDIR}/_ext/260586732/drv_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/260586732/drv_usart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/260586732/drv_usart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/260586732/drv_usart.o.d" -o ${OBJECTDIR}/_ext/260586732/drv_usart.o ../../../../framework/driver/usart/src/dynamic/drv_usart.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o: ../../../../framework/driver/usart/src/dynamic/drv_usart_buffer_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/260586732" 
	@${RM} ${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o.d" -o ${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o ../../../../framework/driver/usart/src/dynamic/drv_usart_buffer_queue.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o: ../../../../framework/driver/usart/src/dynamic/drv_usart_read_write.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/260586732" 
	@${RM} ${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o.d 
	@${RM} ${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o.d" -o ${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o ../../../../framework/driver/usart/src/dynamic/drv_usart_read_write.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/30809027/sys_console.o: ../../../../framework/system/console/src/sys_console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/30809027" 
	@${RM} ${OBJECTDIR}/_ext/30809027/sys_console.o.d 
	@${RM} ${OBJECTDIR}/_ext/30809027/sys_console.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/30809027/sys_console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/30809027/sys_console.o.d" -o ${OBJECTDIR}/_ext/30809027/sys_console.o ../../../../framework/system/console/src/sys_console.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/30809027/sys_console_uart.o: ../../../../framework/system/console/src/sys_console_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/30809027" 
	@${RM} ${OBJECTDIR}/_ext/30809027/sys_console_uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/30809027/sys_console_uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/30809027/sys_console_uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/30809027/sys_console_uart.o.d" -o ${OBJECTDIR}/_ext/30809027/sys_console_uart.o ../../../../framework/system/console/src/sys_console_uart.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1408546951/sys_debug.o: ../../../../framework/system/debug/src/sys_debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1408546951" 
	@${RM} ${OBJECTDIR}/_ext/1408546951/sys_debug.o.d 
	@${RM} ${OBJECTDIR}/_ext/1408546951/sys_debug.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1408546951/sys_debug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1408546951/sys_debug.o.d" -o ${OBJECTDIR}/_ext/1408546951/sys_debug.o ../../../../framework/system/debug/src/sys_debug.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/122796885/sys_int_pic32.o: ../../../../framework/system/int/src/sys_int_pic32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/122796885" 
	@${RM} ${OBJECTDIR}/_ext/122796885/sys_int_pic32.o.d 
	@${RM} ${OBJECTDIR}/_ext/122796885/sys_int_pic32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/122796885/sys_int_pic32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/122796885/sys_int_pic32.o.d" -o ${OBJECTDIR}/_ext/122796885/sys_int_pic32.o ../../../../framework/system/int/src/sys_int_pic32.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1264926591/sys_tmr.o: ../../../../framework/system/tmr/src/sys_tmr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1264926591" 
	@${RM} ${OBJECTDIR}/_ext/1264926591/sys_tmr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1264926591/sys_tmr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1264926591/sys_tmr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1264926591/sys_tmr.o.d" -o ${OBJECTDIR}/_ext/1264926591/sys_tmr.o ../../../../framework/system/tmr/src/sys_tmr.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o: ../src/system_config/Debug/framework/system/clk/src/sys_clk_pic32mx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/753675579" 
	@${RM} ${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o.d" -o ${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o ../src/system_config/Debug/framework/system/clk/src/sys_clk_pic32mx.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/992133666/sys_devcon.o: ../src/system_config/Debug/framework/system/devcon/src/sys_devcon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/992133666" 
	@${RM} ${OBJECTDIR}/_ext/992133666/sys_devcon.o.d 
	@${RM} ${OBJECTDIR}/_ext/992133666/sys_devcon.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/992133666/sys_devcon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/992133666/sys_devcon.o.d" -o ${OBJECTDIR}/_ext/992133666/sys_devcon.o ../src/system_config/Debug/framework/system/devcon/src/sys_devcon.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o: ../src/system_config/Debug/framework/system/devcon/src/sys_devcon_pic32mx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/992133666" 
	@${RM} ${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o.d" -o ${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o ../src/system_config/Debug/framework/system/devcon/src/sys_devcon_pic32mx.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1235143467/sys_ports_static.o: ../src/system_config/Debug/framework/system/ports/src/sys_ports_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1235143467" 
	@${RM} ${OBJECTDIR}/_ext/1235143467/sys_ports_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1235143467/sys_ports_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1235143467/sys_ports_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1235143467/sys_ports_static.o.d" -o ${OBJECTDIR}/_ext/1235143467/sys_ports_static.o ../src/system_config/Debug/framework/system/ports/src/sys_ports_static.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1475841732/system_init.o: ../src/system_config/Debug/system_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1475841732" 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_init.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1475841732/system_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1475841732/system_init.o.d" -o ${OBJECTDIR}/_ext/1475841732/system_init.o ../src/system_config/Debug/system_init.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1475841732/system_interrupt.o: ../src/system_config/Debug/system_interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1475841732" 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_interrupt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_interrupt.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1475841732/system_interrupt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1475841732/system_interrupt.o.d" -o ${OBJECTDIR}/_ext/1475841732/system_interrupt.o ../src/system_config/Debug/system_interrupt.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1475841732/system_exceptions.o: ../src/system_config/Debug/system_exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1475841732" 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_exceptions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1475841732/system_exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1475841732/system_exceptions.o.d" -o ${OBJECTDIR}/_ext/1475841732/system_exceptions.o ../src/system_config/Debug/system_exceptions.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1475841732/system_tasks.o: ../src/system_config/Debug/system_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1475841732" 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1475841732/system_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1475841732/system_tasks.o.d" -o ${OBJECTDIR}/_ext/1475841732/system_tasks.o ../src/system_config/Debug/system_tasks.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/_ext/1360937237/bleApp.o: ../src/bleApp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bleApp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bleApp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/bleApp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1360937237/bleApp.o.d" -o ${OBJECTDIR}/_ext/1360937237/bleApp.o ../src/bleApp.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/somparser.o: ../src/somparser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/somparser.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/somparser.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/somparser.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1360937237/somparser.o.d" -o ${OBJECTDIR}/_ext/1360937237/somparser.o ../src/somparser.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/185269848/drv_tmr.o: ../../../../framework/driver/tmr/src/dynamic/drv_tmr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/185269848" 
	@${RM} ${OBJECTDIR}/_ext/185269848/drv_tmr.o.d 
	@${RM} ${OBJECTDIR}/_ext/185269848/drv_tmr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/185269848/drv_tmr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/185269848/drv_tmr.o.d" -o ${OBJECTDIR}/_ext/185269848/drv_tmr.o ../../../../framework/driver/tmr/src/dynamic/drv_tmr.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/260586732/drv_usart.o: ../../../../framework/driver/usart/src/dynamic/drv_usart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/260586732" 
	@${RM} ${OBJECTDIR}/_ext/260586732/drv_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/260586732/drv_usart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/260586732/drv_usart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/260586732/drv_usart.o.d" -o ${OBJECTDIR}/_ext/260586732/drv_usart.o ../../../../framework/driver/usart/src/dynamic/drv_usart.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o: ../../../../framework/driver/usart/src/dynamic/drv_usart_buffer_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/260586732" 
	@${RM} ${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o.d" -o ${OBJECTDIR}/_ext/260586732/drv_usart_buffer_queue.o ../../../../framework/driver/usart/src/dynamic/drv_usart_buffer_queue.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o: ../../../../framework/driver/usart/src/dynamic/drv_usart_read_write.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/260586732" 
	@${RM} ${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o.d 
	@${RM} ${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o.d" -o ${OBJECTDIR}/_ext/260586732/drv_usart_read_write.o ../../../../framework/driver/usart/src/dynamic/drv_usart_read_write.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/30809027/sys_console.o: ../../../../framework/system/console/src/sys_console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/30809027" 
	@${RM} ${OBJECTDIR}/_ext/30809027/sys_console.o.d 
	@${RM} ${OBJECTDIR}/_ext/30809027/sys_console.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/30809027/sys_console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/30809027/sys_console.o.d" -o ${OBJECTDIR}/_ext/30809027/sys_console.o ../../../../framework/system/console/src/sys_console.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/30809027/sys_console_uart.o: ../../../../framework/system/console/src/sys_console_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/30809027" 
	@${RM} ${OBJECTDIR}/_ext/30809027/sys_console_uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/30809027/sys_console_uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/30809027/sys_console_uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/30809027/sys_console_uart.o.d" -o ${OBJECTDIR}/_ext/30809027/sys_console_uart.o ../../../../framework/system/console/src/sys_console_uart.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1408546951/sys_debug.o: ../../../../framework/system/debug/src/sys_debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1408546951" 
	@${RM} ${OBJECTDIR}/_ext/1408546951/sys_debug.o.d 
	@${RM} ${OBJECTDIR}/_ext/1408546951/sys_debug.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1408546951/sys_debug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1408546951/sys_debug.o.d" -o ${OBJECTDIR}/_ext/1408546951/sys_debug.o ../../../../framework/system/debug/src/sys_debug.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/122796885/sys_int_pic32.o: ../../../../framework/system/int/src/sys_int_pic32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/122796885" 
	@${RM} ${OBJECTDIR}/_ext/122796885/sys_int_pic32.o.d 
	@${RM} ${OBJECTDIR}/_ext/122796885/sys_int_pic32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/122796885/sys_int_pic32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/122796885/sys_int_pic32.o.d" -o ${OBJECTDIR}/_ext/122796885/sys_int_pic32.o ../../../../framework/system/int/src/sys_int_pic32.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1264926591/sys_tmr.o: ../../../../framework/system/tmr/src/sys_tmr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1264926591" 
	@${RM} ${OBJECTDIR}/_ext/1264926591/sys_tmr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1264926591/sys_tmr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1264926591/sys_tmr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1264926591/sys_tmr.o.d" -o ${OBJECTDIR}/_ext/1264926591/sys_tmr.o ../../../../framework/system/tmr/src/sys_tmr.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o: ../src/system_config/Debug/framework/system/clk/src/sys_clk_pic32mx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/753675579" 
	@${RM} ${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o.d" -o ${OBJECTDIR}/_ext/753675579/sys_clk_pic32mx.o ../src/system_config/Debug/framework/system/clk/src/sys_clk_pic32mx.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/992133666/sys_devcon.o: ../src/system_config/Debug/framework/system/devcon/src/sys_devcon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/992133666" 
	@${RM} ${OBJECTDIR}/_ext/992133666/sys_devcon.o.d 
	@${RM} ${OBJECTDIR}/_ext/992133666/sys_devcon.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/992133666/sys_devcon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/992133666/sys_devcon.o.d" -o ${OBJECTDIR}/_ext/992133666/sys_devcon.o ../src/system_config/Debug/framework/system/devcon/src/sys_devcon.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o: ../src/system_config/Debug/framework/system/devcon/src/sys_devcon_pic32mx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/992133666" 
	@${RM} ${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o.d" -o ${OBJECTDIR}/_ext/992133666/sys_devcon_pic32mx.o ../src/system_config/Debug/framework/system/devcon/src/sys_devcon_pic32mx.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1235143467/sys_ports_static.o: ../src/system_config/Debug/framework/system/ports/src/sys_ports_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1235143467" 
	@${RM} ${OBJECTDIR}/_ext/1235143467/sys_ports_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1235143467/sys_ports_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1235143467/sys_ports_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1235143467/sys_ports_static.o.d" -o ${OBJECTDIR}/_ext/1235143467/sys_ports_static.o ../src/system_config/Debug/framework/system/ports/src/sys_ports_static.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1475841732/system_init.o: ../src/system_config/Debug/system_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1475841732" 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_init.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1475841732/system_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1475841732/system_init.o.d" -o ${OBJECTDIR}/_ext/1475841732/system_init.o ../src/system_config/Debug/system_init.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1475841732/system_interrupt.o: ../src/system_config/Debug/system_interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1475841732" 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_interrupt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_interrupt.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1475841732/system_interrupt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1475841732/system_interrupt.o.d" -o ${OBJECTDIR}/_ext/1475841732/system_interrupt.o ../src/system_config/Debug/system_interrupt.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1475841732/system_exceptions.o: ../src/system_config/Debug/system_exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1475841732" 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_exceptions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1475841732/system_exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1475841732/system_exceptions.o.d" -o ${OBJECTDIR}/_ext/1475841732/system_exceptions.o ../src/system_config/Debug/system_exceptions.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1475841732/system_tasks.o: ../src/system_config/Debug/system_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1475841732" 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1475841732/system_tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1475841732/system_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/Debug" -I"../src/Debug" -I"../../../../framework" -I"../src/system_config/Debug/framework" -MMD -MF "${OBJECTDIR}/_ext/1475841732/system_tasks.o.d" -o ${OBJECTDIR}/_ext/1475841732/system_tasks.o ../src/system_config/Debug/system_tasks.c    -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/speedometer.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../../bin/framework/peripheral/PIC32MX170F256B_peripherals.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/speedometer.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ..\..\..\..\bin\framework\peripheral\PIC32MX170F256B_peripherals.a      -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=2048,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/speedometer.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../../bin/framework/peripheral/PIC32MX170F256B_peripherals.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/speedometer.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ..\..\..\..\bin\framework\peripheral\PIC32MX170F256B_peripherals.a      -DXPRJ_Debug=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=2048,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/speedometer.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} -r dist/Debug

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
