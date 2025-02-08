#ifndef __LOG_H__
#define __LOG_H__
#include "public.h"

/**
 * @brief 日志处理类，用于管理日志文件的创建、写入和备份
 * 
 * 该类提供了日志文件的基本操作功能，包括：
 * - 日志文件的打开和关闭
 * - 日志内容的写入（支持格式化输出）
 * - 日志文件的自动备份
 * - 缓冲区控制
 */

class Log
{
	public:
		FILE *m_tracefd;
		char m_log_filename[300];
		char m_open_mode[12];
		bool m_bis_buffer;
		bool m_bis_backup;
		long m_max_log_size;

		Log();

		bool OpenFile(const char *filename, const char *open_mode = 0, bool bis_backup = true, bool bis_buffer = false);

		bool WriteBackupLogFile();

		bool WriteLog(const char *fmt, ...);

		bool WriteLogEx(const char *fmt, ...);

		void CloseLogFile();

		~Log();
};

#endif
