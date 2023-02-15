#include "public.h"
#include "log.h"
#include "utils.h"

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
