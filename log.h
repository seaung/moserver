#ifndef __LOG_H__
#define __LOG_H__
#include "public.h"

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
