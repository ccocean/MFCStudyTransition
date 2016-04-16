#pragma once

#define TITLE           _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>")
#define REQUESTMSG      _T("RequestMsg")
#define RESPONSEMSG     _T("ResponseMsg")
#define MSGHEAD			_T("MsgHead")
#define MSGCODE			_T("MsgCode")
#define PASSKEY			_T("PassKey")
#define MSGBODY			_T("MsgBody")
#define QUERYINFO		_T("QueryInfo")
#define QUERYNAME		_T("QueryName")
#define ORDER			_T("Order")
#define CURPAGE			_T("CurPage")
#define PAGESIZE		_T("PageSize")
#define TOTAL			_T("Total")
#define COURSEINFO		_T("CourseInfo")
#define FILENAME		_T("FileName")
#define NEWFILENAME     _T("NewFileName")
#define NOTES			_T("Notes")
#define MODE			_T("mode")
#define RESULT			_T("Result")
#define MOVIESIZE		_T("MovieSize")
#define RESSIZE			_T("ResSize")
#define RCDSTARTTIME    _T("RcdStartTime")
#define RCDDURATION     _T("RcdDurationTime")
#define FTPUPLOAD		_T("FTPupload")
#define DOWNLOADCNT		_T("DownloadCnt")
#define COURSETEACHER   _T("CourseTeacher")
#define COURSESUBJECT   _T("CourseSubject")
#define USER            _T("User")
#define PASSWD          _T("PassWd")
#define OPTTYPE         _T("OptType")
#define FTPTOOLS        _T("FTPTools")
#define MANAGERPLAT     _T("ManagePlatform")
#define SERIALPLAT		_T("SerialPlatform")
#define RECORDSTATUS    _T("RecordStatus")
#define RECORDTIME      _T("RecordTime")
#define MPMODEL         _T("MPmodel")
#define MPLAYOUT        _T("MPlayout")

#define RESNAME         _T("�μ���")
#define VIDEOSIZE       _T("��Ӱģʽ��С(MB)")
#define LESSIONSIZE     _T("�μ���С(MB)") 
#define RECODESTARTTIME _T("¼�ƿ�ʼʱ��")
#define RCDDURTIME      _T("¼��ʱ��(S)")
#define NOTE            _T("��ע��Ϣ")
#define FTPUPSTATUS     _T("�ϴ�״̬")
#define FTPDOWNCOUNT    _T("���ش���")
#define TEACHERNAME     _T("�γ�������ʦ")
#define COURSENAME      _T("�γ�����")
#define FTP_NOUPLOAD    _T("δ���ϴ�")
#define FTP_YESUPLOAD   _T("���ϴ�")

#define STA_FILENAME    _T("����")
#define STA_STATUS      _T("״̬")
#define STA_PROGRESS    _T("����")
#define STA_SIZE        _T("��С(MB)")
#define STA_LOCALPATH   _T("����·��")
#define STA_REMOTEPATH  _T("Զ��·��")
#define STA_DIRECTION   _T("<->")

#define LOGIN_USERNAME  _T("Username")
#define LOGIN_INFO      _T("LoginInfo")
#define LOGIN_PASSWD    _T("Password")
#define LOGIN_SERVER_IP _T("Server IP")
#define LOGIN_KEY       1

// Э������
#define  ALL_LOGIN_CODE        30015    // ��¼
// FTP  
#define  FTP_SEARCH_CODE       4099     // FTP����
#define  FTP_DELETE_CODE       4100     // FTPɾ��
#define  FTP_CHANGE_CODE       4102     // FTP�޸�
// ManagePlatform
#define  MNG_REQSTREAM_CODE    30011    // ��������
#define  MNG_CTRLCAMERA_CODE   30013    // ��������ͷԶң
#define  MNG_SETLOGO_CODE      30016    // ����logo
#define  MNG_SETDATE_CODE      30018    // ������Ļ&&ʱ��
#define  MNG_SETRCD_CODE       30010    // ����¼��״̬
#define  MNG_HEARTBEAT_CODE    30032    // ˫������
#define  MNG_SETMOVIE_CODE     30047    // ����&&��ȡ��Ӱģʽ
#define  MNG_GETVER_CODE       30045    // ��ȡ�汾��
#define  MNG_MODE_CODE         30060    // ���õ���̨����ģʽ
#define  MNG_SET_FIRSTPIC_CODE 30061    // �����׻��汣��ʱ��
#define  MNG_USER_KICK_OUT     30080    // ��¼�û���������
#define  MNG_GETDISKSPACE_CODE 30081    // ��ȡ���̿ռ�����

// TCP�������˿�
#define SERVER_PORT            21005

#define FTP_MY_SERVER_PATH     _T("/opt/course")
#define FTP_MAX_READ_SIZE      40960  

// �Զ�����Ϣ
#define WM_MY_SHOW_VIDEO   WM_USER + 1001
#define WM_MY_HEARTBEAT    WM_USER + 1002
#define WM_MY_KICK_OUT     WM_USER + 1003
#define WM_CREATE_RCDWND   WM_USER + 1004
#define WM_FTPWND_CLOSE    WM_USER + 1005
#define WM_CONNECT_UPDATE  WM_USER + 1006

// ��ʱ��ID
#define TIME_HEART_BEAT       2001
#define TIME_START_RCD        2002
#define TIME_SYN_TIME         2003
#define TIME_VLC_WND          2004
#define TIME_CONNECT_STATE    2005

// ��Ƶ������
#define VIDEO_NUM          7
// �μ����޸�������
#define FTP_LESSNAME  0
#define FTP_NOTES     4
#define FTP_TEACHER   6
#define FTP_SUBJECT   7
// �û���
#define ADMIN  _T("admin")
#define GUEST  _T("guest")

#define NORMAL_COLOR       _T("#FF232311")
#define SELECT_COLOR       _T("#FF0F6FDA")
#define SPACE_OVER_COLOR   _T("#FFFF0000")
#define VIDEO_WIDTH_RES_OTHERS        660
#define VIDEO_HEIGHT_RES_OTHERS       404
#define VIDEO_WIDTH_RES_1920    1120
#define VIDEO_HEIGHT_RES_1920   676
#define VIDEO_WIDTH_RES_1600    940
#define VIDEO_HEIGHT_RES_1600   496
#define VIDEO_WIDTH_RES_1366    826
#define VIDEO_HEIGHT_RES_1366   412

// Э��ͷ
typedef struct External_MSG_HEAD
{
	char identifier[4];        // ��ʶ��
	unsigned int version;      // Э��汾��
	unsigned int load_len;     // ���س���
	unsigned int msg_code;     // ��Ϣkey
	short reserver;            // �����ֶ�
}External_Mag_Head;

// ��Ƶ����Ϣ(��š�IP��ַ�Լ��˿�)
typedef struct Stream_info
{
	BOOL bHigh;         // �Ƿ����
	CString strIndex;
	CString strIPAddr;
	CString strPort;    
	CString strCtrlPort;
}StreamInfo;

// ��Ӱģʽ�����Ϣ(�ϳɻ���������ģʽ���ֱ��ʡ�֡���Լ�����)
typedef struct Movie_mode
{
	Movie_mode()
	{
		num = mode = width = height = frameRate = bitRate = 0;
	}
	UINT num;
	UINT mode;
	UINT width;
	UINT height;
	UINT frameRate;
	UINT bitRate;
}MovieMode;

// xml�������������
typedef enum
{
	XML_OPERATE_SUCCESS = 1,                    // �û������ɹ�
	XML_DATA_DETECT_SUCCESS = 0,                // �û��������ɹ�

	// Login
	XML_USER_INVALID = -1,                      // �û�����Ȩ�޲���(�ǹ���Ա�û������û�δ��¼)
	XML_USER_LOGIN_TYPE_INVALID = -2,           // �û���¼���ʹ���
	XML_LOGIN_USERNAME_INVALID = -3,            // ��¼�û��˺Ŵ���
	XML_LOGIN_PASSWORD_INVALID = -4,            // ��¼�������
	XML_PASSKEY_INVALID = -5,                   // ƽ̨��ֵ����

	// FTPTools
	XML_FTP_REQUEST_FILE_NO_EXIST = -6,         // �ļ�������
	XML_FTP_REQUEST_FILE_PARA_ERR = -7,         // ��ѯ�ļ���������ȷ(������ʽС��0��)
	XML_FTP_DELETE_FILE_FAILED = -8,            // ɾ���ļ�ʧ��
	XML_FTP_MODIFY_FILENAME_FAILED = -9,        // �޸Ŀμ���ʧ��
	XML_FTP_MODIFY_FILENAME_CN = -10,           // ��֧�����Ŀμ���
	XML_FTP_MODIFY_FILENAME_SAME = -11,         // �μ����ظ�
	XML_FTP_MODIFY_ORIGIN_FILENAME_NULL = -12,  // û��ԭʼ�μ���
	XML_FTP_MODIFY_NOTES_TOO_LONG = -13,        // ��ע��Ϣ����

	// Set Logo
	XML_SET_LOGO_ALPHA_INVALID = -14,           // ͸���Ȳ�������ȷ
	XML_SET_LOGO_FORMAT_INVALID = -15,          // ͼƬ��ʽ��������ȷ
	XML_SET_LOGO_POSITION_TYPE_INVALID = -16,   // λ�����Ͳ�������ȷ
	XML_SET_LOGO_VALUE_INVALID = -17,           // ����ֵ��������ȷ

	XML_SET_DIRECTOR_MODE_MODEL_INVAID = -18,   // ����ģʽ����ֵ���ڷ�Χ��

	XML_SUBTITLE_ALPHA_INVAID	 = -19,	        // ��Ļ͸���Ȳ�������ȷ
	XML_SUBTITLE_TITLE_TOO_LONG	 = -20,	        // ��Ļ���ȹ�����ֻ֧��23�����֣�46��Ӣ����ĸ
	XML_SUBTITLE_TIME_OPTION_INVAID = -21,	    // ��Ļʱ����ʾ��������ȷ
	XML_SUBTITLE_ENABLE_OPTION_INVAID = -22,	// ��Ļ��ʾʹ�ܲ�������ȷ
	XML_SUBTITLE_POSITION_TYPE_INVAILD	 = -23,	// ��Ļλ�����Ͳ���ȷ
	XML_SUBTITLE_POSITION_VALUE_INVAILD = -24,	// ��Ļλ�ò�������ȷ

	XML_RECORD_STATUS_RECORD_TYPE_INVAID = -25,	// ¼��״̬����������Χ��1 Open record��2-Pause Record��3-Stop Record 

	XML_MOVIE_OPTION_TYPE_INVAID = -26,	        // ��Ӱģʽѡ�����?-Set��Ӱģʽ 0 -Get��Ӱģʽ?
	XML_MOVIE_SOURCE_NUMBER_INVAID = -27,	    // �ϳɻ����������
	XML_MOVIE_MPLAYOUT_INVAID = -28,	        // ��Ӱģʽ��������

	XML_USER_RIGHT_MANAGE = -29,	            // �߳��û�

	XML_UNKOWN_FAILED = -50	                    // δ֪����
}XML_RETURN_CODE_t;

// �໭��ϳ�ģʽ
enum SWITCH_TABLE_T
{
	// ������
	SWITCH_TEA = 1,   // ��ʦ����
	SWITCH_STU,       // ѧ������
	SWITCH_VGA,       // vga
	SWITCH_BLBO,      // ����
	SWITCH_PSTU,      // ѧ��ȫ��
	SWITCH_PTEA,      // ��ʦȫ��
	SWITCH_REMOTE,    // Զ�ˣ�ֻ�л�������������
	
	// ˫����
	SWITCH_2_TEA_EQU_VGA      =	 0x221,    // ��ʦ��------VGA��------  545->4
	SWITCH_2_TEA_EQU_VGA_1    =	 0x1221,   // ����
	
	SWITCH_2_VGA_TEA_1_PIP	  =	 0X231,    // VGA��------��ʦ����------561->2
	SWITCH_2_VGA_TEA_1_PIP_1  =	 0X1231,	  // ����

	SWICTH_2_VGA_TEA_2_PIP	  =  0x241,	  // VGA��------��ʦ����------577->1
	SWICTH_2_VGA_TEA_2_PIP_1  =  0X1241,   // ����

	// ������
	SWITCH_3_VGA_TEA_STU	=	 0X311,    // VGA��------��ʦ����------ѧ������------     785 -> 6
	SWITCH_3_TEA_VGA_STU	=	 0X1311,   // ��ʦ��------VGA ����------ѧ������	
	SWITCH_3_STU_VGA_TEA	=	 0X2311,   // ѧ����------VGA����------��ʦ����

	// �Ļ���
	SWITCH_4_VGA_TEA_STU_PTEA	=	0x411,  // VGA��------��ʦ����------ѧ������------��ʦȫ������------   1041
	SWITCH_4_TEA_VGA_STU_PTEA	=	0x1411,	// ��ʦ����------VGA��------ѧ������------��ʦȫ������------   
	SWITCH_4_STU_TEA_VGA_PTEA	=	0x2411,	// ѧ������------��ʦ����------VGA��------��ʦȫ������------   
	SWITCH_4_PSTU_TEA_STU_VGA	=	0x423,	// ��ʦȫ������------��ʦ����------ѧ������------VGA��------   

	// ������
	SWITCH_6_2	= 0X621, // VGA---��ʦ---ѧ��---��ʦȫ��---ѧ��ȫ��---����
};
