#pragma once
#include "session/tcp_socket.h"
#include "session/io_service_help.h"
#include "Markup.h"

class ISessionClientObserver
{
public:
	virtual void OnReceive(const char* buff, int len) {}
};

class CSessionClient : public IIOObserver 
{
public:
	CSessionClient(void);
	~CSessionClient(void);
	
	typedef std::list<ISessionClientObserver*> observer_list;
	observer_list m_listOb;
	void RegisterObserver(ISessionClientObserver* ob);
	void RemoveObserver(ISessionClientObserver* ob);

public:
	bool CreateLink(const std::string ip, unsigned short port);                 // 创建连接
	void CloseLink();															// 关闭连接
	virtual void handle_receive(const char* buff, int len, int bFlag);         // 接收
	virtual void handle_write(const char* buff, int len);						// 发送
	
	int SendCmd(char* pBuf, int nLen, DWORD interval = 3000);
	bool is_open()
	{
		if (sp_ses_)
		{
			return sp_ses_->is_connect();
		}
		return false;
	}
	bool is_connect()
	{
		if (sp_ses_)
		{
			return sp_ses_->is_connect_;
		}
		return false;
	}

public:
	// 需要返回指令的打包
	bool packet(unsigned short msgCode, ::CMarkup& xml);
	void packet(unsigned short msgCode, ::CMarkup& xml, bool flag);
	// 登录命令
	bool LoginAll(CString passkey, CString user, CString passwd, UINT optType);
	// 发送心跳包
	bool SendHeartBeat(int rcdStatus, int rcdTime, int mpMode, int mpLayout);
	// 请求码流
	bool RequestStream(int stream);
	// 获取磁盘空间容量
	bool GetDiskSpace();
	// 设置摄像头远遥 
	bool ControlCamera(int ID, int type, int addr, int speed, int num, int status);
	// 设置logo
	bool SetLogo(BOOL bFlag, int transparency, int length, int fmt, UINT pos, int xPos, int yPos, BOOL bEnable);
	// 设置字幕&&时间
	bool SetTitleAndDate(BOOL bFlag, int nPos, CString title, BOOL flag, int alpha, int xPos, int yPos, BOOL enable);
	// 设置录制状态
	bool SetRecordStatus(int recType, CString roomName, CString teacherName, CString courseName, CString notes);
	// 设置/获取电影模式
	bool SetOrGetMode(BOOL flag, int sourceNum, int width, int height, int mplayout, int frameRate, int BitRate);
	// 设置/获取首画面
	bool SetOrGetFirstPic(BOOL flag, UINT mode, UINT time);
	// 获取版本号
	bool GetVersion();
	// 设置导播台控制模式
	bool SetManageMode(UINT mode);
	// FTPTools查询课件列表
	bool FTP_RequestList(CString key, int order, int curPage, int PageSize = 10);
	// FTPTools修改课件信息(包括课件名、备注、教师名以及课程名)
	bool FTP_ChangeInfo(CString strOld, CString strNew,CString mode, CString strNotes, CString strTeacher = NULL, CString strLession = NULL);
	// FTPTools删除课件
	bool FTP_Delete(CStringArray& arrName);
	// 发送数据
	void SendData(char* buff, int len);

private:
	S_CONNECT_INFO m_conInfo;
	//boost::shared_ptr<ISession> sp_ses_;
	CTCPSocket* sp_ses_;
	bool is_online;

public:
	CEvent m_eve;
	::CMarkup m_xml;
	::CMarkup m_heartXml;
	::CMarkup m_kickXml;
	char m_recvBuf[10240];
	int m_length;
};
