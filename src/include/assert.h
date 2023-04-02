#ifndef _ASSERT_H_
#define _ASSERT_H_

void assertion_failure(char *exp, char *file, char *base, int line);

#define assert(exp)     \
    if (exp)            \
        ;               \
    else                \
        assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)


void panic(const char *fmt, ...);
#endif // _ASSERT_H_