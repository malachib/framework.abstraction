#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <estd/chrono.h>
#include <estd/thread.h>

#include <frab/frab.h>

#include <stdio.h>

void toggle();

using namespace estd;

extern "C" void test_task(void*)
{
    int debug = 0;

    constexpr chrono::milliseconds interval(1000);

    for(;;)
    {
        printf("Debug: %d\n", debug++);

        toggle();
        
        this_thread::sleep_for(interval);
    }
}