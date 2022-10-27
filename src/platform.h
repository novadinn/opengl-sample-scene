#ifndef PLATFORM_H
#define PLATFORM_H

#include <iostream>

#define local_persist static
#define internal static
#define global_variable static
#define shared static

#define ARRAY_LENGTH(arr) (int)((sizeof((arr)) / (sizeof((arr)[0]))))
#define BUFFER_OFFSET(offset) ((void*)(offset))

#define MAX_INTEGER 2147483647
#define MIN_INTEGER -2147483648

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef unsigned int uint;
typedef unsigned char uchar;

#endif
