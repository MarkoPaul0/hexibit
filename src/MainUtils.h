#pragma once

#ifdef DBG
#define _DEBUG(_format_,...) do { printf("DEBUG   %s:%d: ",__FILE__,__LINE__); printf((_format_),##__VA_ARGS__); printf("\n"); } while(0);
#else
#define _DEBUG(_format_,...) do {} while(0);
#endif

#define _INFO(_format_,...)  do { printf("INFO    %s:%d: ",__FILE__,__LINE__); printf((_format_),##__VA_ARGS__); printf("\n");          } while(0);
#define _WARN(_format_,...)  do { printf("WARNING %s:%d: ",__FILE__,__LINE__); printf((_format_),##__VA_ARGS__); printf("\n");          } while(0);
#define _ERROR(_format_,...) do { printf("ERROR   %s:%d: ",__FILE__,__LINE__); printf((_format_),##__VA_ARGS__); printf("\n");          } while(0);
#define _DEATH(_format_,...) do { printf("DEATH   %s:%d: ",__FILE__,__LINE__); printf((_format_),##__VA_ARGS__); printf("\n"); exit(1); } while(0);

#ifndef _ASSERT
#define _ASSERT(x)                                                      \
    if (!(x)) {                                                         \
        char buf[2048];                                                 \
        snprintf (buf, 2048, "Assertion failed in \"%s\", line %d\n"    \
                 "\tProbable bug in software.\n",                       \
                 __FILE__, __LINE__);                                   \
        _ERROR("%s", buf);                                              \
        throw "throwing error";                                         \
    }                                                                   \
    else   // This 'else' exists to catch the user's following semicolon
#endif
