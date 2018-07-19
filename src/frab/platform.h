#pragma once

#ifdef __MBED__
#define FRAB_MBED
#elif defined(ARDUINO)
#define FRAB_ARDUINO
#elif defined(ESP_OPEN_RTOS)
#define FRAB_ESP8266
#define FRAB_ESP_OPEN_RTOS
#elif defined(ESP_PLATFORM)
#define FRAB_IDF
// TODO: translate IDF various version tags into something preprocessor
//       can actually handle
#if IDV_VER == "2.0.0"
#endif

#if defined(ESP8266)
#define FRAB_ESP8266
#else
#define FRAB_ESP32
#endif

#else
#endif
