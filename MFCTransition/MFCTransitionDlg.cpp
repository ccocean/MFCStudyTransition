
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
#define WIDTH 291
#define HEIGHT 298

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
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, m_ipServer);
	//DDX_Control(pDX, IDC_EDIT1, m_editLog);
	DDX_Control(pDX, IDC_LIST1, m_listLog);
	DDX_Control(pDX, IDC_BUTTON1, m_btnDebug);
	DDX_Control(pDX, IDC_CHECK_AUTO, m_checkAuto);
	DDX_Control(pDX, IDC_EDIT_IP, m_editIp);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_IDC_CBSTATIC, m_combobox);
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
	ON_MESSAGE(WM_USER_NOTIFYICON, &CMFCTransitionDlg::OnNotifyMsg)
	ON_BN_CLICKED(IDC_BUTTON_COM, &CMFCTransitionDlg::OnBnClickedButtonCom)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCTransitionDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK_AUTO, &CMFCTransitionDlg::OnBnClickedCheckAuto)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ENTER, &CMFCTransitionDlg::OnBnClickedButtonEnter)
	ON_BN_CLICKED(IDC_BUTTON_MIN, &CMFCTransitionDlg::OnBnClickedButtonMin)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CMFCTransitionDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &CMFCTransitionDlg::OnBnClickedButtonAuto)
	ON_STN_CLICKED(IDC_STATIC_AUTO, &CMFCTransitionDlg::OnStnClickedStaticAuto)
	ON_WM_LBUTTONDOWN()
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
	m_filePath = GetFilePath();
	/*CFont* pFont = new CFont;
	pFont->CreatePointFont(20 * 10, _T("微软雅黑"));
	GetDlgItem(IDC_EDIT_BAUD)->SetFont(pFont);*/
	//GetDlgItem(IDC_STATIC_BAUD2)->SetWindowText(_T("9600"));
	GetDlgItem(IDC_IDC_CBSTATIC)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_BAUD)->SetWindowText(_T("9600"));
	m_comboCom.SetCurSel(0);
	m_spConnect = FALSE;
	m_connet = FALSE;
	SetWindowPos(NULL, 0, 0, WIDTH, HEIGHT, SWP_NOMOVE);
	GetWindowRect(wndRect);
	m_btnDebug.ShowWindow(isShowBtn);
	//LoadIMG();

	FindCommPort();
	CStdioFile myFile;
	int cnt = 0;
	int len;
	CString temp;
	BOOL openResult = myFile.Open(m_filePath+_T("connectInfo.txt"), CFile::modeRead);
	if (!openResult)
	{
		openResult = myFile.Open(m_filePath+_T("connectInfo.txt"), CFile::modeCreate);
		if (!openResult)
		{
			MessageBox(_T("打开文件错误！"));
		}
	}
	else
	{
		while (myFile.ReadString(temp))
		{
			len = temp.GetLength();
			if (len<=0)
			{
				UpdateLog(_T("读取文件错误！"));
				break;
			}
			temp.Delete(len - 1, 1);
			if (cnt == 0)
			{
				m_editIp.SetWindowText(temp);
			}
			if (cnt == 1)
			{
				m_combobox.SetCurSel(_ttoi(temp));
			}
			if (cnt == 2)
			{
				m_isAuto = _ttoi(temp);
			}
			cnt++;
		}
	}
	myFile.Close();
	
	m_bmpB_Close.SubclassDlgItem(IDC_BUTTON_CLOSE, this);
	m_bmpB_Close.LoadBitmaps(IDB_BITMAP_CLOSE_NORMAL, IDB_BITMAP_CLOSE_WORK, 0, 0);
	m_bmpB_Close.SizeToContent();

	m_bmpB_Min.SubclassDlgItem(IDC_BUTTON_MIN, this);
	m_bmpB_Min.LoadBitmaps(IDB_BITMAP_MIN_NORMAL, IDB_BITMAP_MIN_WORK, 0, 0);
	m_bmpB_Min.SizeToContent();

	m_bmpB_Enter.SubclassDlgItem(IDC_BUTTON_ENTER, this);
	m_bmpB_Enter.LoadBitmaps(IDB_BITMAP_ENTER_NORMAL, IDB_BITMAP_ENTER_WORK, 0, 0);
	m_bmpB_Enter.SizeToContent();

	m_bmpB_Auto.SubclassDlgItem(IDC_BUTTON_AUTO, this);
	if (m_isAuto)
	{
		m_bmpB_Auto.LoadBitmaps(IDB_BITMAP_AUTO_WORK, 0, 0, 0);
		m_bmpB_Auto.SizeToContent();
	}
	else
	{
		m_bmpB_Auto.LoadBitmaps(IDB_BITMAP_AUTO_NORMAL, 0, 0, 0);
		m_bmpB_Auto.SizeToContent();
	}
	
	m_redcolor = RGB(255, 0, 0);                      // 红色  
	m_bgcolor = RGB(23, 23, 23);                     // 蓝色  
	m_textcolor = RGB(255, 255, 255);                 // 文本颜色设置为白色  
	m_redbrush.CreateSolidBrush(m_redcolor);      // 红色背景色  
	m_bluebrush.CreateSolidBrush(m_bgcolor);    // 蓝色背景色

	if (!m_connet)
	{
		OnBnClickedButtonEnter();
	}

	/*m_combobox.SetWindowSize(3);
	m_combobox.AddString(_T("你好"));
	m_combobox.AddString(_T("我很好"));
	m_combobox.AddString(_T("Visual Studio"));
	m_combobox.AddString(_T("mycombobox"));
	m_combobox.AddString(_T("mycombobox"));
	m_combobox.AddString(_T("mycombobox"));
	m_combobox.AddString(_T("mycombobox"));
	m_combobox.AddString(_T("mycombobox"));
	m_combobox.InsertString(1, _T("235465"));*/
	//HBITMAP   hBitmap;
	//hBitmap = LoadBitmap(AfxGetInstanceHandle(),
	//	MAKEINTRESOURCE(IDB_BITMAP2)); // IDB_BITMAP_TEST为资源图片ID 
	//((CButton *)GetDlgItem(IDC_BUTTON_CLOSE))->SetBitmap(hBitmap);

	//SetWindowLong(m_hWnd, GWL_HWNDPARENT, NULL);
	////CRgn m_rgn;
	//RECT rc;
	//GetWindowRect(&rc);
	//m_rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 11, 11);
	//if (!m_isDbg)
	//{
	//	SetWindowRgn(m_rgn, TRUE);//改成圆角对话框
	//}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCTransitionDlg::FindCommPort()
{
	CString str;
	CString portName[16];
	HKEY hKey;
	//int cnt = 0;
	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("Hardware\\DeviceMap\\SerialComm"),
		NULL,
		KEY_READ,
		&hKey)==ERROR_SUCCESS)
	{
		int i = 0;
		char portName[25], commName[25];
		DWORD dwLong, dwSize;
		while (1)
		{
			dwLong = dwSize = sizeof(portName);
			if (::RegEnumValueA(hKey,
				i,
				portName,
				&dwLong,
				NULL,
				NULL,
				(PUCHAR)commName,
				&dwSize) == ERROR_NO_MORE_ITEMS) // 枚举串口
				break;
			str.Format(_T("%S"), commName);
			m_combobox.AddString(str); // commName就是串口名字
			//cnt++;
			i++;
		}
		//m_combobox.SetWindowSize(m_combobox.GetCount());
		/*if (m_combobox.GetCount() == 0)
		{
		AfxMessageBox(_T("无法在系统中找到有效串口！"));
		}*/
		if (m_combobox.GetCount() == 0)
		{
			//SetTimer(100, 1000, NULL);
			AfxMessageBox(_T("无法在系统中找到有效串口！"));
		}
		//else
		//{
		//	i = 0;
		//	m_combobox.SetWindowSize(cnt);
		//	while (1)
		//	{
		//		dwLong = dwSize = sizeof(portName);
		//		if (::RegEnumValueA(hKey,
		//			i,
		//			portName,
		//			&dwLong,
		//			NULL,
		//			NULL,
		//			(PUCHAR)commName,
		//			&dwSize) == ERROR_NO_MORE_ITEMS) // 枚举串口
		//			break;
		//		str.Format(_T("%S"), commName);
		//		m_combobox.AddString(str); // commName就是串口名字
		//		//cnt++;
		//		i++;
		//	}
		//}
		RegCloseKey(hKey);
	}

	/*HANDLE tmpCom;
	CString str;
	for (int i = 0; i < 16;i++)
	{
		str.Format()
	}*/
}

void CMFCTransitionDlg::LoadIMG()
{
	//CStatic*  pPic;
	//CRect rc;
	//CImage img;
	//CString temp = m_filePath;
	//temp += "images\\背景.png";
	//HRESULT ret = img.Load(temp); 
	////img.Draw()
	////HBITMAP hBitmap;
	///*CBitmap bitmap;
	//bitmap.Attach(img.Detach());*/
	////hBitmap = (HBITMAP)bitmap.GetSafeHandle();
	////GetDlgItem(IDC_STATIC_MAIN)->SetWindowPos(NULL, 10, 10, 20, 20, SWP_NOMOVE);
	////hBitmap_main = (HBITMAP)LoadImage(AfxGetInstanceHandle(),temp, IMAGE_BITMAP, wndRect.Width(), wndRect.Height(), LR_LOADFROMFILE);
	//pPic = (CStatic*)GetDlgItem(IDC_STATIC_MAIN);
	////HBITMAP hBmp = img.Detach();
	////pPic->SetBitmap(hBitmap_main);
	//
	//ASSERT(pPic);
	//pPic->SetBitmap((HBITMAP)img);

	/*CImage img;
	CDC *pDC;
	CRect rect;
	CWnd *pWnd;
	img.Load(m_filePath+_T("\\images\\背景.png"));
	CPaintDC dc(this);
	if (!img.IsNull()) img.Destroy();
	if (!img.IsNull()) img.Draw(dc.m_hDC, 0, 0); */

	//GetDlgItem(IDC_STATIC_MAIN)->SetWindowPos(NULL, 0, 0, wndRect.right, wndRect.bottom, SWP_NOMOVE);
	/*pWnd = GetDlgItem(IDC_STATIC_MAIN);
	pDC = pWnd->GetDC();
	pWnd->GetClientRect(&rect);
	pDC->SetStretchBltMode(STRETCH_HALFTONE);
	img.Draw(pDC->m_hDC, rect);
	ReleaseDC(pDC);
	img.Destroy();*/

	//img.Load(m_filePath + _T("\\images\\关闭.bmp"));
	////CRect rect;
	//GetDlgItem(IDC_STATIC_CLOSE)->SetWindowPos(NULL, wndRect.right-28, 0, 26, 26, SWP_NOMOVE);
	//pWnd = GetDlgItem(IDC_STATIC_CLOSE);
	//pDC = pWnd->GetDC();
	//pWnd->GetClientRect(&rect);
	//pDC->SetStretchBltMode(STRETCH_HALFTONE);
	//img.Draw(pDC->m_hDC, rect);
	//ReleaseDC(pDC);
	//img.Destroy();

	
	//CDialogEx::OnPaint();
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
		CImage img;
		CPaintDC dc(this);
		if (!img.IsNull()) img.Destroy();
		//img.Load(m_filePath + _T("\\images\\背景.png"));
		img.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_BG);
		if (!img.IsNull()) img.Draw(dc.m_hDC, 0, 0);
		

		CDialogEx::OnPaint();
	}
}

CString CMFCTransitionDlg::GetFilePath()
{
	char  exepath[MAX_PATH];
	CString  strdir, tmpdir;
	memset(exepath, 0, MAX_PATH);
	GetModuleFileNameA(NULL, exepath, MAX_PATH);
	tmpdir = exepath;
	strdir = tmpdir.Left(tmpdir.ReverseFind('\\'));
	return strdir+'\\';
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
	CloseCom();
	UpdateLog(_T("服务器断开连接，正在重连..."));
	m_bmpB_Enter.LoadBitmaps(IDB_BITMAP_ENTER_NORMAL, IDB_BITMAP_ENTER_WORK, 0, 0);
	m_bmpB_Enter.SizeToContent();
	//delete m_pClient;
	//m_pClient = NULL;
	//m_pClient = new ClientSocket;
	//m_pClient->SetParam(this);
	//if (!m_pClient->Create())
	//{
	//	UpdateLog(_T("创建套接字失败"));
	//	return FALSE;
	//}
	//if (!m_pClient->Connect(m_ipStr, SERVER_PORT))
	//{
	//	UpdateLog(_T("连接服务器失败！"));
	//	m_pClient->Close();
	//	return FALSE;
	//}
	//else
	//{
	//	//m_pClient->SetParam(this);
	//	//::CMarkup xml;
	//	//int nResult;
	//	//if (CreateLoginXml(xml, nResult))
	//	//{
	//	//	//SetTimer(1, 3000, NULL);
	//	//	m_listLog.ResetContent();
	//	//	m_btnConnect.SetWindowText(_T("断开"));
	//	//}
	//	//OnBnClickedButtonConnect();
	//	OnBnClickedButtonEnter();
	//	return TRUE;
	//}
	OnBnClickedButtonEnter();
	return TRUE;
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
	int a = 8, b = 2, c = 2;
	unsigned char *sendBuf = new unsigned char[PROTOCOL_LEN];
	switch (nIDEvent)
	{
	case 1:
		//::CMarkup heart_xml;
		//nRes = SendHearBeat(0, 2000, 1, 0x211);
		if (m_isAlive)
		{
			nRes = SendHearBeat(0, 0, 0, 0);
			if (nRes)
			{
				UpdateLog(_T("客户端:发送心跳!"));
				m_isAlive = FALSE;
			}
			else
			{
				//UpdateLog(_T("客户端:心跳失败!"));

				//KillTimer(1);
				UpdateLog(_T("心跳失败，正在重连服务器..."));
				m_isAlive = FALSE;
				::PostMessage(GetSafeHwnd(), WM_USER_TIMEOUT, 0, 0);
				//m_overTime = 0;
			}
		}
		else
		{
			::PostMessage(GetSafeHwnd(), WM_USER_TIMEOUT, 0, 0);
		}
		break;
	case 2:
		unsigned char val;
		val = (a << 4) + (b << 2) + (c);
		memcpy(sendBuf, "@#_#@", PROTOCOL_LEN);
		memcpy(sendBuf + 2, &val, 1);
		WriteComm(sendBuf, PROTOCOL_LEN);
		break;
	case 33:
		SetTimer(100, 1000, NULL);
		AfxMessageBox(_T("连接超时"));
		KillTimer(nIDEvent);
		break;
	case 100:
		KillTimer(nIDEvent);
		keybd_event(VK_RETURN, 0, 0, 0);
		keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);//模拟"回车"命令
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CMFCTransitionDlg::UpdateLog(CString str)
{
	if (m_listLog.GetCount()>400)
	{
		m_listLog.ResetContent();
	}
	CString string;

	CTime time = CTime::GetCurrentTime();

	// 获取系统当前时间

	str += _T("\r\n");

	// 用于换行显示日志

	string = time.Format(_T("%Y/%m/%d %H:%M:%S--")) + str;
	string += "\r\n";

	// 格式化当前时间

	int lastLine = m_listLog.GetCount();
	m_listLog.InsertString(lastLine, string);
	m_listLog.SetCurSel(lastLine);
	//获取编辑框最后一行索引

	//m_editLog.SetSel(lastLine + 1, lastLine + 2, 0);

	//选择编辑框最后一行

	//m_editLog.ReplaceSel(string);                                                             //替换所选那一行的内容

}

BOOL CMFCTransitionDlg::PreTranslateMessage(MSG* pMsg)
{
	/*CRect rect;
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_SERVER))->GetClientRect(&rect);
	InvalidateRect(rect, TRUE);*/
	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)
	{
		//pMsg->wParam = VK_RETURN;   //将ESC键的消息替换为回车键的消息，这样，按ESC的时候  
		return TRUE;
		//也会去调用OnOK函数，而OnOK什么也不做，这样ESC也被屏蔽  
	}
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
	{
		if (m_connet)
		{
			SetTimer(100, 1000, NULL);
			AfxMessageBox(_T("桌面控制器已启动！"));
			return TRUE;
		}
		OnBnClickedButtonEnter();
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetKeyState(VK_CONTROL) < 0) // 或者: if (GetAsyncKeyState(VK_CONTROL) & 0x8000)  
		{
			if (pMsg->wParam == 'D')
			{
				//AfxMessageBox(_T("ctrl + d"));
				if (!isShowBtn)
				{
					isShowBtn = TRUE;
					m_btnDebug.ShowWindow(isShowBtn);
					m_btnDebug.SetWindowText(_T(">"));
					//SetWindowPos(NULL, 0, 0, wndRect.right, wndRect.bottom, SWP_NOMOVE);
				}
				else
				{
					if (!isDebug)
					{
						isShowBtn = FALSE;
						m_btnDebug.ShowWindow(isShowBtn);
					}
					//m_btnDebug.SetWindowText(_T("↓"));
					//SetWindowPos(NULL, 0, 0, wndRect.right, wndRect.bottom - 260, SWP_NOMOVE);
				}
			}
			if (pMsg->wParam=='Y')
			{
				m_bmpB_Enter.EnableWindow(TRUE);
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMFCTransitionDlg::OnBnClickedButtonConnect()
{
	// TODO:  在此添加控件通知处理程序代码
	//BYTE nf1, nf2, nf3, nf4;
	//m_ipServer.GetAddress(nf1, nf2, nf3, nf4);
	//m_ipStr.Format(_T("%d.%d.%d.%d"), nf1, nf2, nf3, nf4);
	m_editIp.GetWindowText(m_ipStr);

	if (m_connet != TRUE)
	{
		m_pClient = new ClientSocket();
		m_pClient->SetParam(this);
		//SetTimer(OVERTIME, 3000, NULL);
		if (!m_pClient->Create())
		{
			SetTimer(100, 1000, NULL);
			AfxMessageBox(_T("创建套接字失败"));
			return;
		}
		if (!m_pClient->Connect(m_ipStr, SERVER_PORT))
		{
			SetTimer(100, 1000, NULL);
			AfxMessageBox(_T("连接服务器失败！"));
			m_pClient->Close();
		}

		else
		{

			::CMarkup xml;
			int nResult;
			if (CreateLoginXml(xml, nResult))
			{
				m_connet = TRUE;
				//m_editLog.Clear();
				//m_strServerIP = m_ipStr;
				m_listLog.ResetContent();
				//SetTimer(1, 3000, NULL);
				/*switch (nResult)
				{
				case 1:
				
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
				OnBnClickedButtonCom();
			}
		}
	}
	else
	{
		m_connet = FALSE;
		m_pClient->Close();
		//delete m_pClient;
		//m_pClient = NULL;
		m_btnConnect.SetWindowText(_T("连接"));
		UpdateLog(_T("Disconnected!!!"));
		KillTimer(1);
		//m_isTimerSet = FALSE;
	}
}

//一系列串口的函数
void CMFCTransitionDlg::OnBnClickedButtonCom()
{
	// TODO:  在此添加控件通知处理程序代码
	CString port;
	if (!m_connet)
	{
		SetTimer(100, 1000, NULL);
		AfxMessageBox(_T("服务器未连接，请先连接服务器！"));
		return;
	}
	int id = m_combobox.GetCurSel();

	if (!m_spConnect)
	{
		m_combobox.GetLBText(id, port);
		if (OpenCom(port))
		{
			//SetTimer(2, 3000, NULL);
			UpdateLog(_T("串口已打开！"));
			
			m_btnCom.SetWindowText(_T("断开串口"));

			CStdioFile myFile;
			CString temp;
			//isAuto = m_checkAuto.GetCheck();
			temp.Format(_T("%d"),id);
			BOOL openResult = myFile.Open(m_filePath+_T("connectInfo.txt"), CFile::modeCreate | CFile::modeReadWrite);
			if (!openResult)
			{
				UpdateLog(_T("打开文件错误，写入信息失败！"));
			}
			else
			{
				//m_btnEnter.EnableWindow(FALSE);
				//HideWindow();
				//m_bmpB_Enter.EnableWindow(FALSE);
				m_combobox.EnableWindow(FALSE);
				myFile.SeekToBegin();
				myFile.WriteString(m_ipStr);
				myFile.Write(("\r\n"), 2);
				myFile.WriteString(temp);
				myFile.Write(("\r\n"), 2);
				temp.Format(_T("%d"), m_isAuto);
				myFile.WriteString(temp);
				myFile.Write(("\r\n"), 2);
				myFile.Flush();
				myFile.Close();
			}
		}
	}
	else
	{
		m_listLog.ResetContent();
		CloseCom();
		m_btnCom.SetWindowText(_T("打开串口"));
		m_spConnect = FALSE;
		m_combobox.EnableWindow(TRUE);
		UpdateLog(_T("串口已关闭！"));
	}
}

LRESULT CMFCTransitionDlg::OnNotifyMsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
		return    1;
	switch (lParam)
	{
	case WM_RBUTTONUP:
	{
		LPPOINT    lpoint = new    tagPOINT;
		::GetCursorPos(lpoint);//得到鼠标位置   
		CMenu    menu;
		menu.CreatePopupMenu();//声明一个弹出式菜单   
		//增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已   
		//隐藏），将程序结束。   
		menu.AppendMenu(MF_STRING, WM_DESTROY, _T("关闭"));
		//确定弹出式菜单的位置   
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		//资源回收   
		HMENU    hmenu = menu.Detach();
		menu.DestroyMenu();
		Shell_NotifyIcon(NIM_DELETE, &m_tnid);
		delete    lpoint;
	}
		break;
	case    WM_LBUTTONDBLCLK://双击左键的处理   
	{
		this->ShowWindow(SW_SHOW);//简单的显示主窗口完事儿   
	}
		break;
	default:
		break;
	}
	return TRUE;
}

void CMFCTransitionDlg::HideWindow()
{
	if (isDebug)
	{
		return;
	}
	m_tnid.cbSize = sizeof(NOTIFYICONDATA);
	m_tnid.hWnd = this->m_hWnd;
	m_tnid.uID = IDR_MAINFRAME;
	m_tnid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	//strcpy(m_tnid.szTip, "Michael_Chen is a good man");
	m_tnid.uCallbackMessage = WM_USER_NOTIFYICON;
	m_tnid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; //OK,下面就是托盘产生了. 
	Shell_NotifyIcon(NIM_ADD, &m_tnid);
	//SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW);
	ShowWindow(SW_HIDE);
}

LRESULT CMFCTransitionDlg::Send2SerialPort(WPARAM wParam, LPARAM lParam)
{
	if (m_d != up_d&&m_d > 0)
		isSet = D;
	if (m_e != up_e&&m_e > 0)
		isSet = E;
	if (m_f != up_f&&m_f > 0)
		isSet = F;
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
		isSet = -1;
	}
	else if (isSet==F)
	{
		UpdateLog(_T("服务器:导播状态更新！"));
		isSet = -1;
	}
	else if (isSet == D)
	{
		UpdateLog(_T("服务器:电影模式更新！"));
		isSet = -1;
	}
	else if (isSet == -2)
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
		
		temp = "串口:<--更新录制模式";
		temp += ReadyBuffer;
		temp += "-->";
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
		
		temp = "串口:<--更新导播模式";
		temp += ReadyBuffer;
		temp += "-->";
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
		if (up_f==1)//1为自动模式
			SetManageMode(2);//2位手动模式
		SetOrGetMode(TRUE, 0, 0, 0, mode, 0, 0);
		
		temp = "串口:<--更新电影模式";
		temp += ReadyBuffer;
		temp += "-->";
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
		SetTimer(100, 1000, NULL);
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
				if (0 <= ch2 &&ch2 <= 8)
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
				if (0 <= ch2 && ch2 <= 3)
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
	
	//int temp = 0;

	CMFCTransitionDlg *pDlg = (CMFCTransitionDlg*)pParam;

	if (!pDlg->m_hCom)
	{
		pDlg->SetTimer(100, 1000, NULL);
		AfxMessageBox(_T("串口句柄错误！"));
		return -1;
	}

	memset(&os, 0, sizeof(OVERLAPPED));
	os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (os.hEvent==NULL)
	{
		pDlg->SetTimer(100, 1000, NULL);
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
		//pDlg->UpdateLog(_T("接受等待"));
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
		//pDlg->UpdateLog(_T("接受完成"));
	}
	CloseHandle(os.hEvent);
	return 0;
}

BOOL CMFCTransitionDlg::OpenCom(CString port)
{
	COMMTIMEOUTS TimeOuts;

	if (m_spConnect)
	{
		SetTimer(100, 1000, NULL);
		AfxMessageBox(_T("串口已打开"));
		return FALSE;
	}

	m_hCom = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);//重叠方式

	if (m_hCom==INVALID_HANDLE_VALUE)
	{
		SetTimer(100, 1000, NULL);
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
		m_pThread = AfxBeginThread(CommProc, this, THREAD_PRIORITY_HIGHEST,0,CREATE_SUSPENDED,NULL);
		if (m_pThread==NULL)
		{
			//m_spConnect = FALSE;
			CloseHandle(m_hCom);
			SetTimer(100, 1000, NULL);
			AfxMessageBox(_T("创建线程错误！"));
			return FALSE;
		}
		else
		{
			m_spConnect = TRUE;
			PurgeComm(m_hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
			m_pThread->ResumeThread();
		}
	}
	else
	{
		m_spConnect = FALSE;
		CloseHandle(m_hCom);
		SetTimer(100, 1000, NULL);
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
	m_hCom = NULL;
	return TRUE;
}

BOOL CMFCTransitionDlg::ConfigConnection()
{
	DCB dcb = { sizeof(dcb) };
	if (!GetCommState(m_hCom, &dcb))
		return FALSE;
	//BuildCommDCB(_T("9600,8,1,n"), &dcb);
	dcb.fBinary = TRUE;
	dcb.BaudRate = 9600;
	dcb.ByteSize = 8;
	dcb.fParity = FALSE;//NOPARITY;
	dcb.StopBits = 0;
	
	return SetCommState(m_hCom, &dcb);
}


void CMFCTransitionDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	/*CRect rc;
	this->GetWindowRect(rc);*/
	
	if (!isDebug)
	{
		isDebug = TRUE;
		m_btnDebug.SetWindowText(_T("<"));
		this->SetWindowPos(NULL, 0, 0, wndRect.right + 320, wndRect.bottom, SWP_NOMOVE);
	}
	else
	{
		isDebug = FALSE;
		m_btnDebug.SetWindowText(_T(">"));
		SetWindowPos(NULL, 0, 0, wndRect.right, wndRect.bottom, SWP_NOMOVE);
	}
	//SetWindowPos(NULL, 0, 0, wndRect.right, wndRect.bottom, SWP_NOMOVE);
	//SetWindowPos(NULL, 0, 0, wndRect.right, wndRect.bottom - 260, SWP_NOMOVE);
}


void CMFCTransitionDlg::OnBnClickedCheckAuto()
{
	// TODO:  在此添加控件通知处理程序代码
	HKEY hKey;

	CString strRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");//找到系统的启动项

	if (m_checkAuto.GetCheck())
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER,strRegPath,0,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS)
		{
			TCHAR szModule[MAX_PATH];
			GetModuleFileName(NULL, szModule, MAX_PATH);//获取本程序自身路径
			RegSetValueEx(hKey, _T("Transition"), 0, REG_SZ, (LPBYTE)szModule, (lstrlen(szModule) + 1)*sizeof(TCHAR));
			RegCloseKey(hKey);
		}
		else
		{
			UpdateLog(_T("系统参数错误，不能随系统自启"));
		}
	}
	else
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			RegDeleteValue(hKey, _T("Transition"));
			RegCloseKey(hKey);
		}
	}
}


//void CMFCTransitionDlg::OnClose()
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//	
//	CDialogEx::OnClose();
//}


HBRUSH CMFCTransitionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	CFont m_font;
	// TODO:  在此更改 DC 的任何特性
	
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_SERVER || 
		pWnd->GetDlgCtrlID() == IDC_STATIC_COM || 
		pWnd->GetDlgCtrlID() == IDC_STATIC_BAUD ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_AUTO ||
		pWnd->GetDlgCtrlID() == IDC_IDC_CBSTATIC)
	{
		m_font.CreatePointFont(100, _T("微软雅黑"));
		pDC->SelectObject(&m_font);       //设置字体 
		pDC->SetTextColor(RGB(255, 255, 255)); //设置字体颜色
		pDC->SetBkMode(TRANSPARENT);      //属性设置为透明
		return (HBRUSH)::GetStockObject(NULL_BRUSH); //不返回画刷

	}
	if (nCtlColor==CTLCOLOR_EDIT)
	{
		if (pWnd->GetDlgCtrlID() == IDC_EDIT_BAUD || pWnd->GetDlgCtrlID() == IDC_EDIT_IP)
		{
			m_font.CreatePointFont(92, _T("微软雅黑"));
			pDC->SelectObject(&m_font);
			pDC->SetBkColor(m_bgcolor);    // change the background  
			// color [background colour  
			// of the text ONLY]  
			pDC->SetTextColor(m_textcolor); // change the text color  
			hbr = (HBRUSH)m_bluebrush;
			return hbr;
		}
	}

	//if (((pWnd->GetDlgCtrlID() == IDC_EDIT_BAUD) && (nCtlColor == CTLCOLOR_EDIT))/* || ((pWnd->GetDlgCtrlID() == IDC_EDIT2) && (nCtlColor == CTLCOLOR_EDIT))*/)
	//{
	//	COLORREF clr = RGB(255, 0, 0);//此处设置背景颜色  
	//	pDC->SetTextColor(RGB(255,255,255));//设置字体颜色  
	//	//pDC->SetBkColor(clr);
	//	pDC->SetBkMode(TRANSPARENT);
	//	//m_redbrush = ::CreateSolidBrush(clr);
	//	//return m_redbrush;
	//	//return NULL;
	//}

	//if ((nCtlColor == CTLCOLOR_EDIT) && (pWnd->GetDlgCtrlID() == IDC_STATIC_BAUD))
	//{
	//	pDC->SetBkMode(TRANSPARENT); //设置背景透明，这样，输出字符的时候就
	//	//是所谓的空心字，而不是有白的底色
	//	pDC->SetTextColor(RGB(255, 0, 0)); //改变字体的颜色
	//	return HBRUSH(GetStockObject(HOLLOW_BRUSH));
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_COMBO_COM /*||
	//	pWnd->GetDlgCtrlID() == IDC_RADIO_ANONYMOUS ||
	//	pWnd->GetDlgCtrlID() == IDC_CHECK_SELFSELECT*/)
	//{
	//	
	//	pDC->SetBkMode(TRANSPARENT);

	//	CRect rc;
	//	pWnd->GetWindowRect(&rc);
	//	ScreenToClient(&rc);

	//	CDC* dc = GetDC();
	//	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);    //把父窗口背景先画到按钮上
	//	ReleaseDC(dc);
	//	pDC->SetTextColor(RGB(255, 255, 255)); //设置字体颜色
	//	return (HBRUSH) ::GetStockObject(NULL_BRUSH);
	//}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CMFCTransitionDlg::OnBnClickedButtonEnter()
{
	// TODO:  在此添加控件通知处理程序代码
	/*CString str;
	m_combobox.GetWindowText(str);*/
	if (!m_connet)
	{
		OnBnClickedButtonConnect();
	}
	else
	{
		m_bmpB_Enter.LoadBitmaps(IDB_BITMAP_ENTER_NORMAL, IDB_BITMAP_ENTER_WORK, 0, 0);
		m_bmpB_Enter.SizeToContent();
		m_listLog.ResetContent();
		CloseCom();
		m_btnCom.SetWindowText(_T("打开串口"));
		m_spConnect = FALSE;
		m_comboCom.EnableWindow(TRUE);
		UpdateLog(_T("串口已关闭！"));
		//m_btnEnter.EnableWindow(TRUE);
		m_connet = FALSE;
		m_pClient->Close();
		//delete m_pClient;
		//m_pClient = NULL;
		m_btnConnect.SetWindowText(_T("连接"));
		UpdateLog(_T("Disconnected!!!"));
		KillTimer(1);
		//m_isTimerSet = FALSE;
	}
}


void CMFCTransitionDlg::OnBnClickedButtonMin()
{
	// TODO:  在此添加控件通知处理程序代码
	HideWindow();
}


void CMFCTransitionDlg::OnBnClickedButtonClose()
{
	// TODO:  在此添加控件通知处理程序代码
	Shell_NotifyIcon(NIM_DELETE, &m_tnid);
	CDialogEx::OnCancel();
}


void CMFCTransitionDlg::OnBnClickedButtonAuto()
{
	// TODO:  在此添加控件通知处理程序代码
	HKEY hKey;

	CString strRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");//找到系统的启动项

	if (!m_isAuto)
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			TCHAR szModule[MAX_PATH];
			GetModuleFileName(NULL, szModule, MAX_PATH);//获取本程序自身路径
			RegSetValueEx(hKey, _T("Transition"), 0, REG_SZ, (LPBYTE)szModule, (lstrlen(szModule) + 1)*sizeof(TCHAR));
			RegCloseKey(hKey);
			m_isAuto = 1;
			m_bmpB_Auto.LoadBitmaps(IDB_BITMAP_AUTO_WORK, 0, 0, 0);
			m_bmpB_Auto.SizeToContent();
		}
		else
		{
			UpdateLog(_T("系统参数错误，不能随系统自启"));
		}
	}
	else
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			RegDeleteValue(hKey, _T("Transition"));
			RegCloseKey(hKey);
			m_isAuto = 0;
			m_bmpB_Auto.LoadBitmaps(IDB_BITMAP_AUTO_NORMAL, 0, 0, 0);
			m_bmpB_Auto.SizeToContent();
		}
	}
}


void CMFCTransitionDlg::OnStnClickedStaticAuto()
{
	// TODO:  在此添加控件通知处理程序代码
	/*OnBnClickedButtonAuto();
	CRect rc;
	GetDlgItem(IDC_BUTTON_AUTO)->GetWindowRect(rc);
	InvalidateRect(rc,TRUE);*/
	return;
}


void CMFCTransitionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//宽220，高40
	if (0 < point.x&&point.x <= 220 && 0 <= point.y&&point.y <= 40)
	{
		SendMessage(WM_SYSCOMMAND, 0xF012, 0);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}
