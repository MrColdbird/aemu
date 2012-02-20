all:
	mv atpro.prx ../dist/seplugins

TARGET = atpro
OBJS = main.o libs.o logs.o

CFLAGS = -Os -G0 -Wall -fno-pic

INCDIR = /home/coldbird/Workspace/PROCFW/procfw/include
LIBDIR = /home/coldbird/Workspace/PROCFW/procfw/libs

LIBS = -lpspsystemctrl_kernel

ifeq ($(CONFIG_620), 1)
CFLAGS += -DCONFIG_620=1
endif

ifeq ($(CONFIG_63X), 1)
CFLAGS += -DCONFIG_63X=1
endif

ifeq ($(CONFIG_660), 1)
CFLAGS += -DCONFIG_660=1
endif

ifeq ($(RELEASE), 1)
CFLAGS += -DRELEASE=1
else
CFLAGS += -DDEBUG=1
endif

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

USE_KERNEL_LIBC = 1
USE_KERNEL_LIBS = 1

LDFLAGS = -mno-crt0 -nostartfiles

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
