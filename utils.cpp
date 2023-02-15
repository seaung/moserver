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
