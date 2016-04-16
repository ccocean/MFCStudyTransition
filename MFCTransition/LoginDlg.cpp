// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCTransition.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "MFCTransitionDlg.h"


// LoginDlg �Ի���

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(LoginDlg::IDD, pParent)
{

}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipAddr);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &LoginDlg::OnBnClickedButtonLogin)
END_MESSAGE_MAP()


// LoginDlg ��Ϣ�������

void LoginDlg::SetSocket(ClientSocket * pSocket)
{
	m_pSocket = pSocket;
}

int LoginDlg::ParseXmlFile()
{
	::CMarkup xml = m_pSocket->m_xml;
	CString strCode, strPassKey, strResult;
	if (xml.FindChildElem(MSGHEAD))
	{
		xml.IntoElem();
		if (xml.FindChildElem(MSGCODE))
		{
			strCode = xml.GetChildData();
		}
		if (xml.FindChildElem(PASSKEY))
		{
			strPassKey = xml.GetChildData();
		}
		if (xml.FindChildElem(RESULT))
		{
			strResult = xml.GetChildData();
		}
		xml.OutOfElem();
	}
	return atoi(StringConvertor::WideToAnsi(strResult));
}

BOOL LoginDlg::CreateLoginXml(::CMarkup& xml, int &nRes)
{
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, ALL_LOGIN_CODE);
	xml.AddChildElem(PASSKEY, SERIALPLAT);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(USER, _T(""));
	xml.AddChildElem(PASSWD, _T(""));
	xml.AddChildElem(OPTTYPE, 1);
	if (m_pSocket->Packet(ALL_LOGIN_CODE, xml))
	{
		nRes = ParseXmlFile();
		return TRUE;
	}
	return FALSE;
}

void LoginDlg::OnBnClickedButtonLogin()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BYTE nf1, nf2, nf3, nf4;
	m_ipAddr.GetAddress(nf1, nf2, nf3, nf4);
	CString m_ipStr;
	m_ipStr.Format(_T("%d.%d.%d.%d"),nf1,nf2,nf3,nf4);

	/*HWND p_hWnd = ::FindWindow(NULL, _T("Transition"));
	CMFCTransitionDlg *pWnd = (CMFCTransitionDlg*)FromHandle(p_hWnd);

	pWnd->m_pClient = new ClientSocket();*/
	if (!m_pSocket->Create())
	{
		AfxMessageBox(_T("�����׽���ʧ��"));
		return;
	}
	if (m_ipStr == _T("0.0.0.0"))
	{

		if (!m_pSocket->Connect(_T("127.0.0.1"), SERVER_PORT))
		{
			AfxMessageBox(_T("���ӷ�����ʧ�ܣ�"));
			m_pSocket->Close();
		}
		else
		{
			theApp.m_strServerIP = m_ipStr;
			theApp.m_connet = 1;
			OnOK();
		}
	}
	else
	{
		if (!m_pSocket->Connect(m_ipStr, SERVER_PORT))
		{
			AfxMessageBox(_T("���ӷ�����ʧ�ܣ�"));
			m_pSocket->Close();
		}
		else
		{
			theApp.m_strServerIP = m_ipStr;
			theApp.m_connet = 1;
			::CMarkup xml;
			int nResult;
			if (CreateLoginXml(xml,nResult))
			{
				switch (nResult)
				{
				case 1:
				case 0:
					break;
				case -3:
					AfxMessageBox(_T("�û�������"));
					return;
				case -4:
					AfxMessageBox(_T("�������"));
					return;
				case -5:
					AfxMessageBox(_T("ƽ̨Key����"));
					return;
				default:
					AfxMessageBox(_T("δ֪����"));
					return;
				}
			}
			theApp.m_strServerIP = m_ipStr;
			theApp.m_connet = 1;
			OnOK();
		}
	}
	
}
