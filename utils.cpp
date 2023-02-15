#include "public.h"
#include "utils.h"

char *StrCopy(char *dest, const size_t destlen, const char *src)
{
	if (dest == 0)
	{
		return 0;
	}

	memset(dest, 0, destlen);

	if (src == 0)
	{
		return dest;
	}

	if (strlen(src) > destlen - 1)
	{
		strncpy(dest, src, destlen - 1);
	}
	else
	{
		strcpy(dest, src);
	}

	return dest;
}

char *StrNCopy(char *dest, const size_t destlen, const char *src, size_t n)
{
	if (dest == 0)
	{
		return 0;
	}

	memset(dest, 0, destlen);

	if (src == 0)
	{
		return dest;
	}

	if (strlen(src) > destlen - 1)
	{
		strncpy(dest, src, destlen - 1);
	}
	else
	{
		strncpy(dest, src, n);
	}

	return dest;
}

char *StrCat(char *dest, const size_t destlen, const char *src)
{
	if (dest == 0)
	{
		return 0;
	}

	if (src == 0)
	{
		return dest;
	}

	unsigned int left = destlen - 1 - strlen(dest);

	if (strlen(src) > left)
	{
		strncat(dest, src, left);
		dest[destlen - 1] = 0;
	}
	else
	{
		strcat(dest, src);
	}

	return dest;
}

char *StrNCat(char *dest, const size_t destlen, const char *src, size_t n)
{
	if (dest == 0)
	{
		return 0;
	}

	if (src == 0)
	{
		return dest;
	}

	unsigned int left = destlen - 1 - strlen(dest);

	if (strlen(src) > left)
	{
		strncat(dest, src, left);
		dest[destlen - 1] = 0;
	}
	else
	{
		strncat(dest, src, n);
	}

	return dest;
}

void StrTrim(char *str, const char chr)
{
	StrTrimL(str, chr);
	StrTrimR(str, chr);
}

void StrTrimL(char *str, const char chr)
{
	if (str == 0)
	{
		return;
	}

	if (strlen(str) == 0)
	{
		return;
	}

	char stmp[strlen(str) + 1];
	int itmp = 0;
	memset(stmp, 0, sizeof(stmp));
	strcpy(stmp, str);

	while (stmp[itmp] == chr)
	{
		itmp++;
	}

	memset(str, 0, strlen(str) + 1);
	strcpy(str, stmp + itmp);

	return;
}

void StrTrimR(char *str, const char chr)
{
	if (str == 0)
	{
		return;
	}

	if (strlen(str) == 0)
	{
		return;
	}

	int istrlen = strlen(str);

	while (istrlen > 0)
	{
		if (str[istrlen - 1] != chr)
		{
			break;
		}

		str[istrlen - 1] = 0;
		istrlen--;
	}
}

bool MKdir(const char *filename, bool bisfile)
{
	char spath[301];
	int ilen = strlen(filename);

	for (int i = 1; i < ilen; i++)
	{
		if (filename[i] != '/')
		{
			continue;
		}

		StrNCopy(spath, sizeof(spath), filename, i);
		if (access(spath, F_OK) == 0)
		{
			continue;
		}

		if (mkdir(spath, 0755) != 0)
		{
			return false;
		}
	}

	if (bisfile == false)
	{
		if (access(filename, F_OK) != 0)
		{
			if (mkdir(filename, 0755) != 0)
			{
				return false;
			}
		}
	}

	return true;
}

FILE *FOpen(const char *filename, const char *mode)
{
	if (MKdir(filename) == false)
	{
		return 0;
	}

	return fopen(filename, mode);
}

void time2str(const time_t ltime, char *stime, const char *fmt)
{
	if (stime == 0)
	{
		return;
	}

	strcpy(stime, "");

	struct tm sttm = *localtime(&ltime);

	sttm.tm_year = sttm.tm_year + 1900;
	sttm.tm_mon++;

	if (fmt == 0)
	{
		snprintf(stime, 20, "%04u-%02u-%02u %02u:%02u:%02u", sttm.tm_year, 
				sttm.tm_mon, sttm.tm_mday, sttm.tm_hour, sttm.tm_min, sttm.tm_sec);
		return;
	}

	if (strcmp(fmt, "yyyy-mm-dd hh24:mi:ss") == 0)
	{
		snprintf(stime, 20, "%04u-%02u-%02u %02u:%02u:%02u", sttm.tm_year, 
				sttm.tm_mon, sttm.tm_mday, sttm.tm_hour, sttm.tm_min, sttm.tm_sec);
		return;
	}

	if (strcmp(fmt, "yyyy-mm-dd hh24:mi") == 0)
	{
		snprintf(stime, 20, "%04u-%02u-%02u %02u:%02u", sttm.tm_year, 
				sttm.tm_mon, sttm.tm_mday, sttm.tm_hour, sttm.tm_min);
		return;
	}

	if (strcmp(fmt, "yyyy-mm-dd hh24") == 0)
	{
		snprintf(stime, 20, "%04u-%02u-%02u %02u", sttm.tm_year, 
				sttm.tm_mon, sttm.tm_mday, sttm.tm_hour);
		return;
	}

	if (strcmp(fmt, "yyyy-mm-dd") == 0)
	{
		snprintf(stime, 20, "%04u-%02u-%02u", sttm.tm_year, 
				sttm.tm_mon, sttm.tm_mday);
		return;
	}

	if (strcmp(fmt, "yyyy-mm") == 0)
	{
		snprintf(stime, 20, "%04u-%02u", sttm.tm_year, 
				sttm.tm_mon);
		return;
	}

	if (strcmp(fmt, "yyyymmddhh24miss") == 0)
	{
		snprintf(stime, 20, "%04u%02u%02u %02u%02u%02u", sttm.tm_year, 
				sttm.tm_mon, sttm.tm_mday, sttm.tm_hour, sttm.tm_min, sttm.tm_sec);
		return;
	}

	if (strcmp(fmt, "yyyymmddhh24mi") == 0)
	{
		snprintf(stime, 20, "%04u%02u%02u %02u%02u", sttm.tm_year, 
				sttm.tm_mon, sttm.tm_mday, sttm.tm_hour, sttm.tm_min);
		return;
	}

	if (strcmp(fmt, "yyyymmddhh24") == 0)
	{
		snprintf(stime, 20, "%04u%02u%02u %02u", sttm.tm_year, 
				sttm.tm_mon, sttm.tm_mday, sttm.tm_hour);
		return;
	}

	if (strcmp(fmt, "yyyymmdd") == 0)
	{
		snprintf(stime, 20, "%04u%02u%02u", sttm.tm_year, 
				sttm.tm_mon, sttm.tm_mday);
		return;
	}
}

void str2time(const char *stime)
{}

void LocalTime(char *stime, const char *fmt, const int timeval)
{
	if (stime == 0)
	{
		return;
	}

	time_t timer;

	time(&timer);
	timer = timer + timeval;
	time2str(timer, stime, fmt);
}

int SNPrintf(char *dest, const size_t destlen, size_t n, const char *fmt, ...)
{
	if (dest == 0)
	{
		return -1;
	}

	memset(dest, 0, destlen);
	int ilen = n;
	if (n < destlen)
	{
		ilen = destlen;
	}

	va_list ap;

	va_start(ap, fmt);
	int iret = vsnprintf(dest, ilen, fmt, ap);
	va_end(ap);

	return iret;
}
