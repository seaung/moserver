#include "public.h"
#include "log.h"

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

bool Log::OpenFile(const char *filename, const char *open_mode, bool bis_backup, bool bis_buffer)
{
	CloseLogFile();

	strcpy(m_log_filename, filename);
	m_bis_buffer = bis_buffer;
	m_bis_backup = bis_backup;
	
	if (open_mode == 0)
	{
		strcpy(m_open_mode, "a");
	}
	else
	{
		strcpy(m_open_mode, "a+");
	}

	return true;
}

bool Log::WriteBackupLogFile()
{
	return true;
}

bool Log::WriteLog(const char *fmt, ...)
{
	return true;
}

bool Log::WriteLogEx(const char *fmt, ...)
{
	return true;
}

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

Log::~Log()
{
	CloseLogFile();
}
