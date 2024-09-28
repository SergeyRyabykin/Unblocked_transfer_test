export PROJECT_NAME = elvees_test
export CPPUTEST_HOME ?= /e/CProjects/cpputest
export PROJECT_HOME_DIR = .

CROSS_COMPILE ?= arm-none-eabi-
CC = ${CROSS_COMPILE}gcc
SIZE = ${CROSS_COMPILE}size
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump

DEBUG_DIR = debug
BUILD_DIR = build
OBJ_DIR = ${BUILD_DIR}\objdir
DEP_DIR = ${BUILD_DIR}\depdir

CONFIG_DIR = config

CODE_DIR = code
CODE_SRC = ${CODE_DIR}/src
CODE_INC = ${CODE_DIR}/inc

CODE_SOURCES = main.c $(notdir $(wildcard ${CODE_SRC}/*.c))

#Linker
LINKER_SCRIPT = ${CONFIG_DIR}/STM32F103XE_FLASH.ld

VPATH += ${DEP_DIR}:${OBJ_DIR}:${CODE_SRC}:${CONFIG_DIR}

INCLUDES +=  ${CODE_INC} ${CONFIG_DIR}
export INC_FOR_TEST := ${INCLUDES}

INCLUDES += ${CODE_INC}/device


CPUFLAGS = -mthumb -mcpu=cortex-m3
DEPFLAGS = -MT $@ -MMD -MF ${DEP_DIR}/$(*F).d
DEFINES =
WARNINGS = -Wall -Wextra
OPTIMIZATION = -O2
DEBUG = -g
OTHER = -fstack-usage -ffunction-sections -fdata-sections \
-nostartfiles -nostdlib -nolibc -std=c89

CFLAGS = $(addprefix -I,${INCLUDES}) ${CPUFLAGS} ${DEPFLAGS} ${DEFINES} ${WARNINGS} ${OPTIMIZATION} ${DEBUG}  ${OTHER} 
LDFLAGS = ${CPUFLAGS} -T${LINKER_SCRIPT} -Xlinker -Map=${PROJECT_NAME}.map -static -Wl,--gc-sections -lc

OBJECTS = $(CODE_SOURCES:%.c=%.o)
EXT_C_DEP_OBJS = $(C_SOURCES:%.c=%.o)

all: ${PROJECT_NAME}.hex

${PROJECT_NAME}.hex: ${PROJECT_NAME}.elf
	${OBJCOPY} -Oihex $< $@
	${SIZE} $<
	${OBJDUMP} -D $< > ${PROJECT_NAME}.asm

${PROJECT_NAME}.elf: ${OBJECTS}  ${EXT_C_DEP_OBJS} startup_stm32f103xe.o
	${CC} ${LDFLAGS} $(addprefix ${OBJ_DIR}/, $(notdir $^)) -o $@

${OBJECTS}: %.o: %.c | ${OBJ_DIR} ${DEP_DIR}
	${CC} ${CFLAGS} ${CPPFLAGS} -c $< -o ${OBJ_DIR}/$@

 ${EXT_C_DEP_OBJS}: %.o: %.c | ${OBJ_DIR} ${DEP_DIR}
	${CC} ${CFLAGS} -c $< -o ${OBJ_DIR}/$@

startup_stm32f103xe.o: startup_stm32f103xe.s | ${OBJ_DIR}
	${CC} ${CFLAGS} -c $< -o ${OBJ_DIR}/$@

${OBJ_DIR} ${DEP_DIR} : | ${BUILD_DIR}
	mkdir $@

${BUILD_DIR}:
	mkdir ${BUILD_DIR}

include $(wildcard ${DEP_DIR}/*.d)

.PHONY: clean
clean:
	rm -fr ${BUILD_DIR}
	rm -f ${PROJECT_NAME}.elf
	rm -f ${PROJECT_NAME}.hex
	rm -f ${PROJECT_NAME}.map
	rm -f ${PROJECT_NAME}.asm
	$(MAKE) test-clean

.PHONY: prog
prog: ${PROJECT_NAME}.elf
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program ${PROJECT_NAME}.elf verify exit reset"


.PHONY: debug
debug:
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program ${PROJECT_NAME}.elf verify exit reset"
	python ${DEBUG_DIR}/debug.py ${DEBUG_DIR}/scenario.gdb

.PHONY : print
print:
	@echo ${CODE_SOURCES}
	@echo ${OBJECTS}

.PHONY: test
test:
	+make -f CpputestMakefile
	+make -f tests/unit-tests/print_service/Makefile

.PHONY: test-serv
test-serv:
	+make -f tests/unit-tests/print_service/Makefile

.PHONY: test-clean
test-clean:
	+make -f CpputestMakefile clean
	rm -f *.exe
	rm -f *.txt
	rm -fr test-obj
	rm -fr test-lib
