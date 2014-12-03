#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

class Logger {
public:
    static void log(char *format, ...)
    {
        va_list args;

        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
};

#endif // LOG_H
