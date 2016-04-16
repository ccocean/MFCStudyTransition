#ifndef	__RH_SOCKET_H__
#define	__RH_SOCKET_H__

//#include <windows.h>
//#include <WinBase.h>
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>

#include <string.h>

#include <time.h>
typedef int			int32_t;
#define RHLISTENQUENUM			100
#define RHADDRMAXLEN			16

#define RHRETSUCCESS			0
#define RHRETFAIL				-1
#define COMMUN_CONNECT_TIMEOUT 1
// *****************************************************
// function	: 以阻塞模式TCP SOCKET 发送数据
// author 	: zhengyb		2014.9.1
// return   : Succes 0  fail -1
// parameter: Fd > 0
//            SndBuf 不为NULL
//			  SndLen 不为NULL ；*SndLen等于要发送数据长度 ；函数返回后*SndLen为已发送长度
// note		: 1.阻塞SOCKET
//			  2.此函数配合 RH_SetSndTimeoutFd 函数设定发送超时 更佳
//******************************************************
int RH_TcpSndBlockFd(int Fd, char *SndBuf, int *SndLen);


// *****************************************************
// function	: 以阻塞模式TCP SOCKET 接收数据
// author 	: zhengyb		2014.9.1
// return   : Succes 0  fail -1
// parameter: Fd > 0
//            RcvBuf 不为NULL
//			  RcvLen 不为NULL ；*RcvLen等于要接收数据长度 ；函数返回后*RcvLen为已接收长度
// note		: 1.阻塞SOCKET
//			  2.此函数配合 RH_SetRcvTimeoutFd 函数设定发送超时 更佳
//******************************************************
int RH_TcpRcvBlockFd(int Fd, char *RcvBuf, int needlen, int *RcvLen);

// *****************************************************
// function	: 创建Tcp SOCKET NoBind 模式
// author 	: zhengyb		2014.9.1
// return   : Succes fd / fail -1
// parameter: NULL
//******************************************************
int RH_CreateTcpNoBindFd(void); //推荐

// *****************************************************
// function	: 以阻塞模式SOCKET 连接服务器
// author 	: zhengyb		2014.9.1
// return   : Succes 0 / fail -1
// parameter: Fd > 0 / ServPort 大于等于 0 /ServIp 不为空 /Timeout 大于等于 0
// note		: 阻塞SOCKET
//******************************************************
int RH_ConnetBlockFd(int Fd, int ServPort, char *ServIp, int Timeout);

int32_t RH_Close(char *file, char *func, int32_t fd);

int RH_SetRcvTimeoutFd(int Fd, int TimeoutSec, int TimeoutUsec);

int32_t RH_Socket(char *file, char *func, int32_t domain, int32_t type, int32_t protocol);

int RH_SetSndTimeoutFd(int Fd, int TimeoutSec, int TimeoutUsec);
typedef enum {
	NS_DEBUG = 20,
	NS_INFO = 40,
	NS_NOTICE = 60,
	NS_WARN = 80,
	NS_ERROR = 100,
	NS_FATAL = 120
} share_log_level;
int share_outputlog(share_log_level levelog, const char *szFormat, ...);
#endif





