#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <stdarg.h>

/* this memcpy won't result in a safty problem */
void *HtMemcpy(void *s1, const void *s2, size_t n)
{
    return memcpy(s1, s2, n);
}

char *HtStrcpy(char *s1, const char *s2)
{
    return strncpy(s1, s2, strlen(s2) + 1);
}

char *HtStrcat(char *s1, const char *s2)
{
    return strncat(s1, s2, strlen(s2) + 1);
}

int HtSprintf(char *s, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    vsprintf(s, format, args);
    va_end(args);

    return 0;
}
