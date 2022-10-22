#ifndef PLATFORM_H
#define PLATFORM_H

#include <iostream>

#define local_persist static
#define internal static
#define global_variable static
#define shared static

#define ARRAY_LENGTH(arr) (int)((sizeof((arr)) / (sizeof((arr)[0]))))
#define BUFFER_OFFSET(offset) ((void*)(offset))

#define PI 3.14159265358979323846
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b)) 
#define DEG_TO_RAD(x) ((x) * PI / 180.0f)
#define RAD_TO_DEG(x) ((x) * 180.0f / PI)

typedef unsigned int uint;
typedef unsigned char uchar;

#endif
