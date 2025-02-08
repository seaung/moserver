#include "public.h"
#include "utils.h"

/**
 * @brief 安全的字符串复制函数
 * @details 将src指向的字符串复制到dest指向的内存空间，确保不会发生缓冲区溢出
 * @param dest 目标字符串缓冲区
 * @param destlen 目标缓冲区的大小
 * @param src 源字符串
 * @return 成功返回dest指针，失败返回0
 * @note 如果src的长度大于destlen-1，则只复制destlen-1个字符
 */
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

/**
 * @brief 安全的字符串复制函数（指定长度）
 * @details 将src指向的字符串的前n个字符复制到dest指向的内存空间
 * @param dest 目标字符串缓冲区
 * @param destlen 目标缓冲区的大小
 * @param src 源字符串
 * @param n 要复制的字符数
 * @return 成功返回dest指针，失败返回0
 * @note 如果src的长度大于destlen-1，则只复制destlen-1个字符
 */
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

/**
 * @brief 安全的字符串连接函数
 * @details 将src指向的字符串连接到dest字符串的末尾
 * @param dest 目标字符串
 * @param destlen 目标字符串缓冲区的总大小
 * @param src 要连接的源字符串
 * @return 成功返回dest指针，失败返回0
 * @note 确保目标缓冲区不会溢出
 */
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

/**
 * @brief 安全的字符串连接函数（指定长度）
 * @details 将src指向的字符串的前n个字符连接到dest字符串的末尾
 * @param dest 目标字符串
 * @param destlen 目标字符串缓冲区的总大小
 * @param src 要连接的源字符串
 * @param n 要连接的字符数
 * @return 成功返回dest指针，失败返回0
 * @note 确保目标缓冲区不会溢出
 */
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

/**
 * @brief 去除字符串两端指定的字符
 * @details 去除字符串开头和结尾处的指定字符
 * @param str 要处理的字符串
 * @param chr 要去除的字符
 */
void StrTrim(char *str, const char chr)
{
	StrTrimL(str, chr);
	StrTrimR(str, chr);
}

/**
 * @brief 去除字符串左端指定的字符
 * @details 去除字符串开头处的指定字符
 * @param str 要处理的字符串
 * @param chr 要去除的字符
 */
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

/**
 * @brief 去除字符串右端指定的字符
 * @details 去除字符串结尾处的指定字符
 * @param str 要处理的字符串
 * @param chr 要去除的字符
 */
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

/**
 * @brief 递归创建目录
 * @details 根据文件路径创建必要的目录结构
 * @param filename 文件或目录的完整路径
 * @param bisfile 是否为文件，true表示文件，false表示目录
 * @return 成功返回true，失败返回false
 */
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

/**
 * @brief 安全的文件打开函数
 * @details 打开文件前会自动创建必要的目录结构
 * @param filename 要打开的文件的完整路径
 * @param mode 文件打开模式
 * @return 成功返回文件指针，失败返回0
 */
FILE *FOpen(const char *filename, const char *mode)
{
	if (MKdir(filename) == false)
	{
		return 0;
	}

	return fopen(filename, mode);
}

/**
 * @brief 时间转换为字符串
 * @details 将时间值转换为指定格式的字符串
 * @param ltime 时间值
 * @param stime 输出的字符串缓冲区
 * @param fmt 输出格式，支持多种日期时间格式
 */
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

/**
 * @brief 字符串转换为时间（待实现）
 * @param stime 时间字符串
 */
time_t str2time(const char *stime)
{
	if (stime == 0)
	{
		return 0;
	}

	struct tm sttm;
	memset(&sttm, 0, sizeof(sttm));

	// 尝试解析 yyyy-mm-dd hh24:mi:ss 格式
	if (sscanf(stime, "%d-%d-%d %d:%d:%d", &sttm.tm_year, &sttm.tm_mon, 
			&sttm.tm_mday, &sttm.tm_hour, &sttm.tm_min, &sttm.tm_sec) == 6)
	{
		sttm.tm_year -= 1900;
		sttm.tm_mon -= 1;
		return mktime(&sttm);
	}

	// 尝试解析 yyyy-mm-dd hh24:mi 格式
	if (sscanf(stime, "%d-%d-%d %d:%d", &sttm.tm_year, &sttm.tm_mon, 
			&sttm.tm_mday, &sttm.tm_hour, &sttm.tm_min) == 5)
	{
		sttm.tm_year -= 1900;
		sttm.tm_mon -= 1;
		sttm.tm_sec = 0;
		return mktime(&sttm);
	}

	// 尝试解析 yyyy-mm-dd hh24 格式
	if (sscanf(stime, "%d-%d-%d %d", &sttm.tm_year, &sttm.tm_mon, 
			&sttm.tm_mday, &sttm.tm_hour) == 4)
	{
		sttm.tm_year -= 1900;
		sttm.tm_mon -= 1;
		sttm.tm_min = 0;
		sttm.tm_sec = 0;
		return mktime(&sttm);
	}

	// 尝试解析 yyyy-mm-dd 格式
	if (sscanf(stime, "%d-%d-%d", &sttm.tm_year, &sttm.tm_mon, &sttm.tm_mday) == 3)
	{
		sttm.tm_year -= 1900;
		sttm.tm_mon -= 1;
		sttm.tm_hour = 0;
		sttm.tm_min = 0;
		sttm.tm_sec = 0;
		return mktime(&sttm);
	}

	// 尝试解析 yyyymmdd 格式
	if (sscanf(stime, "%4d%2d%2d", &sttm.tm_year, &sttm.tm_mon, &sttm.tm_mday) == 3)
	{
		sttm.tm_year -= 1900;
		sttm.tm_mon -= 1;
		sttm.tm_hour = 0;
		sttm.tm_min = 0;
		sttm.tm_sec = 0;
		return mktime(&sttm);
	}

	return 0;
}

/**
 * @brief 获取本地时间
 * @details 获取当前时间并转换为指定格式的字符串
 * @param stime 输出的字符串缓冲区
 * @param fmt 输出格式
 * @param timeval 时间偏移值（秒）
 */
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

/**
 * @brief 安全的格式化字符串函数
 * @details 将格式化的数据写入指定的缓冲区，确保不会发生缓冲区溢出
 * @param dest 目标缓冲区
 * @param destlen 目标缓冲区的大小
 * @param n 要写入的最大字符数
 * @param fmt 格式化字符串
 * @param ... 可变参数列表
 * @return 成功返回写入的字符数，失败返回-1
 */
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
