#include "error.h"

char* get_error_message(int error_code) {
    switch (error_code) {
        case MEM_ERROR:
            return "Failed to allocate memory";
        case INVALID_PARAMS:
            return "Invalid parameters";
        case FS_ERROR:
            return "Failed to open file";
        case FS_FAILED_TO_WRITE:
            return "Failed to write on file";
        case FS_FAILED_TO_READ:
            return "Failed to read from file";
        default:
            return "Unhandled";
    }
}
