#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <estd/chrono.h>
#include <estd/thread.h>

#include <frab/frab.h>

#include <stdio.h>

void toggle();

using namespace estd;

#ifndef CONFIG_TOGGLE_FREQUENCY
#define CONFIG_TOGGLE_FREQUENCY 1000
#endif

extern "C" void test_task(void*)
{
    int debug = 0;

    constexpr chrono::milliseconds interval(CONFIG_TOGGLE_FREQUENCY);

    for(;;)
    {
        printf("Debug: %d\n", debug++);

        toggle();
        
        this_thread::sleep_for(interval);
    }
}