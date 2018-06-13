#pragma once

#include "Defines.h"

#define LOGGING_VERBOSE 0

class Log {
public:
    static void v(const char* tag, const char *format, ...);
    static void v(const char* tag, const block& b);
};
