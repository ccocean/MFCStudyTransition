#include "stdafx.h"
#include "ClientSocket.h"
#include "MFCTransition.h"
#include "MFCTransitionDlg.h"


ClientSocket::ClientSocket()
{
}


ClientSocket::~ClientSocket()
{
}

void ClientSocket::SetParam(CMFCTransitionDlg *pDlg)
{
	m_pDlg = pDlg;
	m_count = 0;
}

void ClientSocket::OnReceive(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	char *pData = NULL;
	
	//pData = new char[10240];
	//memset(pData, 0, sizeof(char)*10240);
	External_Mag_Head msgHead;
	int msgLen = sizeof(External_Mag_Head);
	UINT leng = 0;
	CString strDoc;
	pData = new char[msgLen];
	memset(pData, 0, sizeof(char)*msgLen);
	SetTimeOut(3000);
	leng = Receive(pData, msgLen, 0);
	KillTimeOut();
	AsyncSelect(FD_READ);
	memcpy(&msgHead, pData, msgLen);
	free(pData);
	pData = new char[msgHead.load_len + 1];
	
	memset(pData, 0, sizeof(char)*msgHead.load_len + 1);
	SetTimeOut(3000);
	leng = Receive(pData, msgHead.load_len, 0);
	KillTimeOut();
	AsyncSelect(FD_READ);
	//char *p_msgBody = (char*)malloc(sizeof(char)*msgHead.load_len);
	//memset(p_msgBody, 0, sizeof(char)*msgHead.load_len);
	//memcpy(p_msgBody, pData + msgLen, sizeof(char)*msgHead.load_len);
	strDoc = StringConvertor::Utf8ToWide(pData);
	m_xml.SetDoc(strDoc);
	ParseXml(m_xml);
	/*DWORD *pParams = new DWORD[2];
	pParams[0] = (DWORD)this;
	pParams[1] = (DWORD)pData;
	m_hThread = ::CreateThread(NULL, NULL, ParseXml, (LPVOID*)(pParams), NULL, NULL);*/
	
	delete pData;
	CSocket::OnReceive(nErrorCode);
}

int ClientSocket::Send(const void* lpBuf, int nBufLen, int nFlags = 0/* = 0 */)
{
	SetTimeOut(3000);
	int nSend = CSocket::Send(lpBuf, nBufLen, nFlags);
	KillTimeOut();
	return nSend;
}

BOOL ClientSocket::SendMSG(LPSTR lpBuff, int nlen)
{
	
	int nRes = Send(lpBuff, nlen);
	if (nRes == SOCKET_ERROR)
	{
		if (this->m_pDlg->m_connet == TRUE)
		{
			this->m_pDlg->UpdateLog(_T("Send Error!"));
			return FALSE;
		}
		else
		{
			this->m_pDlg->UpdateLog(_T("Disconnected!"));
			return FALSE;
		}
	}
	return TRUE;
}
//DWORD WINAPI ClientSocket::ParseXml(LPVOID pParam)
//{
//	DWORD *in = (DWORD*)pParam;
//	ClientSocket *p_socket = (ClientSocket *)in[0];
//	char *pData = (char*)in[1];
//	CString strDoc;
//	strDoc = StringConvertor::Utf8ToWide(pData);
//	p_socket->m_xml.SetDoc(strDoc);
//
//	CString strCode, strRes;
//	if (p_socket->m_xml.FindChildElem(MSGHEAD))
//	{
//		p_socket->m_xml.IntoElem();
//		if (p_socket->m_xml.FindChildElem(MSGCODE))
//		{
//			strCode = p_socket->m_xml.GetChildData();
//		}
//		if (p_socket->m_xml.FindChildElem(RESULT))
//		{
//			strRes = p_socket->m_xml.GetChildData();
//		}
//	}
//	if (strCode==LOGIN_SUCCESS&&strRes=="1")
//	{
//		p_socket->m_pDlg->SetTimer(1, 3000, NULL);
//		//theApp.m_strServerIP = m_ipStr;
//		theApp.m_connet = TRUE;
//		p_socket->m_pDlg->UpdateLog(_T("Login Success!"));
//		return TRUE;
//	}
//	else if (strCode==HEART_BEAT)
//	{
//		p_socket->m_pDlg->UpdateLog(_T("Heartbeat from Server!"));
//		return TRUE;
//	}
//	else
//	{
//		p_socket->m_timeout++;
//		p_socket->m_pDlg->UpdateLog(_T("Recv Error!"));
//		return FALSE;
//	}
//}

void ClientSocket::ParseXml(::CMarkup xml)
{
	CString strCode,strRes;
	if (m_xml.FindChildElem(MSGHEAD))
	{
		m_xml.IntoElem();
		if (m_xml.FindChildElem(MSGCODE))
		{
			strCode = m_xml.GetChildData();
		}
		if (m_xml.FindChildElem(RESULT))
		{
			strRes = m_xml.GetChildData();
		}
		m_xml.OutOfElem();
	}
	else
	{
		m_pDlg->UpdateLog(_T("XML文件解析错误!"));
		return;
	}
	if (strCode==LOGIN_SUCCESS&&strRes=="1")
	{
		m_pDlg->SetTimer(1, 3000, NULL);
		//theApp.m_strServerIP = m_ipStr;
		this->m_pDlg->m_connet = TRUE;
		m_pDlg->UpdateLog(_T("Login Success!"));
		m_pDlg->m_bmpB_Enter.LoadBitmaps(IDB_BITMAP_ENTER_WORK, 0, 0, 0);
		m_pDlg->m_bmpB_Enter.SizeToContent();
		return;
	}
	else if (strCode==HEART_BEAT)
	{
		this->KillReconnectTimer();
		
		CString record, model, layout;
		if (m_xml.FindChildElem(MSGBODY))
		{
			m_xml.IntoElem();
			if (m_xml.FindChildElem(RECORDSTATUS))
			{
				record = m_xml.GetChildData();
				m_pDlg->up_e = _ttoi(record);
				//m_pDlg->isSet = E;
				//if (!m_pDlg->CheckUpDown(m_pDlg->up_e, m_pDlg->dn_e))
				//{
				//	//m_pDlg->m_e = _ttoi(record);
				//	m_pDlg->isSet = TRUE;
				//}
			}
			if (m_xml.FindChildElem(MPMODEL))
			{
				model = m_xml.GetChildData();
				m_pDlg->up_f = _ttoi(model);
				//m_pDlg->isSet = D;
				//if (!m_pDlg->CheckUpDown(m_pDlg->up_f, m_pDlg->dn_f))
				//{
				//	//m_pDlg->m_f = _ttoi(model);
				//	m_pDlg->isSet = TRUE;
				//}
			}
			if (m_xml.FindChildElem(MPLAYOUT))
			{
				layout = m_xml.GetChildData();
				m_pDlg->up_d = _ttoi(layout);
				//m_pDlg->isSet = F;
				//if (!m_pDlg->CheckUpDown(m_pDlg->up_d, m_pDlg->dn_d))
				//{
				//	//m_pDlg->m_d = _ttoi(layout);
				//	m_pDlg->isSet = TRUE;
				//}
			}
		}
		
		::PostMessage(m_pDlg->GetSafeHwnd(), WM_SEND2SERIAL, 0, 0);
		//m_pDlg->UpdateLog(_T("Heartbeat from Server!"));
	}
	else if (strCode == SETRCD_CODE&&strRes == "1")
	{
		m_pDlg->UpdateLog(_T("录制模式设置成功！"));
	}
	else if (strCode == MODE_CODE&&strRes == "1")
	{
		m_pDlg->UpdateLog(_T("导播模式设置成功！"));
	}
	else if (strCode == SETMOVIE_CODE&&strRes == "1")
	{
		m_pDlg->UpdateLog(_T("电影模式设置成功！"));
	}
	else
	{
		//m_timeout++;
		//if (m_timeout>10)
		//{
		//	::PostMessage(m_pDlg->GetSafeHwnd(), WM_USER_TIMEOUT, 0, 0);
		//}
		m_pDlg->UpdateLog(_T("Recv Error!"));
	}
}

BOOL ClientSocket::Packet(unsigned short msgCode, ::CMarkup &xml)
{
	char *p_sendBuf = NULL;
	int nLen = 0;
	CString strDoc = xml.GetDoc();
	std::string str = StringConvertor::WideToUtf8(strDoc);
	External_Mag_Head msgHead;
	nLen += sizeof(msgHead);
	memcpy(msgHead.identifier, "@#$&", 4);
	msgHead.version = 2015;
	msgHead.load_len = str.length();
	msgHead.msg_code = msgCode;
	msgHead.reserver = 0;
	p_sendBuf = new char[nLen + str.length()];
	memcpy(p_sendBuf, &msgHead, sizeof(msgHead));
	memcpy(p_sendBuf + nLen, str.c_str(), str.length());
	nLen += str.length();
	BOOL nRes = SendMSG(p_sendBuf, nLen);
	free(p_sendBuf);
	p_sendBuf = NULL;
	return nRes;
}

void ClientSocket::OnClose(int nErrorCode)
{
	//m_pDlg->m_editLog.Clear();
	m_pDlg->m_listLog.ResetContent();
	AfxMessageBox(_T("服务器关闭！"));
	this->Close();
	delete this;
}

BOOL ClientSocket::OnMessagePending()
{
	MSG msg;
	if (::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE))
	{
		if (msg.wParam == (UINT)m_nTimerID)
		{
			// Remove the message and call CancelBlockingCall.
			::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
			CancelBlockingCall();
			return FALSE;  // No need for idle time processing.
		};
		if (msg.wParam == (UINT)m_nTimerReconnect)
		{
			::PostMessage(m_pDlg->GetSafeHwnd(), WM_USER_TIMEOUT,0,0);
			// Remove the message and call CancelBlockingCall.
			::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
			CancelBlockingCall();
			return FALSE;  // No need for idle time processing.
		}
	};
	return CSocket::OnMessagePending();
}

BOOL ClientSocket::SetTimeOut(UINT nTimeOut)
{
	m_nTimerID = SetTimer(NULL, 0, nTimeOut, NULL);
	return m_nTimerID;
}

BOOL ClientSocket::KillTimeOut()
{
	return KillTimer(NULL, m_nTimerID);
}

BOOL ClientSocket::SetReconnectTimer(UINT nTimeOut)
{
	m_nTimerReconnect = SetTimer(NULL, 0, nTimeOut, NULL);
	return m_nTimerReconnect;
}

BOOL ClientSocket::KillReconnectTimer()
{
	return KillTimer(NULL, m_nTimerReconnect);
}