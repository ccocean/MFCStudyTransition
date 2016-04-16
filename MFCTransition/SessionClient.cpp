#include "StdAfx.h"
#include "SessionClient.h"
#include "session/SesFactory.h"
#include "UIUtil.h"
#include "FTPTypeDef.h"
#include "MFCTransition.h"

CSessionClient::CSessionClient(void)
{
	memset(m_recvBuf, 0, 10240);
	m_length = 0;
	sp_ses_ = NULL;
}

CSessionClient::~CSessionClient(void)
{
}

bool CSessionClient::CreateLink(const std::string ip, unsigned short port)
{
	m_conInfo = S_CONNECT_INFO(TYPE_TCP, ip, port, 1);
	//sp_ses_ = boost::shared_ptr<ISession>(SesFactory::Create(m_conInfo.type_, g_sp_io->ios_));
	sp_ses_ = new CTCPSocket(g_sp_io->ios_);
	int nLen = sizeof(CTCPSocket);
	sp_ses_->register_observer(this);
	bool nRes = sp_ses_->open(m_conInfo);
	WaitForSingleObject(sp_ses_->m_eveConnect, 1500);
	return nRes;
}

void CSessionClient::CloseLink()
{
	if (sp_ses_)
	{
		sp_ses_->delete_observer();
		sp_ses_->close();
		delete sp_ses_;
		sp_ses_ = NULL;
	}
}

void CSessionClient::handle_receive( const char* buff, int len, int bFlag)
{
	TRACE1("handle_receive start: len: %d\n", len);
	is_online = true;
	CString strDoc;
	//strDoc = StringConvertor::AnsiToWide(buff);
	strDoc = StringConvertor::Utf8ToWide(buff);
	strDoc = strDoc.Left(len);
	if (bFlag == 1)
	{
		m_heartXml.SetDoc(strDoc);
		m_heartXml.Save(_T("heartbeat.xml"));
		//theApp.m_pDlg->PostMessage(WM_MY_HEARTBEAT);
		return;
	}
	if (bFlag == 2)
	{
		//theApp.m_pDlg->PostMessage(WM_MY_KICK_OUT);
		m_kickXml.SetDoc(strDoc);
		//m_kickXml.Save(_T("kickout.xml"));
		return;
	}
	m_xml.SetDoc(strDoc);
	//m_xml.Save(_T("recv.xml"));
	CString strLength;
	strLength.Format(_T("%d"), strDoc.GetLength());
	m_eve.SetEvent();
	TRACE1("handle_receive end: len: %d\n", len);
}

void CSessionClient::handle_write( const char* buff, int len )
{

}

int CSessionClient::SendCmd( char* pBuf, int nLen, DWORD interval /*= 3000*/ )
{
	::CWaitCursor cur;
	TRACE1("write start: len: %d\n", nLen);
	if (!sp_ses_->is_connect())
	{
		return -1;
	}
	m_eve.ResetEvent();
	sp_ses_->write(pBuf, nLen);
	DWORD dwRet = WaitForSingleObject(m_eve, interval);
	TRACE1("write end: WaitForSingleObject: %d\n", dwRet);
	switch (dwRet)
	{
	case WAIT_OBJECT_0:
		return 0;
	case WAIT_TIMEOUT:
		return 1;
	case WAIT_ABANDONED:
		return 2;
	}
	return 3;
	TRACE1("write end: %d\n", nLen);
}

void CSessionClient::RegisterObserver( ISessionClientObserver* ob )
{
	m_listOb.push_back(ob);
}

void CSessionClient::RemoveObserver( ISessionClientObserver* ob )
{
	m_listOb.remove(ob);
}

//////////////////////////////////////////////////////////////////////
/*
@param
passkey:��Ϣ��Դ(ManagePlatform / FTPTools)
user:�û���(admin / guest)
passwd:����
optType:��������(0-Disconnect 1-connect 2-Reconnect)
*/
bool CSessionClient::LoginAll(CString passkey, CString user, CString passwd, UINT optType)
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, ALL_LOGIN_CODE);
	xml.AddChildElem(PASSKEY, passkey);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(USER, user);
	xml.AddChildElem(PASSWD, passwd);
	xml.AddChildElem(OPTTYPE, optType);
	return packet(ALL_LOGIN_CODE, xml);
}

bool CSessionClient::SendHeartBeat(int rcdStatus, int rcdTime, int mpMode, int mpLayout)
{
	::CMarkup heart_xml;
	heart_xml.SetDoc(TITLE);
	heart_xml.AddElem(REQUESTMSG);
	heart_xml.IntoElem();
	heart_xml.AddElem(MSGHEAD);
	heart_xml.AddChildElem(MSGCODE, MNG_HEARTBEAT_CODE);
	heart_xml.AddChildElem(PASSKEY, MANAGERPLAT);
	heart_xml.AddElem(MSGBODY);
	heart_xml.AddChildElem(RECORDSTATUS, rcdStatus);
	heart_xml.AddChildElem(RECORDTIME, rcdTime);
	heart_xml.AddChildElem(MPMODEL, mpMode);
	heart_xml.AddChildElem(MPLAYOUT, mpLayout);
	
	std::string str = StringConvertor::WideToUtf8(heart_xml.GetDoc());
	External_Mag_Head head;
	memcpy(head.identifier, "@#$&", 4);
	head.version = 2015;
	head.load_len = str.length();
	head.msg_code = MNG_HEARTBEAT_CODE;
	head.reserver = 0;
	char szBuff[PACKLENGTH] = {0};
	int nLen = 0;
	memcpy(szBuff, &head, sizeof(head));
	nLen += sizeof(head);
	memcpy(szBuff + nLen, str.c_str(), str.length());
	nLen += str.length();
	heart_xml.SetDoc(NULL);
	sp_ses_->write(szBuff, nLen);
	return true;
	//return SendCmd(szBuff, nLen, 5000) == 0 ? true : false;
}

bool CSessionClient::RequestStream( int stream )
{
	return true;
}

bool CSessionClient::GetDiskSpace()
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, MNG_GETDISKSPACE_CODE);
	xml.AddChildElem(PASSKEY, MANAGERPLAT);
	return packet(MNG_GETDISKSPACE_CODE, xml);
}

/*
@param 
ID:�����ID��(1-��ʦ���� 2-ѧ������)
type:����(1-�� 2-�� 3-�� 4-�� 5-�Ŵ� 6-��С)
addr:��ַλ
speed:�ٶ�
num:Ԥ��λ
status:״̬(1-��ʼ�ƶ� 0-ֹͣ�ƶ�)
*/
bool CSessionClient::ControlCamera( int ID, int type, int addr, int speed, int num, int status )
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, MNG_CTRLCAMERA_CODE);
	xml.AddChildElem(PASSKEY, MANAGERPLAT);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(_T("ID"), ID);
	xml.AddChildElem(_T("Type"), type);
	xml.AddChildElem(_T("Addr"), addr);
	xml.AddChildElem(_T("Speed"), speed);
	xml.AddChildElem(_T("Num"), num);
	xml.AddChildElem(_T("Status"), status);
	packet(MNG_CTRLCAMERA_CODE, xml, true);
	return true;
}

/*
@param
transparency:͸����(0-��͸ 100-ȫ͸��)
length:ͼƬ��С(�ֽ�)
fmt:ͼƬ��ʽ(0-Jpg ��ʱֻ֧��Jpg)
pos:λ��(0-�Զ���λ�� 1-left up 2-right up 3-left down 4-right down)
xPos:ˮƽλ��(posΪ0ʱxPos��yPos����Ч)
yPos:��ֱλ��
*/
bool CSessionClient::SetLogo( BOOL bFlag, int transparency, int length, int fmt, UINT pos, int xPos, int yPos, BOOL bEnable )
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, MNG_SETLOGO_CODE);
	xml.AddChildElem(PASSKEY, MANAGERPLAT);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(_T("OptType"), (int)bFlag);
	xml.AddChildElem(_T("Alpha"), transparency);
	xml.AddChildElem(_T("LogoLength"), length);
	xml.AddChildElem(_T("format"), fmt);
	xml.AddChildElem(_T("Position"), pos);
	xml.AddChildElem(_T("Xpos"), xPos);
	xml.AddChildElem(_T("Ypos"), yPos);
	xml.AddChildElem(_T("Enable"), (int)bEnable);
	if (!bFlag)
	{
		return packet(MNG_SETLOGO_CODE, xml);
	}
	packet(MNG_SETLOGO_CODE, xml, true);
	return true;
}

/*
@param
nPos:λ��(1-���� 2-���� 3-���� 4-���� 0-��������)
title:��Ļ�ֶ�(���֧��23�����֣�46��Ӣ����ĸ)
flag:��ʾʱ���־
alpha:͸����(0-100)
xPos:ˮƽλ��(nPosΪ0����Ч)
yPos:��ֱλ��
enable:��ʾ��Ļ��־
*/
bool CSessionClient::SetTitleAndDate(BOOL bFlag, int nPos, CString title, BOOL flag, int alpha, int xPos, int yPos, BOOL enable )
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, MNG_SETDATE_CODE);
	xml.AddChildElem(PASSKEY, MANAGERPLAT);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(_T("OptType"), (int)bFlag);
	xml.AddChildElem(_T("Position"), nPos);
	xml.AddChildElem(_T("Title"), title);
	xml.AddChildElem(_T("Time"), (int)flag);
	xml.AddChildElem(_T("Alpha"), alpha);
	xml.AddChildElem(_T("Xpos"), xPos);
	xml.AddChildElem(_T("Ypos"), yPos);
	xml.AddChildElem(_T("Enable"), (int)enable);
	return packet(MNG_SETDATE_CODE, xml);
}

/*
@param
recType:¼��״̬(1-��ʼ 2-��ͣ 3-ֹͣ)
roomName:������
teacherName:��ʦ��
courseName:�γ���
Notes:��ע
*/
bool CSessionClient::SetRecordStatus( int recType, CString roomName, CString teacherName, CString courseName, CString notes )
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, MNG_SETRCD_CODE);
	xml.AddChildElem(PASSKEY, MANAGERPLAT);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(_T("RecType"), recType);
	xml.AddChildElem(_T("RoomName"), roomName);
	xml.AddChildElem(_T("TeacherName"), teacherName);
	xml.AddChildElem(_T("CourseName"), courseName);
	xml.AddChildElem(_T("Notes"), notes);
	return packet(MNG_SETRCD_CODE, xml);
}

/*
@param
flag:��־(true-���õ�Ӱģʽ false-��ȡ��Ӱģʽ)
sourceNum:����ϳɻ������
width:��Ӱģʽ�ֱ���
height:
mplayout:��Ӱģʽ
*/
bool CSessionClient::SetOrGetMode( BOOL flag, int sourceNum, int width, int height, int mplayout, int FrameRate, int bitRate)
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, MNG_SETMOVIE_CODE);
	xml.AddChildElem(PASSKEY, MANAGERPLAT);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(_T("OptType"), (int)flag);
	xml.AddChildElem(_T("SourceNum"), sourceNum);
	xml.AddChildElem(_T("Width"), width);
	xml.AddChildElem(_T("Height"), height);
	xml.AddChildElem(_T("MPlayout"), mplayout);
	xml.AddChildElem(_T("FrameRate"), FrameRate);
	xml.AddChildElem(_T("BitRate"), bitRate);
	return packet(MNG_SETMOVIE_CODE, xml);
}

bool CSessionClient::GetVersion()
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, MNG_GETVER_CODE);
	xml.AddChildElem(PASSKEY, MANAGERPLAT);
	return packet(MNG_GETVER_CODE, xml);
}

/*
@param
mode:����ģʽ(2-�ֶ� 1-�Զ� 0-Ĭ���Զ�)
*/
bool CSessionClient::SetManageMode( UINT mode )
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, MNG_MODE_CODE);
	xml.AddChildElem(PASSKEY, MANAGERPLAT);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(_T("MPmode"), mode);
	return packet(MNG_MODE_CODE, xml);
}

bool CSessionClient::packet( unsigned short msgCode, ::CMarkup& xml )
{
	char sendBuf[10240] = {0};
	int nLen = 0;
	CString strDoc = xml.GetDoc();
	std::string str = StringConvertor::WideToUtf8(strDoc);
	External_Mag_Head msgHead;
	memcpy(msgHead.identifier, "@#$&", 4);
	msgHead.version = 2015;
	msgHead.load_len = str.length();
	msgHead.msg_code = msgCode;
	msgHead.reserver = 0;
	memcpy(sendBuf, &msgHead, sizeof(msgHead));
	nLen += sizeof(msgHead);
	memcpy(sendBuf + nLen, str.c_str(), str.length());
	nLen += str.length();
	return SendCmd(sendBuf, nLen, 5000) == 0 ? TRUE : FALSE;
}

void CSessionClient::packet( unsigned short msgCode, ::CMarkup& xml, bool flag )
{
	char sendBuf[10240] = {0};
	int nLen = 0;
	CString strDoc = xml.GetDoc();
	std::string str = StringConvertor::WideToUtf8(strDoc);
	External_Mag_Head msgHead;
	memcpy(msgHead.identifier, "@#$&", 4);
	msgHead.version = 2015;
	msgHead.load_len = str.length();
	msgHead.msg_code = msgCode;
	msgHead.reserver = 0;
	memcpy(sendBuf, &msgHead, sizeof(msgHead));
	nLen += sizeof(msgHead);
	memcpy(sendBuf + nLen, str.c_str(), str.length());
	nLen += str.length();
	if (is_open())
	{
		sp_ses_->write(sendBuf, nLen);
	}
}

/*
@param
key:��ѯ�μ����ؼ���
order:���򷽷�(0/1-ʱ�併�� 2-ʱ������ 3-��С���� 4-��С����)
curPage:��ǰҳ��
PageSize:ÿҳ��ʾ��Ŀ����
*/
bool CSessionClient::FTP_RequestList( CString key, int order, int curPage, int PageSize /*= 10*/ )
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, FTP_SEARCH_CODE);
	xml.AddChildElem(PASSKEY, MANAGERPLAT);
	xml.AddElem(MSGBODY);
	xml.IntoElem();
	xml.AddElem(QUERYINFO);
	xml.AddChildElem(QUERYNAME, key);
	xml.AddChildElem(ORDER, order);
	xml.AddChildElem(CURPAGE, curPage);
	xml.AddChildElem(PAGESIZE, PageSize);
	xml.AddChildElem(TOTAL);
	return packet(FTP_SEARCH_CODE, xml);
}

/*
@param
strOld:ԭ�μ���
strNew:���޸ĵĿμ���(��֧������)
strNotes:�޸ĵı�ע��Ϣ(֧������)
strTeacher:�ڿν�ʦ��
strLession:�γ���
*/
bool CSessionClient::FTP_ChangeInfo( CString strOld, CString strNew,CString mode, CString strNotes, CString strTeacher, CString strLession )
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, FTP_CHANGE_CODE);
	xml.AddChildElem(PASSKEY, MANAGERPLAT);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(FILENAME, strOld);
	xml.AddChildElem(NEWFILENAME, strNew);
	xml.AddChildElem(COURSETEACHER, strTeacher);
	xml.AddChildElem(COURSESUBJECT, strLession);
	xml.AddChildElem(NOTES, strNotes);
	xml.AddChildElem(MODE, mode);
	return packet(FTP_CHANGE_CODE, xml);
}

/*
@param
arrName:Ҫɾ���Ŀμ�������
*/
bool CSessionClient::FTP_Delete( CStringArray& arrName )
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, FTP_DELETE_CODE);
	xml.AddChildElem(PASSKEY, MANAGERPLAT);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(TOTAL, arrName.GetCount());
	xml.IntoElem();
	for (int i = 0;i < arrName.GetCount();i++)
	{
		xml.AddElem(COURSEINFO);
		xml.AddChildElem(FILENAME, arrName.GetAt(i));
	}
	return packet(FTP_DELETE_CODE, xml);
}

void CSessionClient::SendData( char* buff, int len )
{
	if (is_open())
	{
		sp_ses_->write(buff, len);
	}
}

/*
@param
flag : 0(getģʽ) 1(setģʽ)
mode : �����׻���ģʽ(0�ر� 1VGA 2��ʦ�ض� 3ѧ��ȫ�� 4����)
time : �����׻��汣��ʱ��(1-99s)
*/
bool CSessionClient::SetOrGetFirstPic( BOOL flag, UINT mode, UINT time )
{
	::CMarkup xml;
	xml.SetDoc(TITLE);
	xml.AddElem(REQUESTMSG);
	xml.IntoElem();
	xml.AddElem(MSGHEAD);
	xml.AddChildElem(MSGCODE, MNG_SET_FIRSTPIC_CODE);
	xml.AddChildElem(PASSKEY, MANAGERPLAT);
	xml.AddElem(MSGBODY);
	xml.AddChildElem(OPTTYPE, (int)flag);
	xml.AddChildElem(_T("FirstPicture"), mode);
	xml.AddChildElem(_T("HoldTime"), time);
	return packet(MNG_SET_FIRSTPIC_CODE, xml);
}
