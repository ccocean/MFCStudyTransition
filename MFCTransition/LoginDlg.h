#pragma once
#include "afxcmn.h"
#include "ClientSocket.h"

// LoginDlg �Ի���

class LoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LoginDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	ClientSocket *m_pSocket;

	BOOL CreateLoginXml(::CMarkup& xml, int &nRes);
	int ParseXmlFile();
	afx_msg void OnBnClickedButtonLogin();
	void SetSocket(ClientSocket * pSocket);
	CIPAddressCtrl m_ipAddr;
};
