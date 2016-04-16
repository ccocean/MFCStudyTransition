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

#define RESNAME         _T("课件名")
#define VIDEOSIZE       _T("电影模式大小(MB)")
#define LESSIONSIZE     _T("课件大小(MB)") 
#define RECODESTARTTIME _T("录制开始时间")
#define RCDDURTIME      _T("录制时长(S)")
#define NOTE            _T("备注信息")
#define FTPUPSTATUS     _T("上传状态")
#define FTPDOWNCOUNT    _T("下载次数")
#define TEACHERNAME     _T("课程主讲教师")
#define COURSENAME      _T("课程名字")
#define FTP_NOUPLOAD    _T("未有上传")
#define FTP_YESUPLOAD   _T("已上传")

#define STA_FILENAME    _T("名称")
#define STA_STATUS      _T("状态")
#define STA_PROGRESS    _T("进度")
#define STA_SIZE        _T("大小(MB)")
#define STA_LOCALPATH   _T("本地路径")
#define STA_REMOTEPATH  _T("远程路径")
#define STA_DIRECTION   _T("<->")

#define LOGIN_USERNAME  _T("Username")
#define LOGIN_INFO      _T("LoginInfo")
#define LOGIN_PASSWD    _T("Password")
#define LOGIN_SERVER_IP _T("Server IP")
#define LOGIN_KEY       1

// 协议信令
#define  ALL_LOGIN_CODE        30015    // 登录
// FTP  
#define  FTP_SEARCH_CODE       4099     // FTP查找
#define  FTP_DELETE_CODE       4100     // FTP删除
#define  FTP_CHANGE_CODE       4102     // FTP修改
// ManagePlatform
#define  MNG_REQSTREAM_CODE    30011    // 请求码流
#define  MNG_CTRLCAMERA_CODE   30013    // 设置摄像头远遥
#define  MNG_SETLOGO_CODE      30016    // 设置logo
#define  MNG_SETDATE_CODE      30018    // 设置字幕&&时间
#define  MNG_SETRCD_CODE       30010    // 设置录制状态
#define  MNG_HEARTBEAT_CODE    30032    // 双向心跳
#define  MNG_SETMOVIE_CODE     30047    // 设置&&获取电影模式
#define  MNG_GETVER_CODE       30045    // 获取版本号
#define  MNG_MODE_CODE         30060    // 设置导播台控制模式
#define  MNG_SET_FIRSTPIC_CODE 30061    // 设置首画面保持时间
#define  MNG_USER_KICK_OUT     30080    // 登录用户已满被踢
#define  MNG_GETDISKSPACE_CODE 30081    // 获取磁盘空间容量

// TCP服务器端口
#define SERVER_PORT            21005

#define FTP_MY_SERVER_PATH     _T("/opt/course")
#define FTP_MAX_READ_SIZE      40960  

// 自定义消息
#define WM_MY_SHOW_VIDEO   WM_USER + 1001
#define WM_MY_HEARTBEAT    WM_USER + 1002
#define WM_MY_KICK_OUT     WM_USER + 1003
#define WM_CREATE_RCDWND   WM_USER + 1004
#define WM_FTPWND_CLOSE    WM_USER + 1005
#define WM_CONNECT_UPDATE  WM_USER + 1006

// 定时器ID
#define TIME_HEART_BEAT       2001
#define TIME_START_RCD        2002
#define TIME_SYN_TIME         2003
#define TIME_VLC_WND          2004
#define TIME_CONNECT_STATE    2005

// 视频流数量
#define VIDEO_NUM          7
// 课件可修改项索引
#define FTP_LESSNAME  0
#define FTP_NOTES     4
#define FTP_TEACHER   6
#define FTP_SUBJECT   7
// 用户名
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

// 协议头
typedef struct External_MSG_HEAD
{
	char identifier[4];        // 标识符
	unsigned int version;      // 协议版本号
	unsigned int load_len;     // 负载长度
	unsigned int msg_code;     // 消息key
	short reserver;            // 保留字段
}External_Mag_Head;

// 视频流信息(序号、IP地址以及端口)
typedef struct Stream_info
{
	BOOL bHigh;         // 是否高清
	CString strIndex;
	CString strIPAddr;
	CString strPort;    
	CString strCtrlPort;
}StreamInfo;

// 电影模式相关信息(合成画面数量、模式、分辨率、帧率以及码率)
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

// xml信令操作返回码
typedef enum
{
	XML_OPERATE_SUCCESS = 1,                    // 用户操作成功
	XML_DATA_DETECT_SUCCESS = 0,                // 用户参数检查成功

	// Login
	XML_USER_INVALID = -1,                      // 用户操作权限不够(非管理员用户或者用户未登录)
	XML_USER_LOGIN_TYPE_INVALID = -2,           // 用户登录类型错误
	XML_LOGIN_USERNAME_INVALID = -3,            // 登录用户账号错误
	XML_LOGIN_PASSWORD_INVALID = -4,            // 登录密码错误
	XML_PASSKEY_INVALID = -5,                   // 平台键值错误

	// FTPTools
	XML_FTP_REQUEST_FILE_NO_EXIST = -6,         // 文件不存在
	XML_FTP_REQUEST_FILE_PARA_ERR = -7,         // 查询文件参数不正确(如排序方式小于0等)
	XML_FTP_DELETE_FILE_FAILED = -8,            // 删除文件失败
	XML_FTP_MODIFY_FILENAME_FAILED = -9,        // 修改课件名失败
	XML_FTP_MODIFY_FILENAME_CN = -10,           // 不支持中文课件名
	XML_FTP_MODIFY_FILENAME_SAME = -11,         // 课件名重复
	XML_FTP_MODIFY_ORIGIN_FILENAME_NULL = -12,  // 没有原始课件名
	XML_FTP_MODIFY_NOTES_TOO_LONG = -13,        // 备注信息过长

	// Set Logo
	XML_SET_LOGO_ALPHA_INVALID = -14,           // 透明度参数不正确
	XML_SET_LOGO_FORMAT_INVALID = -15,          // 图片格式参数不正确
	XML_SET_LOGO_POSITION_TYPE_INVALID = -16,   // 位置类型参数不正确
	XML_SET_LOGO_VALUE_INVALID = -17,           // 坐标值参数不正确

	XML_SET_DIRECTOR_MODE_MODEL_INVAID = -18,   // 导播模式参数值不在范围内

	XML_SUBTITLE_ALPHA_INVAID	 = -19,	        // 字幕透明度参数不正确
	XML_SUBTITLE_TITLE_TOO_LONG	 = -20,	        // 字幕长度过长，只支持23个汉字，46个英文字母
	XML_SUBTITLE_TIME_OPTION_INVAID = -21,	    // 字幕时间显示参数不正确
	XML_SUBTITLE_ENABLE_OPTION_INVAID = -22,	// 字幕显示使能参数不正确
	XML_SUBTITLE_POSITION_TYPE_INVAILD	 = -23,	// 字幕位置类型不正确
	XML_SUBTITLE_POSITION_VALUE_INVAILD = -24,	// 字幕位置参数不正确

	XML_RECORD_STATUS_RECORD_TYPE_INVAID = -25,	// 录制状态参数超出范围，1 Open record，2-Pause Record，3-Stop Record 

	XML_MOVIE_OPTION_TYPE_INVAID = -26,	        // 电影模式选项错误?-Set电影模式 0 -Get电影模式?
	XML_MOVIE_SOURCE_NUMBER_INVAID = -27,	    // 合成画面个数错误
	XML_MOVIE_MPLAYOUT_INVAID = -28,	        // 电影模式参数错误

	XML_USER_RIGHT_MANAGE = -29,	            // 踢除用户

	XML_UNKOWN_FAILED = -50	                    // 未知错误
}XML_RETURN_CODE_t;

// 多画面合成模式
enum SWITCH_TABLE_T
{
	// 单画面
	SWITCH_TEA = 1,   // 老师近景
	SWITCH_STU,       // 学生近景
	SWITCH_VGA,       // vga
	SWITCH_BLBO,      // 板书
	SWITCH_PSTU,      // 学生全景
	SWITCH_PTEA,      // 老师全景
	SWITCH_REMOTE,    // 远端，只有互动的主讲才有
	
	// 双画面
	SWITCH_2_TEA_EQU_VGA      =	 0x221,    // 老师左------VGA右------  545->4
	SWITCH_2_TEA_EQU_VGA_1    =	 0x1221,   // 交换
	
	SWITCH_2_VGA_TEA_1_PIP	  =	 0X231,    // VGA大------老师右下------561->2
	SWITCH_2_VGA_TEA_1_PIP_1  =	 0X1231,	  // 交换

	SWICTH_2_VGA_TEA_2_PIP	  =  0x241,	  // VGA大------老师左上------577->1
	SWICTH_2_VGA_TEA_2_PIP_1  =  0X1241,   // 交换

	// 三画面
	SWITCH_3_VGA_TEA_STU	=	 0X311,    // VGA大------老师左上------学生左下------     785 -> 6
	SWITCH_3_TEA_VGA_STU	=	 0X1311,   // 教师大------VGA 左上------学生左下	
	SWITCH_3_STU_VGA_TEA	=	 0X2311,   // 学生大------VGA左上------教师左下

	// 四画面
	SWITCH_4_VGA_TEA_STU_PTEA	=	0x411,  // VGA大------老师左上------学生左中------老师全景左下------   1041
	SWITCH_4_TEA_VGA_STU_PTEA	=	0x1411,	// 老师左上------VGA大------学生左中------老师全景左下------   
	SWITCH_4_STU_TEA_VGA_PTEA	=	0x2411,	// 学生左中------老师左上------VGA大------老师全景左下------   
	SWITCH_4_PSTU_TEA_STU_VGA	=	0x423,	// 老师全景左下------老师左上------学生左中------VGA大------   

	// 六画面
	SWITCH_6_2	= 0X621, // VGA---老师---学生---老师全景---学生全景---板书
};
