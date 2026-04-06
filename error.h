#ifndef ERROR_H
#define ERROR_H

#define EMPTY_ARR 4
#define MEM_ERROR 3
#define INVALID_PARAMS 5
#define FS_ERROR 6
#define FS_FAILED_TO_WRITE 7
#define FS_FAILED_TO_READ 8

char *get_error_message(int error_code);

#endif
