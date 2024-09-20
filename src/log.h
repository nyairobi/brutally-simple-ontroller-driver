#define DRIVER_NAME "mu3io-bsod"

#ifdef DEBUG

#include <unistd.h>
#include <stdio.h>

extern char* program_invocation_name;

void do_print(const char*);

#define println(...)                                                         \
        fprintf(stderr, "[%s] [%s] ", DRIVER_NAME, program_invocation_name), \
        fprintf(stderr, __VA_ARGS__),                                        \
        fprintf(stderr, "\n");                                               \
        char buf[500];                                                       \
        sprintf(buf, __VA_ARGS__),                                           \
        do_print(buf)

#else

#define println(...)

#endif // DEBUG