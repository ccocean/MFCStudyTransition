
// MFCTransitionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTransition.h"
#include "MFCTransitionDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char WorkBuffer[512], ReadyBuffer[PROTOCOL_LEN-4];
unsigned char TempBuffer[5];
unsigned int ReadyLength;
unsigned int BufferLength = 0;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCTransitionDlg 对话框



CMFCTransitionDlg::CMFCTransitionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCTransitionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCTransitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM, m_comboCom);
	DDX_Control(pDX, IDC_BUTTON_COM, m_btnCom);
	DDX_Control(pDX, IDC_EDIT_BAUD, m_editBaud);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, m_ipServer);
	DDX_Control(pDX, IDC_EDIT1, m_editLog);
}

BEGIN_MESSAGE_MAP(CMFCTransitionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMFCTransitionDlg::OnBnClickedButtonConnect)
	ON_MESSAGE(WM_USER_TIMEOUT, &CMFCTransitionDlg::Reconnect)
	ON_MESSAGE(WM_APICOMMNOTIFY, &CMFCTransitionDlg::ParseSerialPack)
	ON_MESSAGE(WM_SEND2SERIAL, &CMFCTransitionDlg::Send2SerialPort)
	ON_BN_CLICKED(IDC_BUTTON_COM, &CMFCTransitionDlg::OnBnClickedButtonCom)
END_MESSAGE_MAP()


// CMFCTransitionDlg 消息处理程序

BOOL CMFCTransitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	
	//m_loginDlg.SetSocket(m_pClient);
		//char *pBuff = "Hello!";
		//m_pClient->SendMSG(pBuff, 6);
	
	m_editBaud.SetWindowText(_T("9600"));
	m_comboCom.SetCurSel(0);
	m_spConnect = FALSE;
	m_connet = FALSE;
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCTransitionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCTransitionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

/*
@param
mode:导播模式(2-手动 1-自动 0-默认自动)
*/
BOOL CMFCTransitionDlg::SetManageMode(UINT mode)
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, MNG_MODE_CODE);
	xml.AddChildElem(PASSKEY, SERIALPLAT);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(_T("MPmode"), mode);
	return m_pClient->Packet(MNG_MODE_CODE, xml);
}

/*
@param
recType:录制状态(1-开始 2-暂停 3-停止)
roomName:教室名
teacherName:教师名
courseName:课程名
Notes:备注
*/
BOOL CMFCTransitionDlg::SetRecordStatus(int recType, CString roomName, CString teacherName, CString courseName, CString notes)
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, MNG_SETRCD_CODE);
	xml.AddChildElem(PASSKEY, SERIALPLAT);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(_T("RecType"), recType);
	xml.AddChildElem(_T("RoomName"), roomName);
	xml.AddChildElem(_T("TeacherName"), teacherName);
	xml.AddChildElem(_T("CourseName"), courseName);
	xml.AddChildElem(_T("Notes"), notes);
	return m_pClient->Packet(MNG_SETRCD_CODE, xml);
}

/*
@param
flag:标志(true-设置电影模式 false-获取电影模式)
sourceNum:参与合成画面个数
width:电影模式分辨率
height:
mplayout:电影模式
*/
BOOL CMFCTransitionDlg::SetOrGetMode(BOOL flag, int sourceNum, int width, int height, int mplayout, int FrameRate, int bitRate)
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, MNG_SETMOVIE_CODE);
	xml.AddChildElem(PASSKEY, SERIALPLAT);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(_T("OptType"), (int)flag);
	xml.AddChildElem(_T("SourceNum"), sourceNum);
	xml.AddChildElem(_T("Width"), width);
	xml.AddChildElem(_T("Height"), height);
	xml.AddChildElem(_T("MPlayout"), mplayout);
	xml.AddChildElem(_T("FrameRate"), FrameRate);
	xml.AddChildElem(_T("BitRate"), bitRate);
	return m_pClient->Packet(MNG_SETMOVIE_CODE, xml);
}

BOOL CMFCTransitionDlg::SendHearBeat(int rcdStatus, int rcdTime, int mpMode, int mpLayout)
{
	::CMarkup heart_xml;
	heart_xml.SetDoc(TITLE);
	heart_xml.AddElem(REQUESTMSG);
	heart_xml.IntoElem();
	heart_xml.AddElem(MSGHEAD);
	heart_xml.AddChildElem(MSGCODE, MNG_HEARTBEAT_CODE);
	heart_xml.AddChildElem(PASSKEY, SERIALPLAT);

	return this->m_pClient->Packet(MNG_HEARTBEAT_CODE, heart_xml);
}

LRESULT CMFCTransitionDlg::Reconnect(WPARAM wParam, LPARAM lParam)
{
	m_pClient->Close();
	m_connet = FALSE;
	UpdateLog(_T("服务器断开连接，正在重连..."));
	//delete m_pClient;
	//m_pClient = NULL;
	//m_pClient = new ClientSocket;
	m_pClient->SetParam(this);
	if (!m_pClient->Create())
	{
		AfxMessageBox(_T("创建套接字失败"));
		return FALSE;
	}
	if (!m_pClient->Connect(m_ipStr, SERVER_PORT))
	{
		AfxMessageBox(_T("连接服务器失败！"));
		m_pClient->Close();
		return FALSE;
	}
	else
	{

		::CMarkup xml;
		int nResult;
		if (CreateLoginXml(xml, nResult))
		{
			m_editLog.Clear();
			m_btnConnect.SetWindowText(_T("断开"));
		}
		return TRUE;
	}
	
}

int CMFCTransitionDlg::ParseXmlFile()
{
	::CMarkup xml = m_pClient->m_xml;
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

BOOL CMFCTransitionDlg::CreateLoginXml(::CMarkup& xml, int &nRes)
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
	if (m_pClient->Packet(ALL_LOGIN_CODE, xml))
	{
		//nRes = ParseXmlFile();
		return TRUE;
	}
	return FALSE;
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCTransitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCTransitionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	BOOL nRes;
	switch (nIDEvent)
	{
	case 1:
		//::CMarkup heart_xml;
		m_pClient->SetReconnectTimer(6000);
		nRes = SendHearBeat(0, 2000, 1, 0x211);
		if (nRes)
		{
			UpdateLog(_T("客户端:发送心跳!"));
		}
		else
		{
			UpdateLog(_T("客户端:心跳失败!"));
		}
		break;
	case 2:
		unsigned char val;
		int a = 8, b = 2, c = 2;
		val = (a << 4) + (b << 2) + (c);
		unsigned char *sendBuf = new unsigned char[5];
		memcpy(sendBuf, "@#_#@", 5);
		memcpy(sendBuf + 2, &val, 1);
		WriteComm(sendBuf, PROTOCOL_LEN);
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CMFCTransitionDlg::UpdateLog(CString str)
{
	if (m_editLog.GetLineCount()>100)
	{
		m_editLog.SetSel(0, -1);
		m_editLog.Clear();
	}
	CString string;

	CTime time = CTime::GetCurrentTime();

	// 获取系统当前时间

	str += _T("\r\n");

	// 用于换行显示日志

	string = time.Format(_T("%Y/%m/%d %H:%M:%S--")) + str;
	string += "\r\n";

	// 格式化当前时间

	int lastLine = m_editLog.LineIndex(m_editLog.GetLineCount() - 1);

	//获取编辑框最后一行索引

	m_editLog.SetSel(lastLine + 1, lastLine + 2, 0);

	//选择编辑框最后一行

	m_editLog.ReplaceSel(string);                                                             //替换所选那一行的内容
}

void CMFCTransitionDlg::OnBnClickedButtonConnect()
{
	// TODO:  在此添加控件通知处理程序代码
	BYTE nf1, nf2, nf3, nf4;
	m_ipServer.GetAddress(nf1, nf2, nf3, nf4);
	m_ipStr.Format(_T("%d.%d.%d.%d"), nf1, nf2, nf3, nf4);

	if (m_connet != TRUE)
	{
		m_pClient = new ClientSocket();
		m_pClient->SetParam(this);
		if (!m_pClient->Create())
		{
			AfxMessageBox(_T("创建套接字失败"));
			return;
		}
		if (!m_pClient->Connect(m_ipStr, SERVER_PORT))
		{
			AfxMessageBox(_T("连接服务器失败！"));
			m_pClient->Close();
		}
		else
		{

			::CMarkup xml;
			int nResult;
			if (CreateLoginXml(xml, nResult))
			{
				m_editLog.Clear();
				/*switch (nResult)
				{
				case 1:
				SetTimer(1, 3000, NULL);
				theApp.m_strServerIP = m_ipStr;
				theApp.m_connet = 1;
				UpdateLog(_T("Login Success!"));
				break;
				case 0:
				break;
				case -3:
				AfxMessageBox(_T("用户名错误"));
				return;
				case -4:
				AfxMessageBox(_T("密码错误"));
				return;
				case -5:
				AfxMessageBox(_T("平台Key错误"));
				return;
				default:
				AfxMessageBox(_T("未知错误"));
				return;
				}*/
				m_btnConnect.SetWindowText(_T("断开"));
			}
		}
	}
	else
	{
		m_connet = FALSE;
		m_pClient->Close();
		delete m_pClient;
		m_pClient = NULL;
		m_btnConnect.SetWindowText(_T("连接"));
		UpdateLog(_T("Disconnected!!!"));
		KillTimer(1);
	}

}

//一系列串口的函数
void CMFCTransitionDlg::OnBnClickedButtonCom()
{
	// TODO:  在此添加控件通知处理程序代码
	CString port;
	/*if (!m_connet)
	{
		AfxMessageBox(_T("服务器未连接，请先连接服务器！"));
		return;
	}*/
	int id = m_comboCom.GetCurSel();

	if (!m_spConnect)
	{
		m_comboCom.GetLBText(id, port);
		if (OpenCom(port))
		{
			//SetTimer(2, 3000, NULL);
			UpdateLog(_T("串口已打开！"));
			m_comboCom.EnableWindow(FALSE);
			m_btnCom.SetWindowText(_T("断开串口"));
		}
	}
	else
	{
		CloseCom();
		m_btnCom.SetWindowText(_T("打开串口"));
		m_spConnect = FALSE;
		m_comboCom.EnableWindow(TRUE);
		UpdateLog(_T("串口已关闭！"));
	}
}

LRESULT CMFCTransitionDlg::Send2SerialPort(WPARAM wParam, LPARAM lParam)
{
	m_d = up_d; m_e = up_e; m_f = up_f;//更新本地状态
	dn_d = m_d; dn_e = m_e; dn_f = m_f;//更新下机位状态
	unsigned char val;
	val = (m_d << 4) + (m_e << 2) + (m_f);
	unsigned char *sendBuf = new unsigned char[5];
	memcpy(sendBuf, "@#_#@", 5);
	memcpy(sendBuf + 2, &val, 1);
	WriteComm(sendBuf, PROTOCOL_LEN);
	if (isSet==E)
	{
		UpdateLog(_T("服务器:录制状态更新！"));
	}
	else if (isSet==F)
	{
		UpdateLog(_T("服务器:导播状态更新！"));
	}
	else if (isSet == D)
	{
		UpdateLog(_T("服务器:电影模式更新！"));
	}
	else if (isSet == -1)
	{
		UpdateLog(_T("服务器:启动同步中..."));
	}
	else
	{
		UpdateLog(_T("服务器:无更新"));
	}
	isSet = FALSE;
	return TRUE;
}

LRESULT CMFCTransitionDlg::ParseSerialPack(WPARAM wParam, LPARAM lParam)
{
	if (!m_spConnect)
	{
		//UpdateLog()
		return FALSE;
	}
	if (!m_connet)
	{
		UpdateLog(_T("网络未连接！"));
		return FALSE;
	}
	CString temp;
	if (ReadyBuffer[0]==E)
	{
		ReadyBuffer[0] += 55;
		ReadyBuffer[1] += 48;
		int rcdType = atoi(&(ReadyBuffer[1]));
		dn_e = rcdType;
		if (CheckUpDown(up_e, dn_e))
		{
			UpdateLog(_T("重复设置！"));
			return FALSE;
		}
		SetRecordStatus(rcdType, _T(""), _T(""), _T(""), _T(""));
		temp = ReadyBuffer;
		temp += "串口：<--更新录制模式-->";
		isSet = E;
		UpdateLog(temp);
		return TRUE;
	}
	if (ReadyBuffer[0] == F)
	{
		ReadyBuffer[0] += 55;
		ReadyBuffer[1] += 48;
		int mngType = atoi(&(ReadyBuffer[1]));
		dn_f = mngType;
		if (CheckUpDown(up_f, dn_f))
		{
			UpdateLog(_T("重复设置！"));
			return FALSE;
		}
		SetManageMode(mngType);
		temp = ReadyBuffer;
		temp += "串口：<--更新导播模式-->";
		isSet = F;
		UpdateLog(temp);
		return TRUE;
	}
	if (ReadyBuffer[0] == D)
	{
		ReadyBuffer[0] += 55;
		ReadyBuffer[1] += 48;
		int mode = atoi(&(ReadyBuffer[1]));
		dn_d = mode;
		if (CheckUpDown(up_d, dn_d))
		{
			UpdateLog(_T("重复设置！"));
			return FALSE;
		}
		//if (up_f==1)//1为自动模式
		//	SetManageMode(2);//2位手动模式
		SetOrGetMode(TRUE, 0, 0, 0, mode, 0, 0);
		temp = ReadyBuffer;
		temp += "串口：<--更新电影模式-->";
		isSet = D;
		UpdateLog(temp);
		return TRUE;
	}
	return FALSE;
}

void CMFCTransitionDlg::OnAPICommNotify(void)
{
	CString str;
	unsigned char buf[64];
	DWORD length = 0;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	//unsigned int i, j;

	if (!m_hCom)
	{
		AfxMessageBox(_T("串口句柄错误！"));
		return;
	}

	ClearCommError(m_hCom, &dwErrorFlags, &ComStat);
	length = min(ComStat.cbInQue, 64);

	memset(&m_osRead, 0, sizeof(m_osRead));
	m_osRead.hEvent = CreateEvent(NULL, NULL, NULL, NULL);
	ReadFile(m_hCom, buf, length, &length, &m_osRead);
	::CloseHandle(m_osRead.hEvent);

	for (int i = 0; i < length; i++)
	{
		WorkBuffer[BufferLength + i] = buf[i];
	}
	BufferLength += length;
	
	UINT y;
	while (1)
	{
		for (int i = 0; i < BufferLength; i++)
		{
			if (WorkBuffer[i] == '@')
			{
				for (unsigned int j = 0; j <= BufferLength - i; j++)
					WorkBuffer[j] = WorkBuffer[j + i];
				BufferLength = BufferLength - i;
				break;
			}
		}
		if (BufferLength<PROTOCOL_LEN)
			break;
		if ((WorkBuffer[1] == '#') && (WorkBuffer[3] == '#') && (WorkBuffer[4] == '@'))
		{
			//在这里处理WorkBuffer[2],WorkBuffer[3]
			char ch1, ch2;
			ch1 = (WorkBuffer[2] & 0xf0) >> 4;
			ch2 = WorkBuffer[2] & 0x0f;
			if (ch1 == D)
			{
				if (0 <= ch2 &&ch2 <= 9)
				{
					/*for (int i = 0; i < PROTOCOL_LEN-4; i++)
					{
						ReadyBuffer[i] = WorkBuffer[i + 2];
					}*/
					ReadyBuffer[0] = ch1;
					ReadyBuffer[1] = ch2;
					PostMessage(WM_APICOMMNOTIFY, 0, 0);
				}
			}
			if (ch1 == E || ch1 == F)
			{
				if (0 <= ch2 && ch2 <= 2)
				{
					/*for (int i = 0; i < PROTOCOL_LEN-4; i++)
					{
					ReadyBuffer[i] = WorkBuffer[i + 2];
					}*/
					ReadyBuffer[0] = ch1;
					ReadyBuffer[1] = ch2;
					PostMessage(WM_APICOMMNOTIFY, 0, 0);
				}
			}
			/*调整工作缓冲区*/
			y = PROTOCOL_LEN;		//满足包协议，无论出错与否，去掉6个字节
		}
		else
		{
			y = 1;		//不满足包协议，去掉首字符@
		}
		for (int i = 0; i < BufferLength - y; i++)
		{
			WorkBuffer[i] = WorkBuffer[i + y];
		}
		BufferLength -= y;
	}
}

UINT CMFCTransitionDlg::CommProc(LPVOID pParam)
{
	OVERLAPPED os;

	DWORD dwMask, dwTrans;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	
	

	CMFCTransitionDlg *pDlg = (CMFCTransitionDlg*)pParam;

	if (!pDlg->m_hCom)
	{
		AfxMessageBox(_T("串口句柄错误！"));
		return -1;
	}

	memset(&os, 0, sizeof(OVERLAPPED));
	os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (os.hEvent==NULL)
	{
		AfxMessageBox(_T("Can't create event object!"));
		return (UINT)-1;
	}
	while (pDlg->m_spConnect)
	{
		ClearCommError(pDlg->m_hCom, &dwErrorFlags, &ComStat);
		if (ComStat.cbInQue)
		{
			pDlg->OnAPICommNotify();
		}
		dwMask = 0;
		if (!WaitCommEvent(pDlg->m_hCom, &dwMask, &os))
		{
			if (GetLastError()==ERROR_IO_PENDING)
			{
				GetOverlappedResult(pDlg->m_hCom, &os, &dwTrans, TRUE);
			}
			else
			{
				CloseHandle(os.hEvent);
				return (UINT)-1;
			}
		}
	}
	CloseHandle(os.hEvent);
	return 0;
}

BOOL CMFCTransitionDlg::OpenCom(CString port)
{
	COMMTIMEOUTS TimeOuts;

	if (m_spConnect)
	{
		AfxMessageBox(_T("串口已打开"));
		return FALSE;
	}

	m_hCom = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);//重叠方式

	if (m_hCom==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("串口不存在或被占用！"));
		return FALSE;
	}
	SetupComm(m_hCom, MAXBUFFER, MAXBUFFER);
	SetCommMask(m_hCom, EV_RXCHAR);

	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;

	TimeOuts.WriteTotalTimeoutMultiplier = 50;
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(m_hCom, &TimeOuts);

	if (ConfigConnection())
	{
		m_pThread = AfxBeginThread(CommProc, this, THREAD_PRIORITY_HIGHEST,0,0,NULL);
		if (m_pThread==NULL)
		{
			CloseHandle(m_hCom);
			AfxMessageBox(_T("创建线程错误！"));
			return FALSE;
		}
		else
		{
			PurgeComm(m_hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
			m_spConnect = TRUE;
			m_pThread->ResumeThread();
		}
	}
	else
	{
		m_spConnect = FALSE;
		CloseHandle(m_hCom);
		AfxMessageBox(_T("配置文件出错！"));
		return FALSE;
	}
	return TRUE;
}

long CMFCTransitionDlg::WriteComm(unsigned char *pBuf, long dwlength)
{
	BOOL fState;
	DWORD length = dwlength;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	memset(&m_osWrite, 0, sizeof(m_osWrite));
	ClearCommError(m_hCom,&dwErrorFlags,&ComStat);
	fState = WriteFile(m_hCom, pBuf, length, &length, &m_osWrite);
	if (!fState)
	{
		if (GetLastError()==ERROR_IO_PENDING)
		{
			GetOverlappedResult(m_hCom, &m_osWrite, &length, TRUE);//等待
		}
		else
		{
			length = 0;
		}
	}
	return length;
}

BOOL CMFCTransitionDlg::CloseCom()
{
	if (!m_spConnect)return TRUE;
	m_spConnect = FALSE;

	SetEvent(m_hPostMsgEvent);
	SetCommMask(m_hCom, 0);
	WaitForSingleObject(m_pThread->m_hThread, INFINITE);
	m_pThread = NULL;

	CloseHandle(m_hCom);
	return TRUE;
}

BOOL CMFCTransitionDlg::ConfigConnection()
{
	DCB dcb;
	if (!GetCommState(m_hCom, &dcb))
		return FALSE;

	dcb.fBinary = TRUE;
	dcb.BaudRate = BAUDRATE;
	dcb.ByteSize = 8;
	dcb.fParity = NOPARITY;
	dcb.StopBits = 0;
	
	return SetCommState(m_hCom, &dcb);
}
