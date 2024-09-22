#include "../src/utils.h"
#include <stdio.h>
#include <assert.h>

void test_logger() {
    // Initialize Logger
    assert(initialize_logger("logs/test_utils.log", LOG_INFO) == 0);

    // Log messages
    log_message(LOG_INFO, "This is an info message.");
    log_message(LOG_WARNING, "This is a warning message.");
    log_message(LOG_ERROR, "This is an error message.");

    // Cleanup Logger
    cleanup_logger();
    printf("test_logger passed.\n");
}

int main() {
    test_logger();
    return 0;
}
