#include "error.h"

#include <cstring>
#include <unistd.h>

void print_error() {
    char error_message[30] = "An error has occurred\n";
    ssize_t ignored = write(STDERR_FILENO, error_message, strlen(error_message));
    (void)ignored;
}
