#include "Log.h"
#include <cstdarg>
#include <cstdio>


void Log::v(const char* tag, const char *format, ...) {
    if(LOGGING_VERBOSE) {
        va_list args;
        va_start(args, format);

        printf("[%s] ", tag);
        vprintf(format, args);
        printf("\n");
    }

}

void Log::v(const char *tag, const block &b) {
    if(LOGGING_VERBOSE) {
        printf("[%s] ", tag);
        for (size_t i = 0; i < sizeof(block); i++) {
//      for(ssize_t i = sizeof(block)-1; i >= 0; i--) {
            printf("%02X", ((uint8_t *) &b)[i]);
        }
        printf("\n");
    }
}
