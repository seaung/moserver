#ifndef __TCPSOCKET__
#define __TCPSOCKET__
#include "public.h"

bool TCPWrite(const int sockfd, const char * buffer, const int ibuffer_len);

bool TCPWriteN(const int sockfd, char *buffer, const size_t n);

bool TCPRead(const int sockfd, char *buffer, int *ibuffer_len, const int itimeout);

bool TCPReadN(const int sockfd, char * buffer, const size_t n);

// TCP Client类
class TCPClient
{
	public:
		/*
		 * m_connfd     客户端socket文件句柄
		 * m_host       服务端主机地址
		 * m_port       服务端端口地址
		 * m_timeout    用于调用ReadBuffer方法，调用失败原因是否超时，true为超时，false为未超时
		 * m_buffer_len 用于调用ReadBuffer方法，接收到的报文的大小，单位为，字节
		 * */
		int  m_connfd;
		char m_host[32]; 
		int  m_port; 
		bool m_timeout;
		int  m_buffer_len;

		TCPClient(); // TCPClient构造函数

		/*
		 * 用于创建一个TCP连接
		 * host 为服务监听的主机地址
		 * int  为服务监听的主机端口
		 * 返回值为 true为成功，false为失败
		 * */
		bool NewTCPClient(const char *host, const int port);

		/*
		 * 用于接收服务的发送过来的数据
		 * buffer 用于接收数据的的缓冲区地址, 接收的长度为m_buffer_len
		 * itimeout 等待接收数据超时时间，单位为秒，缺省值为0表示无限等待
		 * 返回值为 true为成功 false为失败，失败的情况有两种1.等待超时，成员变量m_timeout的值被设置为true, 2. socket连接不可用
		 * */
		bool ReadBuffer(char *buffer, const int itimeout = 0);

		/*
		 * 用于向服务端发送数据
		 * buffer 待发送数据缓冲区的地址
		 * ibuffer_len 待发送数据大小，单位为字节，缺省值为0，如果发送的是ascii字符串，ibuffer_len取0
		 * 如果是二进制流数据则ibuffer_len为二进制数据块的大小
		 * 返回值 true为成功 false为失败,如果失败表示socket连接已不可用
		 * */
		bool WriteBuffer(const char *buffer, const int ibuffer_len = 0);

		void Close(); // 关闭连接

		~TCPClient(); // 释放资源
};

class TCPServer
{
	public:
		int  m_listenfd;
		int  m_clientfd;
		bool m_btimeout;
		int  m_ibuffer_len;
	private:
		int    m_socklen;
		struct sockaddr_in m_servaddr;
		struct sockaddr_in m_cliaddr;

	TCPServer();

	bool NewServer(const unsigned int port, const int backlog = 5);

	bool Accept();

	char *GetClientIP();

	bool TCPRead(char *buffer, const int itimeout = 0);

	bool TCPWrite(const char *buffer, const int ibuffer_len = 0);

	void CloseServerSocket();

	void CloseClientSocket();

	~TCPServer();
};

#endif
