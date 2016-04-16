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
	bool CreateLink(const std::string ip, unsigned short port);                 // ��������
	void CloseLink();															// �ر�����
	virtual void handle_receive(const char* buff, int len, int bFlag);         // ����
	virtual void handle_write(const char* buff, int len);						// ����
	
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
	// ��Ҫ����ָ��Ĵ��
	bool packet(unsigned short msgCode, ::CMarkup& xml);
	void packet(unsigned short msgCode, ::CMarkup& xml, bool flag);
	// ��¼����
	bool LoginAll(CString passkey, CString user, CString passwd, UINT optType);
	// ����������
	bool SendHeartBeat(int rcdStatus, int rcdTime, int mpMode, int mpLayout);
	// ��������
	bool RequestStream(int stream);
	// ��ȡ���̿ռ�����
	bool GetDiskSpace();
	// ��������ͷԶң 
	bool ControlCamera(int ID, int type, int addr, int speed, int num, int status);
	// ����logo
	bool SetLogo(BOOL bFlag, int transparency, int length, int fmt, UINT pos, int xPos, int yPos, BOOL bEnable);
	// ������Ļ&&ʱ��
	bool SetTitleAndDate(BOOL bFlag, int nPos, CString title, BOOL flag, int alpha, int xPos, int yPos, BOOL enable);
	// ����¼��״̬
	bool SetRecordStatus(int recType, CString roomName, CString teacherName, CString courseName, CString notes);
	// ����/��ȡ��Ӱģʽ
	bool SetOrGetMode(BOOL flag, int sourceNum, int width, int height, int mplayout, int frameRate, int BitRate);
	// ����/��ȡ�׻���
	bool SetOrGetFirstPic(BOOL flag, UINT mode, UINT time);
	// ��ȡ�汾��
	bool GetVersion();
	// ���õ���̨����ģʽ
	bool SetManageMode(UINT mode);
	// FTPTools��ѯ�μ��б�
	bool FTP_RequestList(CString key, int order, int curPage, int PageSize = 10);
	// FTPTools�޸Ŀμ���Ϣ(�����μ�������ע����ʦ���Լ��γ���)
	bool FTP_ChangeInfo(CString strOld, CString strNew,CString mode, CString strNotes, CString strTeacher = NULL, CString strLession = NULL);
	// FTPToolsɾ���μ�
	bool FTP_Delete(CStringArray& arrName);
	// ��������
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
