#pragma once

#include <cstdint>
#include <cstdio>

#define _INFO(_format_,...)  do { printf("INFO      "); printf((_format_),##__VA_ARGS__); printf("\n"); } while(0);
#ifdef DBG
#define _DEBUG(_format_,...) do { printf("DEBUG     "); printf((_format_),##__VA_ARGS__); printf("\n"); } while(0);
#else
#define _DEBUG(_format_,...) do {} while(0);
#endif

//TODO: add do {} while(); so that there cannot be indentation problems

#define _WARN(_format_,...)  do { printf("WARNING   "); printf((_format_),##__VA_ARGS__); printf("\n"); } while(0);
#define _ERROR(_format_,...) do { printf("ERROR     "); printf((_format_),##__VA_ARGS__); printf("\n"); } while(0);
#define _DEATH(_format_,...) do { printf("DEATH     "); printf((_format_),##__VA_ARGS__); printf("\n"); throw "Code called _DEATH"; } while(0);
