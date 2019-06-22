#pragma once

#include <cstdint>
#include <cstdio>

#define _INFO(_format_,...)  printf("INFO      "); printf((_format_),##__VA_ARGS__); printf("\n");
#ifdef DBG
#define _DEBUG(_format_,...) printf("DEBUG     "); printf((_format_),##__VA_ARGS__); printf("\n");
#else
#define _DEBUG(_format_,...)
#endif

//TODO: add do {} while(); so that there cannot be indentation problems

#define _WARN(_format_,...)  printf("WARNING   "); printf((_format_),##__VA_ARGS__); printf("\n");
#define _ERROR(_format_,...) printf("ERROR     "); printf((_format_),##__VA_ARGS__); printf("\n");
#define _DEATH(_format_,...) printf("DEATH     "); printf((_format_),##__VA_ARGS__); printf("\n"); throw "Code called _DEATH"; //TODO review
