#ifndef __UTILS_H__
#define __UTILS_H__
#include "public.h"

char *StrCopy(char *dest, const size_t destlen, const char *src);

char *StrNCopy(char *dest, const size_t destlen, const char *src, size_t n);

char *StrCat(char *dest, const size_t destlen, const char *src);

char *StrNCat(char *dest, const size_t destlen, const char *src, size_t n);

void StrTrim(char *str, const char chr);

void StrTrimL(char *str, const char chr);

void StrTrimR(char *str, const char chr);

#endif
