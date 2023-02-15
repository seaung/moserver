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

FILE *FOpen(const char *filename, const char *mode);

bool MKdir(const char *filename, bool bisfile = true);

void time2str(const time_t ltime, char *stime, const char *fmt=0);

void str2time(const char *stime);

void LocalTime(char *stime, const char *fmt=0, const int timeval=0);

int SNPrintf(char *dest, const size_t destlen, size_t n, const char *fmt, ...);

#endif
