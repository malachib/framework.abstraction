namespace framework_abstraction {

// NOTE: Not tested at ALL
// NOTE: shamelessly lifted from arduino libraries!

static os_timer_t delay_timer;
static os_timer_t micros_overflow_timer;
static uint32_t micros_at_last_overflow_tick = 0;
static uint32_t micros_overflow_count = 0;

void micros_overflow_tick(void* arg) {
    (void) arg;
    uint32_t m = sdk_system_get_time();
    if(m < micros_at_last_overflow_tick)
        ++micros_overflow_count;
    micros_at_last_overflow_tick = m;
}

unsigned long ICACHE_RAM_ATTR millis() {
    uint32_t m = sdk_system_get_time();
    uint32_t c = micros_overflow_count + ((m < micros_at_last_overflow_tick) ? 1 : 0);
    return c * 4294967 + m / 1000;
}

unsigned long ICACHE_RAM_ATTR micros() {
    return system_get_time();
}

void esp_timer_init() {
    //initPins();
    //timer1_isr_init();
    os_timer_setfn(&micros_overflow_timer, (os_timer_func_t*) &micros_overflow_tick, 0);
    os_timer_arm(&micros_overflow_timer, 60000, REPEAT);
}

}
