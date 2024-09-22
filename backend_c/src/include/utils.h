#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdarg.h>

// Logging Levels
typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

// Initialize Logger
int initialize_logger(const char *log_file, LogLevel level);

// Log Message
void log_message(LogLevel level, const char *format, ...);

// Cleanup Logger
void cleanup_logger();

#endif // UTILS_H
