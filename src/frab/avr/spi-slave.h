#pragma once

#include <estd/queue.h>
#include "spi.h"

#ifndef FRAB_SPISLAVE_BUFFER_SIZE
#define FRAB_SPISLAVE_BUFFER_SIZE 32
#endif

extern estd::layer1::queue<uint8_t, FRAB_SPISLAVE_BUFFER_SIZE> spi_slave_queue;

namespace framework_abstraction {

}