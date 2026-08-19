#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>

#include "util.h"

static int global_log_level = UTIL_LOG_LEVEL_INFO;

static const char* const log_level_names[] = {
    "INFO",
    "WARN",
    "ERROR"
};

#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"

extern void util_set_log_level(int log_level) {
    global_log_level = log_level;
}

// prints formatted message to stderr with color if stderr is a file
static void util_write_message(int log_level, const char* message);

extern void util_log_impl(int log_level, const char* const file, const char* const func, const int line, const char* const format, ...) {
    char out_buffer[2048];
    
    if (global_log_level > log_level) {
        return;
    }

    const int log_level_names_count = sizeof(log_level_names) / sizeof(log_level_names[0]);

    if (UTIL_LOG_LEVEL_MAX != log_level_names_count) {
        fprintf(stderr, "Invalid log level names count: %d != %d\n", UTIL_LOG_LEVEL_MAX, log_level_names_count);
        return;
    }

    if (log_level < 0 || log_level >= log_level_names_count) {
        fprintf(stderr, "Invalid log level specified: %d (%s:%s:%d)\n", log_level, file, func, line);
        return; 
    }
        
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    size_t bytes_written = strftime(out_buffer, sizeof(out_buffer), "%F %T ", tm);
    if (bytes_written == 0) {
        fprintf(stderr, "Failed to format timestamp\n");
        return; 
    }
    
    int prefix_len = snprintf(out_buffer + bytes_written, sizeof(out_buffer) - bytes_written, "[%s] %s:%d:%s(): ", log_level_names[log_level], file, line, func);
    if (prefix_len <= 0) {
        fprintf(stderr, "Failed to format log prefix\n");
        return; 
    }

    bytes_written += prefix_len;

    va_list args;
    va_start(args, format);
    int message_len = vsnprintf(out_buffer + bytes_written, sizeof(out_buffer) - bytes_written, format, args);
    va_end(args);

    if (message_len <= 0) {
        fprintf(stderr, "Failed to format log message\n");
        return; 
    }

    bytes_written += message_len;

    if (bytes_written >= (sizeof(out_buffer) - 2)) {
        fprintf(stderr, "Log message too long\n");
        return; 
    }

    util_write_message(log_level, out_buffer);
}

static void util_write_message(int log_level, const char* message) {
    static int initialized = 0;
    static int is_tty = 0;
    //static FILE* log_file = NULL; //TODO: log to file

    if (!initialized) {
        initialized = 1;
        is_tty = isatty(fileno(stderr));
    }

    if (is_tty) {
        switch(log_level) {
            case UTIL_LOG_LEVEL_INFO:
                fprintf(stderr, COLOR_GREEN "%s\n" COLOR_RESET, message);
                break;
            case UTIL_LOG_LEVEL_WARN:
                fprintf(stderr, COLOR_YELLOW "%s\n" COLOR_RESET, message);
                break;
            case UTIL_LOG_LEVEL_ERROR:
                fprintf(stderr, COLOR_RED "%s\n" COLOR_RESET, message);
                break;
            default:
                fprintf(stderr, "%s\n", message);
        }
    } else {
        fprintf(stderr, "%s\n", message);
    }
    fflush(stderr);
    //TODO: could log to a file too
}
