#include "tcpsocket.h"
#include "public.h"

/*
 * 函数功能：向TCP连接写入数据
 * 参数说明：
 *   sockfd      - socket文件描述符
 *   buffer      - 待发送数据的缓冲区
 *   ibuffer_len - 待发送数据的长度，如果为0则按字符串处理
 * 返回值：
 *   true  - 发送成功
 *   false - 发送失败
 */
bool TCPWrite(const int sockfd, const char * buffer, const int ibuffer_len)
{
	// 检查socket是否有效
	if (sockfd == -1)
	{
		return false;
	}

	// 计算需要发送的数据长度
	int ilen = 0;
	if (ibuffer_len == 0)
	{
		// 如果未指定长度，则按字符串处理
		ilen = strlen(buffer);
	}
	else
	{
		// 使用指定的长度
		ilen = ibuffer_len;
	}

	// 转换为网络字节序
	int ilen_byte = htonl(ilen);

	// 准备发送缓冲区，包含长度头和数据
	char TCPBuffer[ilen + 4];
	memset(TCPBuffer, 0, sizeof(TCPBuffer));
	memcpy(TCPBuffer, &ilen_byte, 4);
	memcpy(TCPBuffer + 4, buffer, ilen);

	// 发送数据
	if (TCPWriteN(sockfd, TCPBuffer, ilen + 4) == false)
	{
		return false;
	}
	return true;
}

/*
 * 函数功能：从TCP连接读取数据
 * 参数说明：
 *   sockfd      - socket文件描述符
 *   buffer      - 接收数据的缓冲区
 *   ibuffer_len - 接收到的数据长度
 *   itimeout    - 超时时间(秒)，0表示不超时，>0表示超时时间，-1表示无限等待
 * 返回值：
 *   true  - 读取成功
 *   false - 读取失败或超时
 */
bool TCPRead(const int sockfd, char *buffer, int *ibuffer_len, const int itimeout)
{
	// 检查socket是否有效
	if (sockfd == -1)
	{
		return false;
	}

	// 处理超时设置
	if (itimeout > 0)
	{
		// 使用poll进行超时检测
		struct pollfd pfd;
		pfd.fd = sockfd;
		pfd.events = POLLIN;
		if (poll(&pfd, 1, itimeout * 1000) <= 0)
		{
			return false;
		}
	}

	// 处理无限等待的情况
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

	// 初始化接收长度
	(*ibuffer_len) = 0;

	// 先读取4字节的长度信息
	if (TCPReadN(sockfd, (char *)ibuffer_len, 4) == false)
	{
		return false;
	}

	// 转换网络字节序为主机字节序
	(*ibuffer_len) = ntohl(*ibuffer_len);

	// 读取实际数据
	if (TCPReadN(sockfd, buffer, (*ibuffer_len)) == false)
	{
		return false;
	}

	return true;
}

/*
 * 函数功能：从TCP连接读取指定字节数的数据
 * 参数说明：
 *   sockfd - socket文件描述符
 *   buffer - 接收数据的缓冲区
 *   n      - 需要读取的字节数
 * 返回值：
 *   true  - 读取成功
 *   false - 读取失败
 */
bool TCPReadN(const int sockfd, char * buffer, const size_t n)
{
	// 剩余需要读取的字节数
	int ileft = n;
	// 已经读取的字节数
	int irbytes = 0;
	// 本次读取的字节数
	int iwbytes;

	// 循环读取，直到读取完指定的字节数或发生错误
	while (ileft > 0)
	{
		if ((iwbytes = recv(sockfd, buffer + irbytes, ileft, 0)) <= 0)
		{
			return false;
		}
		// 更新剩余需要读取的字节数和已读取的字节数
		ileft -= iwbytes;
		irbytes += iwbytes;
	}

	return true;
}

/*
 * 函数功能：向TCP连接写入指定字节数的数据
 * 参数说明：
 *   sockfd - socket文件描述符
 *   buffer - 待发送数据的缓冲区
 *   n      - 需要发送的字节数
 * 返回值：
 *   true  - 发送成功
 *   false - 发送失败
 */
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

/*
 * 函数功能：TCPClient类构造函数
 * 功能说明：初始化TCPClient对象的成员变量
 */
TCPClient::TCPClient()
{
	m_connfd = -1;
	memset(m_host, 0, sizeof(m_host));
	m_port = 0;
	m_timeout = false;
}

/*
 * 函数功能：创建TCP客户端连接
 * 参数说明：
 *   host - 服务器主机名或IP地址
 *   port - 服务器端口号
 * 返回值：
 *   true  - 连接成功
 *   false - 连接失败
 */
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

/*
 * 函数功能：从服务器读取数据
 * 参数说明：
 *   buffer   - 接收数据的缓冲区
 *   itimeout - 超时时间(秒)，0表示不超时
 * 返回值：
 *   true  - 读取成功
 *   false - 读取失败或超时
 */
bool TCPClient::ReadBuffer(char *buffer, const int itimeout)
{
	// 检查socket连接是否有效
	if (m_connfd == -1)
	{
		return false;
	}

	// 如果设置了超时时间，则进行超时检测
	if (itimeout > 0)
	{
		struct pollfd pfd;
		pfd.fd = m_connfd;
		pfd.events = POLLIN;
		int iret;
		m_timeout = false;

		// 等待数据到达或超时
		if ((iret = poll(&pfd, 1, itimeout * 1000)) <= 0)
		{
			if (iret == 0)
			{
				m_timeout = true; // 设置超时标志
			}
			return false;
		}
	}

	m_buffer_len = 0;
	// 调用底层TCPRead函数读取数据
	return (TCPRead(m_connfd, buffer, &m_buffer_len));
}

/*
 * 函数功能：向服务器发送数据
 * 参数说明：
 *   buffer      - 待发送数据的缓冲区
 *   ibuffer_len - 待发送数据的长度，如果为0则按字符串处理
 * 返回值：
 *   true  - 发送成功
 *   false - 发送失败
 */
bool TCPClient::WriteBuffer(const char *buffer, const int ibuffer_len)
{
	// 检查socket连接是否有效
	if (m_connfd == -1)
	{
		return false;
	}

	// 计算需要发送的数据长度
	int ilen = ibuffer_len;
	if (ilen == 0)
	{
		ilen = strlen(buffer);
	}

	// 调用底层TCPWrite函数发送数据
	return (TCPWrite(m_connfd, buffer, ilen));
}

/*
 * 函数功能：关闭TCP客户端连接
 * 功能说明：关闭socket连接并重置成员变量
 */
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

/*
 * 函数功能：TCPServer类构造函数
 * 功能说明：初始化TCPServer对象的成员变量
 */
TCPServer::TCPServer()
{
	m_listenfd = -1;
	m_clientfd = -1;
	m_socklen = 0;
	m_btimeout = false;
}

/*
 * 函数功能：创建TCP服务器
 * 参数说明：
 *   port    - 监听端口
 *   backlog - 连接队列最大长度
 * 返回值：
 *   true  - 创建成功
 *   false - 创建失败
 */
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

/*
 * 函数功能：接受客户端连接
 * 返回值：
 *   true  - 接受连接成功
 *   false - 接受连接失败
 */
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

/*
 * 函数功能：获取已连接客户端的IP地址
 * 返回值：客户端IP地址字符串
 */
char *TCPServer::GetClientIP()
{
	return (inet_ntoa(m_cliaddr.sin_addr));
}

/*
 * 函数功能：从客户端读取数据
 * 参数说明：
 *   buffer   - 接收数据的缓冲区
 *   itimeout - 超时时间(秒)，0表示不超时
 * 返回值：
 *   true  - 读取成功
 *   false - 读取失败或超时
 */
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

/*
 * 函数功能：向客户端发送数据
 * 参数说明：
 *   buffer      - 待发送数据的缓冲区
 *   ibuffer_len - 待发送数据的长度，如果为0则按字符串处理
 * 返回值：
 *   true  - 发送成功
 *   false - 发送失败
 */
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

/*
 * 函数功能：关闭服务器socket
 * 功能说明：关闭监听socket和客户端socket
 */
void TCPServer::CloseServerSocket()
{
	if (m_listenfd > 0)
	{
		close(m_listenfd);
		close(m_clientfd);
	}
}

/*
 * 函数功能：关闭客户端socket
 * 功能说明：关闭当前连接的客户端socket
 */
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
