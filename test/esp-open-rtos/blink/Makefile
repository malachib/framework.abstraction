PROGRAM=blink

#EXTRA_COMPONENTS = ../PGFR-8/lib/coap ../PGFR-7/lib/util.embedded extras/mbedtls
# PROGRAM_CFLAGS seems ignored
#PROGRAM_CFLAGS += -lstdc++
#CFLAGS += -lstdc++
#CFLAGS += -std=c++11

PROGRAM_INC_DIR += ../../../src
#PROGRAM_INC_DIR += ../../ext/lwipcpp
#PROGRAM_INC_DIR += ../../ext/FreeRTOScpp
#PROGRAM_INC_DIR += ../../ext/coapcpp

#CFLAGS +=-DESP_OPEN_RTOS=1
CPPFLAGS += -DESP_OPEN_RTOS -DNODUINO -DFEATURE_IOSTREAM_SHIM -DYACOAP_DEBUG=1 -DLWIP_DEBUG -DCOAPCPP
PROGRAM_CPPFLAGS = $(CPPFLAGS) -std=c++11
#main_CPPFLAGS += -std=c++11
#PROGRAM_CPPFLAGS = -DESP_OPEN_RTOS -DFEATURE_IOSTREAM_SHIM -std=c++11 -DNODUINO

include $(ESP_OPEN_RTOS)/common.mk

console:
	miniterm.py $(ESPPORT) 115200
