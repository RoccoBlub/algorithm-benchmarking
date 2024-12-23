#include "logging.h"

// Initialize the global mutex
pthread_mutex_t logMutex = PTHREAD_MUTEX_INITIALIZER;

void logMessage(const char *format, ...) {
    va_list args;
    pthread_mutex_lock(&logMutex); // Lock the mutex to prevent log overlap

    va_start(args, format);
    vprintf(format, args); // Print the log message
    va_end(args);

    pthread_mutex_unlock(&logMutex); // Unlock the mutex after logging
}
