
// MFCTransitionDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
#define WIDTH 291
#define HEIGHT 298

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFCTransitionDlg �Ի���



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


// CMFCTransitionDlg ��Ϣ�������

BOOL CMFCTransitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_filePath = GetFilePath();
	/*CFont* pFont = new CFont;
	pFont->CreatePointFont(20 * 10, _T("΢���ź�"));
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
			MessageBox(_T("���ļ�����"));
		}
	}
	else
	{
		while (myFile.ReadString(temp))
		{
			len = temp.GetLength();
			if (len<=0)
			{
				UpdateLog(_T("��ȡ�ļ�����"));
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
	
	m_redcolor = RGB(255, 0, 0);                      // ��ɫ  
	m_bgcolor = RGB(23, 23, 23);                     // ��ɫ  
	m_textcolor = RGB(255, 255, 255);                 // �ı���ɫ����Ϊ��ɫ  
	m_redbrush.CreateSolidBrush(m_redcolor);      // ��ɫ����ɫ  
	m_bluebrush.CreateSolidBrush(m_bgcolor);    // ��ɫ����ɫ

	if (!m_connet)
	{
		OnBnClickedButtonEnter();
	}

	/*m_combobox.SetWindowSize(3);
	m_combobox.AddString(_T("���"));
	m_combobox.AddString(_T("�Һܺ�"));
	m_combobox.AddString(_T("Visual Studio"));
	m_combobox.AddString(_T("mycombobox"));
	m_combobox.AddString(_T("mycombobox"));
	m_combobox.AddString(_T("mycombobox"));
	m_combobox.AddString(_T("mycombobox"));
	m_combobox.AddString(_T("mycombobox"));
	m_combobox.InsertString(1, _T("235465"));*/
	//HBITMAP   hBitmap;
	//hBitmap = LoadBitmap(AfxGetInstanceHandle(),
	//	MAKEINTRESOURCE(IDB_BITMAP2)); // IDB_BITMAP_TESTΪ��ԴͼƬID 
	//((CButton *)GetDlgItem(IDC_BUTTON_CLOSE))->SetBitmap(hBitmap);

	//SetWindowLong(m_hWnd, GWL_HWNDPARENT, NULL);
	////CRgn m_rgn;
	//RECT rc;
	//GetWindowRect(&rc);
	//m_rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 11, 11);
	//if (!m_isDbg)
	//{
	//	SetWindowRgn(m_rgn, TRUE);//�ĳ�Բ�ǶԻ���
	//}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
				&dwSize) == ERROR_NO_MORE_ITEMS) // ö�ٴ���
				break;
			str.Format(_T("%S"), commName);
			m_combobox.AddString(str); // commName���Ǵ�������
			//cnt++;
			i++;
		}
		//m_combobox.SetWindowSize(m_combobox.GetCount());
		/*if (m_combobox.GetCount() == 0)
		{
		AfxMessageBox(_T("�޷���ϵͳ���ҵ���Ч���ڣ�"));
		}*/
		if (m_combobox.GetCount() == 0)
		{
			//SetTimer(100, 1000, NULL);
			AfxMessageBox(_T("�޷���ϵͳ���ҵ���Ч���ڣ�"));
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
		//			&dwSize) == ERROR_NO_MORE_ITEMS) // ö�ٴ���
		//			break;
		//		str.Format(_T("%S"), commName);
		//		m_combobox.AddString(str); // commName���Ǵ�������
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
	//temp += "images\\����.png";
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
	img.Load(m_filePath+_T("\\images\\����.png"));
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

	//img.Load(m_filePath + _T("\\images\\�ر�.bmp"));
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCTransitionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CImage img;
		CPaintDC dc(this);
		if (!img.IsNull()) img.Destroy();
		//img.Load(m_filePath + _T("\\images\\����.png"));
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
mode:����ģʽ(2-�ֶ� 1-�Զ� 0-Ĭ���Զ�)
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
recType:¼��״̬(1-��ʼ 2-��ͣ 3-ֹͣ)
roomName:������
teacherName:��ʦ��
courseName:�γ���
Notes:��ע
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
flag:��־(true-���õ�Ӱģʽ false-��ȡ��Ӱģʽ)
sourceNum:����ϳɻ������
width:��Ӱģʽ�ֱ���
height:
mplayout:��Ӱģʽ
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
	UpdateLog(_T("�������Ͽ����ӣ���������..."));
	m_bmpB_Enter.LoadBitmaps(IDB_BITMAP_ENTER_NORMAL, IDB_BITMAP_ENTER_WORK, 0, 0);
	m_bmpB_Enter.SizeToContent();
	//delete m_pClient;
	//m_pClient = NULL;
	//m_pClient = new ClientSocket;
	//m_pClient->SetParam(this);
	//if (!m_pClient->Create())
	//{
	//	UpdateLog(_T("�����׽���ʧ��"));
	//	return FALSE;
	//}
	//if (!m_pClient->Connect(m_ipStr, SERVER_PORT))
	//{
	//	UpdateLog(_T("���ӷ�����ʧ�ܣ�"));
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
	//	//	m_btnConnect.SetWindowText(_T("�Ͽ�"));
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCTransitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCTransitionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
				UpdateLog(_T("�ͻ���:��������!"));
				m_isAlive = FALSE;
			}
			else
			{
				//UpdateLog(_T("�ͻ���:����ʧ��!"));

				//KillTimer(1);
				UpdateLog(_T("����ʧ�ܣ���������������..."));
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
		AfxMessageBox(_T("���ӳ�ʱ"));
		KillTimer(nIDEvent);
		break;
	case 100:
		KillTimer(nIDEvent);
		keybd_event(VK_RETURN, 0, 0, 0);
		keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);//ģ��"�س�"����
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

	// ��ȡϵͳ��ǰʱ��

	str += _T("\r\n");

	// ���ڻ�����ʾ��־

	string = time.Format(_T("%Y/%m/%d %H:%M:%S--")) + str;
	string += "\r\n";

	// ��ʽ����ǰʱ��

	int lastLine = m_listLog.GetCount();
	m_listLog.InsertString(lastLine, string);
	m_listLog.SetCurSel(lastLine);
	//��ȡ�༭�����һ������

	//m_editLog.SetSel(lastLine + 1, lastLine + 2, 0);

	//ѡ��༭�����һ��

	//m_editLog.ReplaceSel(string);                                                             //�滻��ѡ��һ�е�����

}

BOOL CMFCTransitionDlg::PreTranslateMessage(MSG* pMsg)
{
	/*CRect rect;
	((CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS_SERVER))->GetClientRect(&rect);
	InvalidateRect(rect, TRUE);*/
	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)
	{
		//pMsg->wParam = VK_RETURN;   //��ESC������Ϣ�滻Ϊ�س�������Ϣ����������ESC��ʱ��  
		return TRUE;
		//Ҳ��ȥ����OnOK��������OnOKʲôҲ����������ESCҲ������  
	}
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)
	{
		if (m_connet)
		{
			SetTimer(100, 1000, NULL);
			AfxMessageBox(_T("�����������������"));
			return TRUE;
		}
		OnBnClickedButtonEnter();
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetKeyState(VK_CONTROL) < 0) // ����: if (GetAsyncKeyState(VK_CONTROL) & 0x8000)  
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
					//m_btnDebug.SetWindowText(_T("��"));
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
			AfxMessageBox(_T("�����׽���ʧ��"));
			return;
		}
		if (!m_pClient->Connect(m_ipStr, SERVER_PORT))
		{
			SetTimer(100, 1000, NULL);
			AfxMessageBox(_T("���ӷ�����ʧ�ܣ�"));
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
				}*/
				m_btnConnect.SetWindowText(_T("�Ͽ�"));
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
		m_btnConnect.SetWindowText(_T("����"));
		UpdateLog(_T("Disconnected!!!"));
		KillTimer(1);
		//m_isTimerSet = FALSE;
	}
}

//һϵ�д��ڵĺ���
void CMFCTransitionDlg::OnBnClickedButtonCom()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString port;
	if (!m_connet)
	{
		SetTimer(100, 1000, NULL);
		AfxMessageBox(_T("������δ���ӣ��������ӷ�������"));
		return;
	}
	int id = m_combobox.GetCurSel();

	if (!m_spConnect)
	{
		m_combobox.GetLBText(id, port);
		if (OpenCom(port))
		{
			//SetTimer(2, 3000, NULL);
			UpdateLog(_T("�����Ѵ򿪣�"));
			
			m_btnCom.SetWindowText(_T("�Ͽ�����"));

			CStdioFile myFile;
			CString temp;
			//isAuto = m_checkAuto.GetCheck();
			temp.Format(_T("%d"),id);
			BOOL openResult = myFile.Open(m_filePath+_T("connectInfo.txt"), CFile::modeCreate | CFile::modeReadWrite);
			if (!openResult)
			{
				UpdateLog(_T("���ļ�����д����Ϣʧ�ܣ�"));
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
		m_btnCom.SetWindowText(_T("�򿪴���"));
		m_spConnect = FALSE;
		m_combobox.EnableWindow(TRUE);
		UpdateLog(_T("�����ѹرգ�"));
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
		::GetCursorPos(lpoint);//�õ����λ��   
		CMenu    menu;
		menu.CreatePopupMenu();//����һ������ʽ�˵�   
		//���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ���   
		//���أ��������������   
		menu.AppendMenu(MF_STRING, WM_DESTROY, _T("�ر�"));
		//ȷ������ʽ�˵���λ��   
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		//��Դ����   
		HMENU    hmenu = menu.Detach();
		menu.DestroyMenu();
		Shell_NotifyIcon(NIM_DELETE, &m_tnid);
		delete    lpoint;
	}
		break;
	case    WM_LBUTTONDBLCLK://˫������Ĵ���   
	{
		this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶�   
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
	m_tnid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; //OK,����������̲�����. 
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
	m_d = up_d; m_e = up_e; m_f = up_f;//���±���״̬
	dn_d = m_d; dn_e = m_e; dn_f = m_f;//�����»�λ״̬
	unsigned char val;
	val = (m_d << 4) + (m_e << 2) + (m_f);
	unsigned char *sendBuf = new unsigned char[5];
	memcpy(sendBuf, "@#_#@", 5);
	memcpy(sendBuf + 2, &val, 1);
	WriteComm(sendBuf, PROTOCOL_LEN);
	if (isSet==E)
	{
		UpdateLog(_T("������:¼��״̬���£�"));
		isSet = -1;
	}
	else if (isSet==F)
	{
		UpdateLog(_T("������:����״̬���£�"));
		isSet = -1;
	}
	else if (isSet == D)
	{
		UpdateLog(_T("������:��Ӱģʽ���£�"));
		isSet = -1;
	}
	else if (isSet == -2)
	{
		UpdateLog(_T("������:����ͬ����..."));
	}
	else
	{
		UpdateLog(_T("������:�޸���"));
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
		UpdateLog(_T("����δ���ӣ�"));
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
			UpdateLog(_T("�ظ����ã�"));
			return FALSE;
		}
		SetRecordStatus(rcdType, _T(""), _T(""), _T(""), _T(""));
		
		temp = "����:<--����¼��ģʽ";
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
			UpdateLog(_T("�ظ����ã�"));
			return FALSE;
		}
		SetManageMode(mngType);
		
		temp = "����:<--���µ���ģʽ";
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
			UpdateLog(_T("�ظ����ã�"));
			return FALSE;
		}
		if (up_f==1)//1Ϊ�Զ�ģʽ
			SetManageMode(2);//2λ�ֶ�ģʽ
		SetOrGetMode(TRUE, 0, 0, 0, mode, 0, 0);
		
		temp = "����:<--���µ�Ӱģʽ";
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
		AfxMessageBox(_T("���ھ������"));
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
			//�����ﴦ��WorkBuffer[2],WorkBuffer[3]
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
			/*��������������*/
			y = PROTOCOL_LEN;		//�����Э�飬���۳������ȥ��6���ֽ�
		}
		else
		{
			y = 1;		//�������Э�飬ȥ�����ַ�@
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
		AfxMessageBox(_T("���ھ������"));
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
		//pDlg->UpdateLog(_T("���ܵȴ�"));
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
		//pDlg->UpdateLog(_T("�������"));
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
		AfxMessageBox(_T("�����Ѵ�"));
		return FALSE;
	}

	m_hCom = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);//�ص���ʽ

	if (m_hCom==INVALID_HANDLE_VALUE)
	{
		SetTimer(100, 1000, NULL);
		AfxMessageBox(_T("���ڲ����ڻ�ռ�ã�"));
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
			AfxMessageBox(_T("�����̴߳���"));
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
		AfxMessageBox(_T("�����ļ�����"));
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
			GetOverlappedResult(m_hCom, &m_osWrite, &length, TRUE);//�ȴ�
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	HKEY hKey;

	CString strRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");//�ҵ�ϵͳ��������

	if (m_checkAuto.GetCheck())
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER,strRegPath,0,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS)
		{
			TCHAR szModule[MAX_PATH];
			GetModuleFileName(NULL, szModule, MAX_PATH);//��ȡ����������·��
			RegSetValueEx(hKey, _T("Transition"), 0, REG_SZ, (LPBYTE)szModule, (lstrlen(szModule) + 1)*sizeof(TCHAR));
			RegCloseKey(hKey);
		}
		else
		{
			UpdateLog(_T("ϵͳ�������󣬲�����ϵͳ����"));
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
//	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	
//	CDialogEx::OnClose();
//}


HBRUSH CMFCTransitionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	CFont m_font;
	// TODO:  �ڴ˸��� DC ���κ�����
	
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_SERVER || 
		pWnd->GetDlgCtrlID() == IDC_STATIC_COM || 
		pWnd->GetDlgCtrlID() == IDC_STATIC_BAUD ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_AUTO ||
		pWnd->GetDlgCtrlID() == IDC_IDC_CBSTATIC)
	{
		m_font.CreatePointFont(100, _T("΢���ź�"));
		pDC->SelectObject(&m_font);       //�������� 
		pDC->SetTextColor(RGB(255, 255, 255)); //����������ɫ
		pDC->SetBkMode(TRANSPARENT);      //��������Ϊ͸��
		return (HBRUSH)::GetStockObject(NULL_BRUSH); //�����ػ�ˢ

	}
	if (nCtlColor==CTLCOLOR_EDIT)
	{
		if (pWnd->GetDlgCtrlID() == IDC_EDIT_BAUD || pWnd->GetDlgCtrlID() == IDC_EDIT_IP)
		{
			m_font.CreatePointFont(92, _T("΢���ź�"));
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
	//	COLORREF clr = RGB(255, 0, 0);//�˴����ñ�����ɫ  
	//	pDC->SetTextColor(RGB(255,255,255));//����������ɫ  
	//	//pDC->SetBkColor(clr);
	//	pDC->SetBkMode(TRANSPARENT);
	//	//m_redbrush = ::CreateSolidBrush(clr);
	//	//return m_redbrush;
	//	//return NULL;
	//}

	//if ((nCtlColor == CTLCOLOR_EDIT) && (pWnd->GetDlgCtrlID() == IDC_STATIC_BAUD))
	//{
	//	pDC->SetBkMode(TRANSPARENT); //���ñ���͸��������������ַ���ʱ���
	//	//����ν�Ŀ����֣��������а׵ĵ�ɫ
	//	pDC->SetTextColor(RGB(255, 0, 0)); //�ı��������ɫ
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
	//	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);    //�Ѹ����ڱ����Ȼ�����ť��
	//	ReleaseDC(dc);
	//	pDC->SetTextColor(RGB(255, 255, 255)); //����������ɫ
	//	return (HBRUSH) ::GetStockObject(NULL_BRUSH);
	//}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CMFCTransitionDlg::OnBnClickedButtonEnter()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		m_btnCom.SetWindowText(_T("�򿪴���"));
		m_spConnect = FALSE;
		m_comboCom.EnableWindow(TRUE);
		UpdateLog(_T("�����ѹرգ�"));
		//m_btnEnter.EnableWindow(TRUE);
		m_connet = FALSE;
		m_pClient->Close();
		//delete m_pClient;
		//m_pClient = NULL;
		m_btnConnect.SetWindowText(_T("����"));
		UpdateLog(_T("Disconnected!!!"));
		KillTimer(1);
		//m_isTimerSet = FALSE;
	}
}


void CMFCTransitionDlg::OnBnClickedButtonMin()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	HideWindow();
}


void CMFCTransitionDlg::OnBnClickedButtonClose()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Shell_NotifyIcon(NIM_DELETE, &m_tnid);
	CDialogEx::OnCancel();
}


void CMFCTransitionDlg::OnBnClickedButtonAuto()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	HKEY hKey;

	CString strRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");//�ҵ�ϵͳ��������

	if (!m_isAuto)
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			TCHAR szModule[MAX_PATH];
			GetModuleFileName(NULL, szModule, MAX_PATH);//��ȡ����������·��
			RegSetValueEx(hKey, _T("Transition"), 0, REG_SZ, (LPBYTE)szModule, (lstrlen(szModule) + 1)*sizeof(TCHAR));
			RegCloseKey(hKey);
			m_isAuto = 1;
			m_bmpB_Auto.LoadBitmaps(IDB_BITMAP_AUTO_WORK, 0, 0, 0);
			m_bmpB_Auto.SizeToContent();
		}
		else
		{
			UpdateLog(_T("ϵͳ�������󣬲�����ϵͳ����"));
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	/*OnBnClickedButtonAuto();
	CRect rc;
	GetDlgItem(IDC_BUTTON_AUTO)->GetWindowRect(rc);
	InvalidateRect(rc,TRUE);*/
	return;
}


void CMFCTransitionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��220����40
	if (0 < point.x&&point.x <= 220 && 0 <= point.y&&point.y <= 40)
	{
		SendMessage(WM_SYSCOMMAND, 0xF012, 0);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}
