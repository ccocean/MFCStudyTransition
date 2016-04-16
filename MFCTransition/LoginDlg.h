#pragma once
#include "afxcmn.h"
#include "ClientSocket.h"

// LoginDlg 对话框

class LoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LoginDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	ClientSocket *m_pSocket;

	BOOL CreateLoginXml(::CMarkup& xml, int &nRes);
	int ParseXmlFile();
	afx_msg void OnBnClickedButtonLogin();
	void SetSocket(ClientSocket * pSocket);
	CIPAddressCtrl m_ipAddr;
};
