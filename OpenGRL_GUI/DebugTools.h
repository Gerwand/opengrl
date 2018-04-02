#pragma once

#define GRL_DEBUG
#include <cstdio>

#ifdef GRL_DEBUG
#define DEBUG_ERR(text, ...) fprintf(stderr, "file %s, fun %s, line %d err:\n\t" text "\n", \
									 __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define DEBUG_INFO(text, ...) fprintf(stdout, "fun %s:\n\t" text "\n", \
									  __FUNCTION__, __VA_ARGS__)
#else
#define DEBUG_ERR(text, ...)
#define DEBUG_INFO(text, ...) 
#endif