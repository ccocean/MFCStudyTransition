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
passkey:消息来源(ManagePlatform / FTPTools)
user:用户名(admin / guest)
passwd:密码
optType:操作类型(0-Disconnect 1-connect 2-Reconnect)
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
ID:摄像机ID号(1-教师待定 2-学生待定)
type:类型(1-上 2-下 3-左 4-右 5-放大 6-缩小)
addr:地址位
speed:速度
num:预置位
status:状态(1-开始移动 0-停止移动)
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
transparency:透明度(0-不透 100-全透明)
length:图片大小(字节)
fmt:图片格式(0-Jpg 暂时只支持Jpg)
pos:位置(0-自定义位置 1-left up 2-right up 3-left down 4-right down)
xPos:水平位置(pos为0时xPos和yPos才有效)
yPos:竖直位置
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
nPos:位置(1-左上 2-右上 3-左下 4-右下 0-开启坐标)
title:字幕字段(最大支持23个汉字，46个英文字母)
flag:显示时间标志
alpha:透明度(0-100)
xPos:水平位置(nPos为0才有效)
yPos:竖直位置
enable:显示字幕标志
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
recType:录制状态(1-开始 2-暂停 3-停止)
roomName:教室名
teacherName:教师名
courseName:课程名
Notes:备注
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
flag:标志(true-设置电影模式 false-获取电影模式)
sourceNum:参与合成画面个数
width:电影模式分辨率
height:
mplayout:电影模式
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
mode:导播模式(2-手动 1-自动 0-默认自动)
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
key:查询课件名关键字
order:排序方法(0/1-时间降序 2-时间升序 3-大小降序 4-大小升序)
curPage:当前页码
PageSize:每页显示条目数量
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
strOld:原课件名
strNew:新修改的课件名(不支持中文)
strNotes:修改的备注信息(支持中文)
strTeacher:授课教师名
strLession:课程名
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
arrName:要删除的课件名数组
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
flag : 0(get模式) 1(set模式)
mode : 设置首画面模式(0关闭 1VGA 2老师特定 3学生全景 4板书)
time : 设置首画面保持时间(1-99s)
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
