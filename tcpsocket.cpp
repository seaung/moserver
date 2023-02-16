#include "tcpsocket.h"
#include "public.h"

bool TCPWrite(const int sockfd, const char * buffer, const int ibuffer_len)
{
	if (sockfd == -1)
	{
		return false;
	}

	int ilen = 0;

	if (ibuffer_len == 0)
	{
		ilen = strlen(buffer);
	}
	else
	{
		ilen = ibuffer_len;
	}

	int ilen_byte = htonl(ilen);

	char TCPBuffer[ilen + 4];
	memset(TCPBuffer, 0, sizeof(TCPBuffer));
	memcpy(TCPBuffer, &ilen_byte, 4);
	memcpy(TCPBuffer + 4, buffer, ilen);


	if (TCPWriteN(sockfd, TCPBuffer, ilen + 4) == false)
	{
		return false;
	}
	return true;
}

bool TCPReadN(const int sockfd, char * buffer, const size_t n)
{
	int ileft = n;
	int irbytes = 0;
	int iwbytes;

	while (ileft > 0)
	{
		if ((iwbytes = recv(sockfd, buffer + irbytes, ileft, 0)) <= 0)
		{
			return false;
		}
	}

	return true;
}

bool TCPWriteN(const int sockfd, char *buffer, const size_t n)
{
	int left = n;
	int rbytes = 0;
	int wbytes;

	while(left > 0)
	{
		if ((wbytes = send(sockfd, buffer + rbytes, left, 0)) <= 0)
		{
			return false;
		}
		left = left - wbytes;
		rbytes = rbytes + wbytes;
	}

	return true;
}

bool TCPRead(const int sockfd, char *buffer, int *ibuffer_len, const int itimeout)
{
	if (sockfd == -1)
	{
		return false;
	}

	if (itimeout > 0)
	{
		struct pollfd pfd;
		pfd.fd = sockfd;
		pfd.events = POLLIN;
		if (poll(&pfd, 1, itimeout * 1000) <= 0)
		{
			return false;
		}
	}

	if (itimeout == -1)
	{
		struct pollfd pfd;
		pfd.fd = sockfd;
		pfd.events = POLLIN;
		if (poll(&pfd, 1, itimeout * 1000) <= 0)
		{
			return false;
		}
	}

	(*ibuffer_len) = 0;

	if (TCPReadN(sockfd, (char *)ibuffer_len, 4) == false)
	{
		return false;
	}

	(*ibuffer_len) = ntohl(*ibuffer_len);

	if (TCPReadN(sockfd, buffer, (*ibuffer_len)) == false)
	{
		return false;
	}

	return true;
}

TCPClient::TCPClient()
{
	m_connfd = -1;
	memset(m_host, 0, sizeof(m_host));
	m_port = 0;
	m_timeout = false;
}

bool TCPClient::NewTCPClient(const char *host, const int port)
{
	if (m_connfd != -1)
	{
		close(m_connfd);
		m_connfd = -1;
	}

	signal(SIGPIPE, SIG_IGN);

	strcpy(m_host, host);

	m_port = port;

	struct hostent* hstent;
	struct sockaddr_in serv_addr;

	if ((m_connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return false;
	}

	if (!(hstent = gethostbyname(m_host)))
	{
		close(m_connfd);
		m_connfd = -1;
		return false;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(m_port);
	memcpy(&serv_addr.sin_addr, hstent->h_addr_list, hstent->h_length);

	if (connect(m_connfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0)
	{
		close(m_connfd);
		m_connfd = -1;
		return false;
	}

	return true;
}

bool TCPClient::ReadBuffer(char *buffer, const int itimeout)
{
	if (m_connfd == -1)
	{
		return false;
	}

	if (itimeout > 0)
	{
		struct pollfd pfd;
		pfd.fd = m_connfd;
		pfd.events = POLLIN;
		int iret;
		m_timeout = false;

		if ((iret = poll(&pfd, 1, itimeout * 1000)) <= 0)
		{
			if (iret == 0)
			{
				m_timeout = false;
			}
			return false;
		}
	}

	m_buffer_len = 0;

	return true;
}

bool TCPClient::WriteBuffer(const char *buffer, const int ibuffer_len)
{
	return true;
}

void TCPClient::Close()
{
	if (m_connfd > 0)
	{
		close(m_connfd);
	}

	m_connfd = -1;
	memset(m_host, 0, sizeof(m_host));
	m_port = 0;
	m_timeout = false;
}

TCPClient::~TCPClient()
{
	Close();
}

TCPServer::TCPServer()
{
	m_listenfd = -1;
	m_clientfd = -1;
	m_socklen = 0;
	m_btimeout = false;
}

bool TCPServer::NewServer(const unsigned int port, const int backlog)
{
	if (m_listenfd > 0)
	{
		close(m_listenfd);
		m_listenfd = -1;
	}

	if ((m_listenfd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
	{
		return false;
	}

	signal(SIGPIPE, SIG_IGN);

	int sock_opt = 1;
	unsigned int len = sizeof(sock_opt);
	setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, &sock_opt, len);

	memset(&m_servaddr, 0, sizeof(m_servaddr));
	m_servaddr.sin_family = AF_INET;
	m_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_servaddr.sin_port = htons(port);

	if (bind(m_listenfd, (struct sockaddr *)&m_servaddr, sizeof(m_servaddr)) != 0)
	{
		CloseServerSocket();
		return false;
	}

	if (listen(m_listenfd, backlog) != 0)
	{
		CloseServerSocket();
		return false;
	}

	return true;
}

bool TCPServer::Accept()
{
	if (m_listenfd == -1)
	{
		return false;
	}

	m_socklen = sizeof(struct sockaddr_in);

	if ((m_clientfd = accept(m_listenfd, (struct sockaddr *)&m_cliaddr, (socklen_t*)&m_socklen)) < 0)
	{
		return false;
	}
	return true;
}

char *TCPServer::GetClientIP()
{
	return (inet_ntoa(m_cliaddr.sin_addr));
}

bool TCPServer::TCPReadBuffer(char *buffer, const int itimeout)
{
	if (m_clientfd == -1)
	{
		return false;
	}

	if (itimeout > 0)
	{
		struct pollfd pfd;
		pfd.fd = m_clientfd;
		pfd.events = POLLIN;
		m_btimeout = false;
		int iret;

		if ((iret = poll(&pfd, 1, itimeout * 1000)) <= 0)
		{
			if (iret == 0)
			{
				m_btimeout = true;
				return false;
			}
		}
	}

	m_ibuffer_len = 0;

	return (TCPRead(m_clientfd, buffer, &m_ibuffer_len));
}

bool TCPServer::TCPWriteBuffer(const char *buffer, const int ibuffer_len)
{
	if (m_clientfd == -1)
	{
		return false;
	}

	int ilen = ibuffer_len;

	if (ilen == 0)
	{
		ilen = strlen(buffer);
	}

	return (TCPWrite(m_clientfd, buffer, ilen));
}

void TCPServer::CloseServerSocket()
{
	if (m_listenfd > 0)
	{
		close(m_listenfd);
		close(m_clientfd);
	}
}

void TCPServer::CloseClientSocket()
{
	if (m_clientfd > 0)
	{
		close(m_clientfd);
		close(m_listenfd);
	}
}

TCPServer::~TCPServer()
{
	CloseServerSocket();
	CloseClientSocket();
}
