
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
	
	//m_loginDlg.SetSocket(m_pClient);
		//char *pBuff = "Hello!";
		//m_pClient->SendMSG(pBuff, 6);
	
	m_editBaud.SetWindowText(_T("9600"));
	m_comboCom.SetCurSel(0);
	m_spConnect = FALSE;
	m_connet = FALSE;
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
		CDialogEx::OnPaint();
	}
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
	UpdateLog(_T("�������Ͽ����ӣ���������..."));
	//delete m_pClient;
	//m_pClient = NULL;
	//m_pClient = new ClientSocket;
	m_pClient->SetParam(this);
	if (!m_pClient->Create())
	{
		AfxMessageBox(_T("�����׽���ʧ��"));
		return FALSE;
	}
	if (!m_pClient->Connect(m_ipStr, SERVER_PORT))
	{
		AfxMessageBox(_T("���ӷ�����ʧ�ܣ�"));
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
			m_btnConnect.SetWindowText(_T("�Ͽ�"));
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
	switch (nIDEvent)
	{
	case 1:
		//::CMarkup heart_xml;
		m_pClient->SetReconnectTimer(6000);
		nRes = SendHearBeat(0, 2000, 1, 0x211);
		if (nRes)
		{
			UpdateLog(_T("�ͻ���:��������!"));
		}
		else
		{
			UpdateLog(_T("�ͻ���:����ʧ��!"));
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

	// ��ȡϵͳ��ǰʱ��

	str += _T("\r\n");

	// ���ڻ�����ʾ��־

	string = time.Format(_T("%Y/%m/%d %H:%M:%S--")) + str;
	string += "\r\n";

	// ��ʽ����ǰʱ��

	int lastLine = m_editLog.LineIndex(m_editLog.GetLineCount() - 1);

	//��ȡ�༭�����һ������

	m_editLog.SetSel(lastLine + 1, lastLine + 2, 0);

	//ѡ��༭�����һ��

	m_editLog.ReplaceSel(string);                                                             //�滻��ѡ��һ�е�����
}

void CMFCTransitionDlg::OnBnClickedButtonConnect()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BYTE nf1, nf2, nf3, nf4;
	m_ipServer.GetAddress(nf1, nf2, nf3, nf4);
	m_ipStr.Format(_T("%d.%d.%d.%d"), nf1, nf2, nf3, nf4);

	if (m_connet != TRUE)
	{
		m_pClient = new ClientSocket();
		m_pClient->SetParam(this);
		if (!m_pClient->Create())
		{
			AfxMessageBox(_T("�����׽���ʧ��"));
			return;
		}
		if (!m_pClient->Connect(m_ipStr, SERVER_PORT))
		{
			AfxMessageBox(_T("���ӷ�����ʧ�ܣ�"));
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
			}
		}
	}
	else
	{
		m_connet = FALSE;
		m_pClient->Close();
		delete m_pClient;
		m_pClient = NULL;
		m_btnConnect.SetWindowText(_T("����"));
		UpdateLog(_T("Disconnected!!!"));
		KillTimer(1);
	}

}

//һϵ�д��ڵĺ���
void CMFCTransitionDlg::OnBnClickedButtonCom()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString port;
	/*if (!m_connet)
	{
		AfxMessageBox(_T("������δ���ӣ��������ӷ�������"));
		return;
	}*/
	int id = m_comboCom.GetCurSel();

	if (!m_spConnect)
	{
		m_comboCom.GetLBText(id, port);
		if (OpenCom(port))
		{
			//SetTimer(2, 3000, NULL);
			UpdateLog(_T("�����Ѵ򿪣�"));
			m_comboCom.EnableWindow(FALSE);
			m_btnCom.SetWindowText(_T("�Ͽ�����"));
		}
	}
	else
	{
		CloseCom();
		m_btnCom.SetWindowText(_T("�򿪴���"));
		m_spConnect = FALSE;
		m_comboCom.EnableWindow(TRUE);
		UpdateLog(_T("�����ѹرգ�"));
	}
}

LRESULT CMFCTransitionDlg::Send2SerialPort(WPARAM wParam, LPARAM lParam)
{
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
	}
	else if (isSet==F)
	{
		UpdateLog(_T("������:����״̬���£�"));
	}
	else if (isSet == D)
	{
		UpdateLog(_T("������:��Ӱģʽ���£�"));
	}
	else if (isSet == -1)
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
		temp = ReadyBuffer;
		temp += "���ڣ�<--����¼��ģʽ-->";
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
		temp = ReadyBuffer;
		temp += "���ڣ�<--���µ���ģʽ-->";
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
		//if (up_f==1)//1Ϊ�Զ�ģʽ
		//	SetManageMode(2);//2λ�ֶ�ģʽ
		SetOrGetMode(TRUE, 0, 0, 0, mode, 0, 0);
		temp = ReadyBuffer;
		temp += "���ڣ�<--���µ�Ӱģʽ-->";
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
	
	

	CMFCTransitionDlg *pDlg = (CMFCTransitionDlg*)pParam;

	if (!pDlg->m_hCom)
	{
		AfxMessageBox(_T("���ھ������"));
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
		AfxMessageBox(_T("�����Ѵ�"));
		return FALSE;
	}

	m_hCom = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);//�ص���ʽ

	if (m_hCom==INVALID_HANDLE_VALUE)
	{
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
		m_pThread = AfxBeginThread(CommProc, this, THREAD_PRIORITY_HIGHEST,0,0,NULL);
		if (m_pThread==NULL)
		{
			CloseHandle(m_hCom);
			AfxMessageBox(_T("�����̴߳���"));
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
