
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


#include <afxsock.h>            // MFC �׽�����չ

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

////CTpEdit����
//
//class CTpEdit : public CEdit
//{
//	public:
//	//m_mousedown������¼�������Ƿ���
//		BOOL m_mousedown;
//	protected:
//	//��Ӧ���µ���Ϣ
//		//{{AFX_MSG(CTpEdit)
//		/*afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg UINT OnGetDlgCode();*/
//	//}}AFX_MSG
//		DECLARE_MESSAGE_MAP()
//
//		//CTpEdit����Ϣ��Ӧ��������
//		//������ͼ
//		BOOL CTpEdit::OnEraseBkgnd(CDC* pDC)
//		{
//			//�õ�Edit�ؼ�����򣬼���������
//			RECT updatarect;
//			GetClientRect(&updatarect);
//			//���������һ�����һ����ɫ�ľ���
//			CBrush newBrush;
//			newBrush.CreateSolidBrush(RGB(255, 255, 200));
//			CBrush * oldBrush = pDC->SelectObject(&newBrush);
//			pDC->Rectangle(&updatarect);
//			pDC->SelectObject(oldBrush);
//			return TRUE;
//		}
//
//		//ǿ��Edit�ؼ�������������д�ַ�
//		UINT CTpEdit::OnGetDlgCode()
//		{
//			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
//			return CEdit::OnGetDlgCode();
//		}
//		//��¼�������Ƿ���
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
//		//���������²����϶�����Ҫˢ����ʾ
//		void CTpEdit::OnMouseMove(UINT nFlags, CPoint point)
//		{
//			if (m_mousedown)
//				RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
//			CEdit::OnMouseMove(nFlags, point);
//		}
//		//��ʼ����Ա����
//		CTpEdit::CTpEdit()
//		{
//			m_mousedown = FALSE;
//		}
//
//};



