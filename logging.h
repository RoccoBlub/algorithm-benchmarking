#ifndef LOGGING_H
#define LOGGING_H

#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>

// Declare the global mutex for logging
extern pthread_mutex_t logMutex;

// Declare the logMessage function
void logMessage(const char *format, ...);

#endif // LOGGING_H
