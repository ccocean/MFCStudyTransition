#pragma once
#include "afxsock.h"
#include "Markup.h"
#include "FTPTypeDef.h"
#include "UIUtil.h"
#include <string>
class CMFCTransitionDlg;


#define LOGIN_SUCCESS _T("30015")
#define HEART_BEAT _T("30032")
#define SETMOVIE_CODE     _T("30047")    // ����&&��ȡ��Ӱģʽ
#define MODE_CODE         _T("30060")    // ���õ���̨����ģʽ
#define SETRCD_CODE       _T("30010")   // ����¼��״̬


class ClientSocket :
	public CSocket
{
public:
	CMFCTransitionDlg *m_pDlg;
	ClientSocket();
	~ClientSocket();
	void SetParam(CMFCTransitionDlg *pDlg);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags /* = 0 */);
	virtual BOOL OnMessagePending();
	BOOL SendMSG(LPSTR lpBuff, int nlen);
	BOOL Packet(unsigned short msgCode, ::CMarkup &xml);
	void ParseXml(::CMarkup xml);
	BOOL SetReconnectTimer(UINT nTimeOut);
	//static DWORD WINAPI ParseXml(LPVOID pParam);
	::CMarkup m_xml;
	int m_timeout;
	int m_count;
protected:
	int m_nTimerID;
	int m_nTimerReconnect;
private:
	
	BOOL KillReconnectTimer();
	BOOL KillTimeOut();
	BOOL SetTimeOut(UINT nTimeOut);
};

