#include "utils.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

// Global Variables for Logger
static FILE *log_fp = NULL;
static LogLevel current_level = LOG_INFO;

int initialize_logger(const char *log_file, LogLevel level) {
    log_fp = fopen(log_file, "a");
    if (!log_fp) {
        fprintf(stderr, "Failed to open log file: %s\n", log_file);
        return -1;
    }
    current_level = level;
    return 0;
}

void log_message(LogLevel level, const char *format, ...) {
    if (level < current_level) {
        return;
    }

    const char *level_str;
    switch(level) {
        case LOG_INFO: level_str = "INFO"; break;
        case LOG_WARNING: level_str = "WARNING"; break;
        case LOG_ERROR: level_str = "ERROR"; break;
        default: level_str = "UNKNOWN"; break;
    }

    time_t now = time(NULL);
    char time_str[20];
    struct tm *tm_info = localtime(&now);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(log_fp, "[%s] %s: ", time_str, level_str);

    va_list args;
    va_start(args, format);
    vfprintf(log_fp, format, args);
    va_end(args);

    fprintf(log_fp, "\n");
    fflush(log_fp);
}

void cleanup_logger() {
    if (log_fp) {
        fclose(log_fp);
        log_fp = NULL;
    }
}
