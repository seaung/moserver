#include "public.h"
#include "log.h"
#include "utils.h"

/**
 * @brief 构造函数，初始化日志对象
 * 
 * 初始化成员变量：
 * - 文件描述符设为0
 * - 清空文件名和打开模式缓冲区
 * - 设置默认的备份和缓冲标志
 * - 设置默认的最大日志文件大小
 */
Log::Log()
{
	m_tracefd = 0;
	memset(m_log_filename, 0, sizeof(m_log_filename));
	memset(m_open_mode, 0, sizeof(m_open_mode));

	m_bis_backup = true;
	m_bis_buffer = false;
	m_max_log_size = 1024;
	if (m_max_log_size < 10)
	{
		m_max_log_size = 10;
	}
}

/**
 * @brief 打开日志文件
 * 
 * @param filename 日志文件名
 * @param open_mode 文件打开模式，默认为NULL，此时使用"a+"模式
 * @param bis_backup 是否启用日志备份功能，默认为true
 * @param bis_buffer 是否启用文件缓冲，默认为false
 * @return true 文件打开成功
 * @return false 文件打开失败
 */
bool Log::OpenFile(const char *filename, const char *open_mode, bool bis_backup, bool bis_buffer)
{
	CloseLogFile();

	strcpy(m_log_filename, filename);
	m_bis_buffer = bis_buffer;
	m_bis_backup = bis_backup;
	
	if (open_mode == 0)
	{
		StrCopy(m_open_mode, sizeof(m_open_mode), "a+");
	}
	else
	{
		StrCopy(m_open_mode, sizeof(m_open_mode), open_mode);
	}

	if ((m_tracefd = FOpen(m_log_filename, open_mode)) == 0)
	{
		return false;
	}

	return true;
}

/**
 * @brief 检查并备份日志文件
 * 
 * 当日志文件大小超过设定的最大值时，将当前日志文件重命名为带时间戳的备份文件，
 * 并创建新的日志文件继续写入。
 * 
 * @return true 备份成功或无需备份
 * @return false 备份过程中出现错误
 */
bool Log::WriteBackupLogFile()
{
	if (m_tracefd == 0)
	{
		return false;
	}

	if (m_bis_backup == false) {
		return true;
	}

	if (ftell(m_tracefd) > m_max_log_size * 1024 * 1024)
	{
		fclose(m_tracefd);
		m_tracefd = 0;

		char str_local_time[21];
		memset(str_local_time, 0, sizeof(str_local_time));
		LocalTime(str_local_time, "yyy-mm-dd-hh24-mi-ss");

		char bak_filename[301];
		SNPrintf(bak_filename, sizeof(bak_filename), 300, "%s.%s", m_log_filename, str_local_time);
		rename(m_log_filename, bak_filename);

		if((m_tracefd= FOpen(m_log_filename, m_open_mode)) == 0)
		{
			return false;
		}
	}

	return true;
}

/**
 * @brief 写入带时间戳的日志
 * 
 * 在日志内容前添加当前时间戳，支持格式化输出。
 * 如果需要，会在写入前执行日志文件备份。
 * 
 * @param fmt 格式化字符串
 * @param ... 可变参数列表
 * @return true 写入成功
 * @return false 写入失败
 */
bool Log::WriteLog(const char *fmt, ...)
{
	if (m_tracefd == 0)
	{
		return false;
	}

	if (WriteBackupLogFile() == false)
	{
		return false;
	}

	char strtime[20];
	LocalTime(strtime);
	va_list ap;
	va_start(ap, fmt);
	fprintf(m_tracefd, "%s ", strtime);
	vfprintf(m_tracefd, fmt, ap);
	va_end(ap);

	if (m_bis_buffer == false)
	{
		fflush(m_tracefd);
	}

	return true;
}

/**
 * @brief 写入日志（不带时间戳）
 * 
 * 直接写入日志内容，支持格式化输出。
 * 主要用于需要自定义日志格式的场景。
 * 
 * @param fmt 格式化字符串
 * @param ... 可变参数列表
 * @return true 写入成功
 * @return false 写入失败
 */
bool Log::WriteLogEx(const char *fmt, ...)
{
	if (m_tracefd == 0)
	{
		return false;
	}

	va_list ap;
	va_start(ap, fmt);
	vfprintf(m_tracefd, fmt, ap);
	va_end(ap);

	if (m_bis_buffer == false)
	{
		fflush(m_tracefd);
	}

	return true;
}

/**
 * @brief 关闭日志文件
 * 
 * 关闭文件描述符，清空文件名和打开模式缓冲区，
 * 重置备份和缓冲标志为默认值。
 */
void Log::CloseLogFile()
{
	if (m_tracefd != 0)
	{
		fclose(m_tracefd);
		m_tracefd = 0;
	}

	memset(m_log_filename, 0, sizeof(m_log_filename));
	memset(m_open_mode, 0, sizeof(m_open_mode));
	m_bis_backup = true;
	m_bis_buffer = false;
}

/**
 * @brief 析构函数
 * 
 * 确保在对象销毁时关闭日志文件，释放相关资源
 */
Log::~Log()
{
	CloseLogFile();
}
