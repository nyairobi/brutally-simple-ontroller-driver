#include "mu3io_bsod.h"

#include <stdio.h>
#include <errno.h>

extern char *program_invocation_name;
FILE* g_file = NULL;

void do_print(const char* s)
{
    if(g_file == NULL) {
        char buf[256];

#ifdef WIN32
        sprintf(buf, "mu3io_bsod_%s.dll.log", program_invocation_name);
#else
        sprintf(buf, "mu3io_bsod_%s.so.log", program_invocation_name);
#endif // WIN32

        g_file = fopen(buf, "w");
    }

    fprintf(g_file, s);
    fprintf(g_file, "\n");
    fflush(g_file);
}