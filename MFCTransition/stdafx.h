
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


#include <afxsock.h>            // MFC 套接字扩展

#define WM_USER_TIMEOUT WM_USER + 1
#define WM_APICOMMNOTIFY WM_USER + 2
#define WM_SEND2SERIAL WM_USER + 3
#define WM_USER_NOTIFYICON WM_USER + 4





#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

////CTpEdit对象
//
//class CTpEdit : public CEdit
//{
//	public:
//	//m_mousedown用来记录鼠标左键是否按下
//		BOOL m_mousedown;
//	protected:
//	//响应如下的消息
//		//{{AFX_MSG(CTpEdit)
//		/*afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg UINT OnGetDlgCode();*/
//	//}}AFX_MSG
//		DECLARE_MESSAGE_MAP()
//
//		//CTpEdit的消息响应函数如下
//		//画背景图
//		BOOL CTpEdit::OnEraseBkgnd(CDC* pDC)
//		{
//			//得到Edit控件的外框，即背景区域
//			RECT updatarect;
//			GetClientRect(&updatarect);
//			//画背景，我画的是一个黄色的矩形
//			CBrush newBrush;
//			newBrush.CreateSolidBrush(RGB(255, 255, 200));
//			CBrush * oldBrush = pDC->SelectObject(&newBrush);
//			pDC->Rectangle(&updatarect);
//			pDC->SelectObject(oldBrush);
//			return TRUE;
//		}
//
//		//强迫Edit控件擦除背景，重写字符
//		UINT CTpEdit::OnGetDlgCode()
//		{
//			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
//			return CEdit::OnGetDlgCode();
//		}
//		//记录鼠标左键是否按下
//		void CTpEdit::OnLButtonDown(UINT nFlags, CPoint point)
//		{
//			m_mousedown = TRUE;
//			SetCapture();
//			CEdit::OnLButtonDown(nFlags, point);
//		}
//
//		void CTpEdit::OnLButtonUp(UINT nFlags, CPoint point)
//		{
//			if (m_mousedown)
//				ReleaseCapture();
//			m_mousedown = FALSE;
//			CEdit::OnLButtonUp(nFlags, point);
//		}
//
//		//如果左键按下并且拖动鼠标就要刷新显示
//		void CTpEdit::OnMouseMove(UINT nFlags, CPoint point)
//		{
//			if (m_mousedown)
//				RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
//			CEdit::OnMouseMove(nFlags, point);
//		}
//		//初始化成员变量
//		CTpEdit::CTpEdit()
//		{
//			m_mousedown = FALSE;
//		}
//
//};



